const { Schema, model } = require("mongoose");

const schema = new Schema(
  {
    message: { type: String, required: true },
    createdAt: Date,
    updatedAt: Date,
  },
  {
    timestamps: true,
  }
);

const IoT = model("iot", schema);

module.exports = IoT;
