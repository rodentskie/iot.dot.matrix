const exitHook = require("async-exit-hook");
const { start, stop } = require("./service");

const delay = async (sec) => {
  return new Promise((resolve) => setTimeout(resolve, sec * 1000));
};

start();

exitHook(async (callback) => {
  await stop();
  await delay(10);
  callback();
});

exitHook.uncaughtExceptionHandler(async () => {
  await delay(10);
  process.exit(-1);
});
