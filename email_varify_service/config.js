const fs = require("fs");
const path = require("path");

const configPath = path.join(__dirname, "config.json");
const config = JSON.parse(fs.readFileSync(configPath, "utf8"));

const emailUser = config.email.user;
const emailPass = config.email.pass;
const httpHost = (config.http && config.http.host) || "0.0.0.0";
const httpPort = (config.http && config.http.port) || "5000";

module.exports = { emailUser, emailPass, httpHost, httpPort };
