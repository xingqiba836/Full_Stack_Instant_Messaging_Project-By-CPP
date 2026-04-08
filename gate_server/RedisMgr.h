#ifndef REDIS_MGR_H
#define REDIS_MGR_H

#include "singleton.h"
#include <functional>
#include <memory>
#include <string>

struct redisContext;
struct redisReply;

class RedisMgr : public Singleton<RedisMgr>,
                 public std::enable_shared_from_this<RedisMgr>
{
    friend class Singleton<RedisMgr>;

public:
    ~RedisMgr();

    bool Get(const std::string &key, std::string &value);
    bool Set(const std::string &key, const std::string &value);
    bool SetEx(const std::string &key, const std::string &value, int ttl_seconds);
    bool Del(const std::string &key);
    bool ExistsKey(const std::string &key);

    bool LPush(const std::string &key, const std::string &value);
    bool LPop(const std::string &key, std::string &value);
    bool RPush(const std::string &key, const std::string &value);
    bool RPop(const std::string &key, std::string &value);

    bool HSet(const std::string &key, const std::string &hkey, const std::string &value);
    std::string HGet(const std::string &key, const std::string &hkey);

    void Close();

private:
    RedisMgr();
    bool EnsureReady() const;
    bool ExecWithConnection(const std::function<bool(redisContext *)> &handler);

    class RedisConPool;
    std::unique_ptr<RedisConPool> _con_pool;
};

#endif // REDIS_MGR_H
