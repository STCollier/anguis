#include "client.hpp"

#include <format>
#include <print>

void Client::connect(const std::string& server, const int port, const std::string& username) {
    client.connect(std::format("https://{}:{}", server, port));

    peers.clear();
    peerSnakes.clear();
    dead = false;

    client.socket()->emit("username", username);

    client.socket()->on("connection", [this](const std::string& name, sio::message::ptr data, bool isAck, const sio::message::list& ack_resp) {
        this->on_connection(name, data, isAck, const_cast<sio::message::list&>(ack_resp));
    });

    client.socket()->on("my_id", [this](const std::string& name, sio::message::ptr data, bool isAck, const sio::message::list& ack_resp) {
        this->on_my_id(name, data, isAck, const_cast<sio::message::list&>(ack_resp));
    });

    client.socket()->on("peers", [this](const std::string& name, sio::message::ptr data, bool isAck, const sio::message::list& ack_resp) {
        this->on_peers(name, data, isAck, const_cast<sio::message::list&>(ack_resp));
    });

    client.socket()->on("chat_message", [this](const std::string& name, sio::message::ptr data, bool isAck, const sio::message::list& ack_resp) {
        this->on_chat_message(name, data, isAck, const_cast<sio::message::list&>(ack_resp));
    });

    client.socket()->on("death", [this](const std::string& name, sio::message::ptr data, bool isAck, const sio::message::list& ack_resp) {
        this->on_death(name, data, isAck, const_cast<sio::message::list&>(ack_resp));
    });

    client.socket()->on("disconnection", [this](const std::string& name, sio::message::ptr data, bool isAck, const sio::message::list& ack_resp) {
        this->on_disconnection(name, data, isAck, const_cast<sio::message::list&>(ack_resp));
    });
}

void Client::disconnect() {
    client.close();
}

void Client::kys(std::string& you, std::string& me) {
    std::vector<std::string> messages {
        "killed",
        "laid to rest",
        "massacred",
        "wormed",
        "cut off"
    };

    std::mt19937 gen(std::random_device{}());
    std::uniform_int_distribution<std::size_t> dist(0, messages.size() - 1);

    std::string msg = std::format("{} was {} by {}", you, messages[dist(gen)], me);

    client.socket()->emit("death", msg);
}

void Client::on_connection([[maybe_unused]] const std::string& name, sio::message::ptr data, [[maybe_unused]] bool isAck, [[maybe_unused]] sio::message::list& ack_resp) {
    auto map = data->get_map();

    std::string id = map["id"]->get_string();
    std::string username = map["username"]->get_string();

    chatMessages.push_back(std::format("{} connected", username));
}

void Client::erasePeer(const std::string& id) {
    peers.erase(id);
    peerSnakes.erase(id);
}

void Client::on_disconnection([[maybe_unused]] const std::string& name, sio::message::ptr data, [[maybe_unused]] bool isAck, [[maybe_unused]] sio::message::list& ack_resp) {
    auto map = data->get_map();
    
    std::string id = map["id"]->get_string();
    std::string username = map["username"]->get_string();

    erasePeer(id);

    chatMessages.push_back(std::format("{} disconnected", username));
}

void Client::on_peers([[maybe_unused]] const std::string& name, sio::message::ptr data, [[maybe_unused]] bool isAck, [[maybe_unused]] sio::message::list& ack_resp) {
    auto peerList = data->get_vector();
    
    for (auto& peer : peerList) {
        auto map = peer->get_map();

        std::string id = map["id"]->get_string();
        std::string username = map["username"]->get_string();
        auto positions = map["positions"]->get_vector();

        std::vector<glm::vec2> segmentList;
        for (auto& segment : positions) {
            auto vec = segment->get_vector();

            float x = static_cast<float>(vec[0]->get_double());
            float y = static_cast<float>(vec[1]->get_double());

            segmentList.push_back(glm::vec2(x, y));
        }

        peers[id] = PeerData { username, segmentList };
    }
}

void Client::on_my_id([[maybe_unused]] const std::string& name, sio::message::ptr data, [[maybe_unused]] bool isAck, [[maybe_unused]] sio::message::list& ack_resp) {
    socketID = data->get_string();
}

void Client::on_chat_message([[maybe_unused]] const std::string& name, sio::message::ptr data, [[maybe_unused]] bool isAck, [[maybe_unused]] sio::message::list& ack_resp) {
    auto map = data->get_map();
    
    std::string username = map["username"]->get_string();
    std::string message = map["message"]->get_string();

    chatMessages.push_back(std::format("{}: {}", username, message));
}


void Client::on_death([[maybe_unused]] const std::string& name, sio::message::ptr data, [[maybe_unused]] bool isAck, [[maybe_unused]] sio::message::list& ack_resp) {
    auto map = data->get_map();

    std::string message = map["message"]->get_string();

    chatMessages.push_back(message);
    disconnect();
}

void Client::renderPeers(Shader& mainShader, Shader& fadeShader, Shader& particleShader, Camera& camera, Lighting& lighting, State& state, float dt) {
    for (auto& [id, snake] : peerSnakes) {
        snake.get()->render(mainShader, fadeShader, particleShader, camera, lighting, state, dt);
    }
}

static float rate = 1.f / 30.f; // 30 updates per second
static float t = 0.f;

void Client::update(Snake& snake, float dt) {
    for (const auto& [id, peerData] : peers) {
        auto it = peerSnakes.find(id);
        if (id != socketID) {
            if (it == peerSnakes.end()) {
                peerSnakes[id] = std::make_unique<Snake>(peerData.positions);
            } else {
                peerSnakes[id].get()->setPositions(peerData.positions);
            }
        }
    }

    for (auto& [id, pSnake] : peerSnakes) {
        if (snake.collideWithOther(pSnake.get()->positions)) {
            if (!dead) {
                kys(username, peers[id].username);
                dead = true;
            }
            snake.die();
        }
    }

    // Both server and client send/recieve data 30 times per second
    t += dt;
    while (t >= rate) {
        emitPeerData(snake);
        t -= rate;
    }
}

void Client::emitPeerData(Snake& snake) {
    sio::message::ptr peerData = sio::object_message::create();

    // We don't need to send over username because server already knows it
    peerData->get_map()["id"] = sio::string_message::create(socketID);

    sio::message::ptr positions = sio::array_message::create();
    for (const auto& pos : snake.positions) {
        sio::message::ptr vec = sio::array_message::create();

        vec->get_vector().push_back(sio::double_message::create(pos.x));
        vec->get_vector().push_back(sio::double_message::create(pos.y));
    
        positions->get_vector().push_back(vec);
    }

    peerData->get_map()["positions"] = positions;
    
    client.socket()->emit("peer", peerData);
}

void Client::emitChatMessage(const std::string& message) {
    client.socket()->emit("chat_message", sio::string_message::create(message));
}