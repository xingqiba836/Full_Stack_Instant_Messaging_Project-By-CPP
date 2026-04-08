const { createClient } = require("redis");
const configModule = require("./config");

let client = null;

function getRedisUrl() {
  const host = configModule.redisHost;
  const port = configModule.redisPort;
  if (configModule.redisPassword) {
    return `redis://:${encodeURIComponent(configModule.redisPassword)}@${host}:${port}`;
  }
  return `redis://${host}:${port}`;
}

async function getClient() {
  if (client && client.isOpen) {
    return client;
  }

  if (!client) {
    client = createClient({ url: getRedisUrl() });
    client.on("error", (err) => {
      console.log("redis client error:", err);
    });
  }

  if (!client.isOpen) {
    await client.connect();
  }
  return client;
}

async function setVerifyCode(email, code, ttlSeconds) {
  const redisClient = await getClient();
  const key = `${configModule.codePrefix}${email}`;
  await redisClient.set(key, code, { EX: ttlSeconds });
}

module.exports = { setVerifyCode };
