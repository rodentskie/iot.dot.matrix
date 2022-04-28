const { connect, disconnect } = require("mongoose");

const start = async () => {
  const uri = "mongodb://localhost/iot";

  await connect(uri, {
    useNewUrlParser: true,
    useUnifiedTopology: true,
    useFindAndModify: false,
    useCreateIndex: true,
  });
};

const stop = async () => {
  await disconnect();
};

module.exports = {
  start,
  stop,
};
