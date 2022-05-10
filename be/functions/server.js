const { WebSocketServer } = require("ws");

const { insertMessage, getLatestMessage } = require("./queries");

class MyWebSocketServer {
  static server;
  static wss;
  constructor(server) {
    this.server = server;
  }

  async start() {
    console.log("Starting WebSocket Server ..");

    this.wss = new WebSocketServer({ server: this.server });

    this.wss.on("connection", (ws) => {
      console.log("💻 A client has connected.");
      ws.on("message", async (data) => {
        console.log("💬 Received: %s", data);
        const received = JSON.parse(data);
        if (received.isEmit) {
          await insertMessage(received.msg);

          this.wss.clients.forEach(function each(client) {
            if (client !== ws && client.readyState === client.OPEN) {
              client.send(
                JSON.stringify({
                  broadcast: true,
                  msgs: received.msg,
                })
              );
            }
          });
        }

        if (received.latest) {
          const data = await getLatestMessage();
          if (!data) return;
          const { message } = data;
          ws.send(
            JSON.stringify({
              msgs: message,
            })
          );
        }
      });
    });
  }

  async close() {
    console.log("Closing WebSocket Server ..");

    this.wss.clients.forEach((socket) => {
      socket.close();
    });

    setTimeout(() => {
      this.wss.clients.forEach((socket) => {
        if ([socket.OPEN, socket.CLOSING].includes(socket.readyState)) {
          socket.terminate();
        }
      });
    }, 5000);

    this.wss.close();
  }
}

module.exports = MyWebSocketServer;
