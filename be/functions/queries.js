const IoT = require("../models/iot");

const insertMessage = async (msg) => {
  try {
    await IoT.create({
      message: msg,
    });
    return true;
  } catch (e) {
    return false;
  }
};

const getLatestMessage = async () => {
  try {
    const res = await IoT.findOne({}, ["message"], {
      sort: {
        updatedAt: -1,
      },
      limit: 1,
    });
    return res;
  } catch (e) {
    console.log("Error: ", e);
  }
};

module.exports = {
  insertMessage,
  getLatestMessage,
};
