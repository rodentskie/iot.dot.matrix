const { connect, disconnect } = require("mongoose");
const { config } = require("dotenv");

config();

const start = async () => {
  const uri = process.env.MONGO_URI || "mongodb://localhost/iot";
  console.log(uri);
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
