const Koa = require("koa");
const MyWebSocketServer = require("./functions/server");
const db = require("./data");
const { Server } = require("http");

let server = new Server();

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

  const ws = new MyWebSocketServer(server);
  ws.start();

  return server;
};

const stop = async () => {
  await new Promise((resolve) => server.close(resolve));
  await db.stop();
};

module.exports = {
  start,
  stop,
};
