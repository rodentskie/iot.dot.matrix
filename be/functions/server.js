const { WebSocketServer } = require("ws");

const { insertMessage, getLatestMessage } = require("./queries");

class MyWebSocketServer {
  static server;

  constructor(server) {
    this.server = server;
  }

  async start() {
    const wss = new WebSocketServer({ server: this.server });

    wss.on("connection", (ws) => {
      console.log("💻 A client has connected.");
      ws.on("message", async (data) => {
        console.log("💬 Received: %s", data);
        const received = JSON.parse(data);
        await insertMessage(received.msg);
        if (received.isEmit) {
          ws.send(
            JSON.stringify({
              received: true,
              msgs: received.msg,
            })
          );
        }
        if (received.latest) {
          const data = await getLatestMessage();
          const { message } = data;
          ws.send(
            JSON.stringify({
              msgs: message,
            })
          );
        }

        wss.clients.forEach(function each(client) {
          if (client !== ws && client.readyState === WebSocketServer.OPEN) {
            client.send(
              JSON.stringify({
                broadcast: true,
                msgs: received.msg,
              })
            );
          }
        });
      });
    });
  }
}

module.exports = MyWebSocketServer;
