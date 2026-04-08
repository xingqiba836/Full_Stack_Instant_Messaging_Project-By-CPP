const fs = require("fs");
const path = require("path");

const configPath = path.join(__dirname, "config.json");
const config = JSON.parse(fs.readFileSync(configPath, "utf8"));

const emailUser = config.email.user;
const emailPass = config.email.pass;
const httpHost = (config.http && config.http.host) || "0.0.0.0";
const httpPort = (config.http && config.http.port) || "5000";
const redisHost = (config.redis && config.redis.host) || "127.0.0.1";
const redisPort = (config.redis && config.redis.port) || "6380";
const redisPassword = (config.redis && config.redis.password) || "";
const codePrefix = "code_";

module.exports = {
  emailUser,
  emailPass,
  httpHost,
  httpPort,
  redisHost,
  redisPort,
  redisPassword,
  codePrefix
};
