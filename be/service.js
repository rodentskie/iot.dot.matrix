const Koa = require("koa");
const MyWebSocketServer = require("./functions/server");
const db = require("./data");
const { Server } = require("http");

let server = new Server();
let ws = MyWebSocketServer;

const start = async () => {
  const app = new Koa();

  await db.start();
  const PORT = process.env.PORT || 3000;
  server = app.listen(PORT, () => {
    console.log(`🚀⚙️  Server ready at http://localhost:${PORT}`);
  });

  app.use((ctx) => {
    ctx.body = "Welcome IoT.";
  });

  ws = new MyWebSocketServer(server);
  ws.start();

  return server;
};

const stop = async () => {
  ws.close();
  await db.stop();
  await new Promise((resolve) => server.close(resolve));
};

module.exports = {
  start,
  stop,
};
