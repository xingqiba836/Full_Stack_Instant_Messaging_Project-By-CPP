#include "RedisMgr.h"

#include "ConfigMgr.h"
#include <atomic>
#include <condition_variable>
#include <cstdlib>
#include <iostream>
#include <mutex>
#include <queue>

#include "hiredis/hiredis.h"

class RedisMgr::RedisConPool {
public:
    RedisConPool(std::size_t pool_size, const std::string &host, int port, const std::string &password)
        : _stopped(false)
    {
        for (std::size_t i = 0; i < pool_size; ++i) {
            redisContext *context = redisConnect(host.c_str(), port);
            if (context == nullptr || context->err != 0) {
                if (context != nullptr) {
                    std::cerr << "[RedisConPool] connect failed: " << context->errstr << std::endl;
                    redisFree(context);
                }
                continue;
            }

            if (!password.empty()) {
                redisReply *reply = static_cast<redisReply *>(redisCommand(context, "AUTH %s", password.c_str()));
                if (reply == nullptr || reply->type == REDIS_REPLY_ERROR) {
                    std::cerr << "[RedisConPool] auth failed" << std::endl;
                    if (reply != nullptr) {
                        freeReplyObject(reply);
                    }
                    redisFree(context);
                    continue;
                }
                freeReplyObject(reply);
            }

            _connections.push(context);
        }
    }

    ~RedisConPool()
    {
        Close();
        std::lock_guard<std::mutex> lock(_mutex);
        while (!_connections.empty()) {
            redisContext *context = _connections.front();
            _connections.pop();
            redisFree(context);
        }
    }

    redisContext *GetConnection()
    {
        std::unique_lock<std::mutex> lock(_mutex);
        _condition.wait(lock, [this]() { return _stopped || !_connections.empty(); });
        if (_stopped) {
            return nullptr;
        }
        redisContext *context = _connections.front();
        _connections.pop();
        return context;
    }

    void ReturnConnection(redisContext *context)
    {
        if (context == nullptr) {
            return;
        }
        std::lock_guard<std::mutex> lock(_mutex);
        if (_stopped) {
            redisFree(context);
            return;
        }
        _connections.push(context);
        _condition.notify_one();
    }

    void Close()
    {
        _stopped = true;
        _condition.notify_all();
    }

private:
    std::atomic<bool> _stopped;
    std::queue<redisContext *> _connections;
    std::mutex _mutex;
    std::condition_variable _condition;
};

RedisMgr::RedisMgr()
{
    const std::string host = gConfigMgr["Redis"]["Host"].empty() ? "127.0.0.1" : gConfigMgr["Redis"]["Host"];
    const std::string port_string = gConfigMgr["Redis"]["Port"].empty() ? "6380" : gConfigMgr["Redis"]["Port"];
    const std::string password = gConfigMgr["Redis"]["Passwd"];
    const std::string pool_size_string = gConfigMgr["Redis"]["PoolSize"].empty() ? "5" : gConfigMgr["Redis"]["PoolSize"];

    const int port = std::atoi(port_string.c_str());
    const int parsed_pool_size = std::atoi(pool_size_string.c_str());
    const std::size_t pool_size = parsed_pool_size > 0 ? static_cast<std::size_t>(parsed_pool_size) : 5;

    _con_pool = std::make_unique<RedisConPool>(pool_size, host, port, password);
}

RedisMgr::~RedisMgr()
{
    Close();
}

bool RedisMgr::EnsureReady() const
{
    if (_con_pool == nullptr) {
        std::cerr << "[RedisMgr] connection pool is not initialized" << std::endl;
        return false;
    }
    return true;
}

bool RedisMgr::ExecWithConnection(const std::function<bool(redisContext *)> &handler)
{
    if (!EnsureReady()) {
        return false;
    }

    redisContext *connection = _con_pool->GetConnection();
    if (connection == nullptr) {
        return false;
    }

    const bool result = handler(connection);
    _con_pool->ReturnConnection(connection);
    return result;
}

bool RedisMgr::Get(const std::string &key, std::string &value)
{
    return ExecWithConnection([&](redisContext *connection) {
        redisReply *reply = static_cast<redisReply *>(redisCommand(connection, "GET %s", key.c_str()));
        if (reply == nullptr || reply->type != REDIS_REPLY_STRING) {
            if (reply != nullptr) {
                freeReplyObject(reply);
            }
            return false;
        }
        value = reply->str;
        freeReplyObject(reply);
        return true;
    });
}

bool RedisMgr::Set(const std::string &key, const std::string &value)
{
    return ExecWithConnection([&](redisContext *connection) {
        redisReply *reply = static_cast<redisReply *>(redisCommand(connection, "SET %s %s", key.c_str(), value.c_str()));
        const bool ok = reply != nullptr && reply->type == REDIS_REPLY_STATUS;
        if (reply != nullptr) {
            freeReplyObject(reply);
        }
        return ok;
    });
}

bool RedisMgr::SetEx(const std::string &key, const std::string &value, int ttl_seconds)
{
    return ExecWithConnection([&](redisContext *connection) {
        redisReply *reply = static_cast<redisReply *>(
            redisCommand(connection, "SETEX %s %d %s", key.c_str(), ttl_seconds, value.c_str()));
        const bool ok = reply != nullptr && reply->type == REDIS_REPLY_STATUS;
        if (reply != nullptr) {
            freeReplyObject(reply);
        }
        return ok;
    });
}

bool RedisMgr::Del(const std::string &key)
{
    return ExecWithConnection([&](redisContext *connection) {
        redisReply *reply = static_cast<redisReply *>(redisCommand(connection, "DEL %s", key.c_str()));
        const bool ok = reply != nullptr && reply->type == REDIS_REPLY_INTEGER;
        if (reply != nullptr) {
            freeReplyObject(reply);
        }
        return ok;
    });
}

bool RedisMgr::ExistsKey(const std::string &key)
{
    return ExecWithConnection([&](redisContext *connection) {
        redisReply *reply = static_cast<redisReply *>(redisCommand(connection, "EXISTS %s", key.c_str()));
        const bool ok = reply != nullptr && reply->type == REDIS_REPLY_INTEGER && reply->integer > 0;
        if (reply != nullptr) {
            freeReplyObject(reply);
        }
        return ok;
    });
}

bool RedisMgr::LPush(const std::string &key, const std::string &value)
{
    return ExecWithConnection([&](redisContext *connection) {
        redisReply *reply = static_cast<redisReply *>(redisCommand(connection, "LPUSH %s %s", key.c_str(), value.c_str()));
        const bool ok = reply != nullptr && reply->type == REDIS_REPLY_INTEGER && reply->integer > 0;
        if (reply != nullptr) {
            freeReplyObject(reply);
        }
        return ok;
    });
}

bool RedisMgr::LPop(const std::string &key, std::string &value)
{
    return ExecWithConnection([&](redisContext *connection) {
        redisReply *reply = static_cast<redisReply *>(redisCommand(connection, "LPOP %s", key.c_str()));
        if (reply == nullptr || reply->type != REDIS_REPLY_STRING) {
            if (reply != nullptr) {
                freeReplyObject(reply);
            }
            return false;
        }
        value = reply->str;
        freeReplyObject(reply);
        return true;
    });
}

bool RedisMgr::RPush(const std::string &key, const std::string &value)
{
    return ExecWithConnection([&](redisContext *connection) {
        redisReply *reply = static_cast<redisReply *>(redisCommand(connection, "RPUSH %s %s", key.c_str(), value.c_str()));
        const bool ok = reply != nullptr && reply->type == REDIS_REPLY_INTEGER && reply->integer > 0;
        if (reply != nullptr) {
            freeReplyObject(reply);
        }
        return ok;
    });
}

bool RedisMgr::RPop(const std::string &key, std::string &value)
{
    return ExecWithConnection([&](redisContext *connection) {
        redisReply *reply = static_cast<redisReply *>(redisCommand(connection, "RPOP %s", key.c_str()));
        if (reply == nullptr || reply->type != REDIS_REPLY_STRING) {
            if (reply != nullptr) {
                freeReplyObject(reply);
            }
            return false;
        }
        value = reply->str;
        freeReplyObject(reply);
        return true;
    });
}

bool RedisMgr::HSet(const std::string &key, const std::string &hkey, const std::string &value)
{
    return ExecWithConnection([&](redisContext *connection) {
        redisReply *reply = static_cast<redisReply *>(
            redisCommand(connection, "HSET %s %s %s", key.c_str(), hkey.c_str(), value.c_str()));
        const bool ok = reply != nullptr && reply->type == REDIS_REPLY_INTEGER;
        if (reply != nullptr) {
            freeReplyObject(reply);
        }
        return ok;
    });
}

std::string RedisMgr::HGet(const std::string &key, const std::string &hkey)
{
    std::string value;
    ExecWithConnection([&](redisContext *connection) {
        redisReply *reply = static_cast<redisReply *>(redisCommand(connection, "HGET %s %s", key.c_str(), hkey.c_str()));
        if (reply == nullptr || reply->type != REDIS_REPLY_STRING) {
            if (reply != nullptr) {
                freeReplyObject(reply);
            }
            return false;
        }
        value = reply->str;
        freeReplyObject(reply);
        return true;
    });
    return value;
}

void RedisMgr::Close()
{
    if (_con_pool != nullptr) {
        _con_pool->Close();
    }
}
