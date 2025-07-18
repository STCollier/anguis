const { Server } = require("socket.io");
const http = require("http");

const server = http.createServer();
const PORT = 3000;

const io = new Server(server, {
    cors: {
        origin: "*",
    }
});

let segmentSize = 1.0;
let peers = [];

io.on("connection", (socket) => {
    console.log("Client connected:", socket.id);

    // New username connected to game; server tells all clients except that user
    socket.on("username", (username) => {
        console.log(socket.id + " is connected as " + username);

        socket.emit("my_id", socket.id);
        io.emit("connection", {
            id: socket.id,
            username: username
        })

        let head = [0, 0];

        let positions = [head];
        
        for (let i = 1; i < 10; i++) {
            positions.push([0, (i * segmentSize * 2)]);
        }

        peers.push({
            id: socket.id,
            username: username,
            positions: positions,
            score: positions.length,
            didEvent: false
        })
    });

    socket.on("peer", (data) => {
        const id = data.id;
        const positions = data.positions; // array of [x, y] arrays
    
        for (let i = 0; i < peers.length; i++) {
            if (peers[i].id == id) {
                peers[i].positions = positions;
            }
        }
    });

    socket.on("chat_message", (data) => {
        const idx = peers.findIndex(p => p.id === socket.id);

        io.emit("chat_message", {
            username: peers[idx].username,
            message: data
        });
    })

    socket.on("death", (data) => {
        io.emit("death", {
            message: data
        })
    })

    socket.on("disconnect", (reason) => {
        const idx = peers.findIndex(p => p.id === socket.id);

        if (idx !== -1) {
            io.emit("disconnection", {
                id: peers[idx].id,
                username: peers[idx].username

            });

            peers.splice(idx, 1);
        }

        console.log("Client disconneted, reason: ", reason);
    })
});

setInterval(() => {
    io.emit("peers", peers);
}, 1000 / 30);

server.listen(PORT, () => {
    console.log(`Anguis server running at http://localhost:${PORT}`);
});