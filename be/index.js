const Koa = require("koa");
const MyWebSocketServer = require("./server");

const app = new Koa();

const PORT = process.env.PORT || 3000;
const server = app.listen(PORT, () => {
  console.log(`🚀⚙️  Server ready at http://localhost:${PORT}`);
});

app.use((ctx) => {
  ctx.body = "Welcome IoT.";
});

const ws = new MyWebSocketServer(server);
ws.start();

module.exports = {
  server,
};
