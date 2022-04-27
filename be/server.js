const { WebSocketServer } = require("ws");

class MyWebSocketServer {
  static server;

  constructor(server) {
    this.server = server;
  }

  async start() {
    const wss = new WebSocketServer({ server: this.server });

    wss.on("connection", (ws) => {
      console.log("💻 A client has connected.");
      ws.on("message", (data) => {
        console.log("💬 Received: %s", data);
      });
    });
  }
}

module.exports = MyWebSocketServer;
