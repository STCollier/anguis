#pragma once

#include <sio_client.h>
#include <glm/glm.hpp>
#include <map>
#include <iostream>

#include "shader.hpp"
#include "../anguis/snake.hpp"

struct PeerData {
    std::string username;
    std::vector<glm::vec2> positions;
};

class Client {
    public:
        void connect(const std::string& server, const int port, const std::string& username);
        void renderPeers(Shader& mainShader, Shader& fadeShader, Shader& particleShader, Camera& camera, Lighting& lighting, State& state, float dt);
        void erasePeer(const std::string& id);
        void disconnect();
        void kys(std::string& you, std::string& me);
        void update(Snake& snake, float dt);
        void emitPeerData(Snake& snake);
        void emitChatMessage(const std::string& message);

        void setUsername(std::string& name) { username = name; }
        std::vector<std::string> getChatMessages() { return chatMessages; }

    private:
        sio::client client;
        std::string socketID;
        std::string username = "Player";
        bool dead = false;

        // key = socketID
        std::map<std::string, PeerData> peers;
        std::map<std::string, std::unique_ptr<Snake>> peerSnakes;

        std::vector<std::string> chatMessages;

        // Callbacks
        void on_connection(const std::string& name, sio::message::ptr data, bool isAck, sio::message::list& ack_resp);
        void on_peers(const std::string& name, sio::message::ptr data, bool isAck, sio::message::list& ack_resp);
        void on_my_id(const std::string& name, sio::message::ptr data, bool isAck, sio::message::list& ack_resp);
        void on_chat_message(const std::string& name, sio::message::ptr data, bool isAck, sio::message::list& ack_resp);
        void on_death(const std::string& name, sio::message::ptr data, bool isAck, sio::message::list& ack_resp);
        void on_disconnection(const std::string& name, sio::message::ptr data, bool isAck, sio::message::list& ack_resp);

};