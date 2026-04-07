#ifndef VERIFY_EMAIL_SERVICE_CLIENT_H
#define VERIFY_EMAIL_SERVICE_CLIENT_H

#include "const.h"
#include "singleton.h"
#include <condition_variable>
#include <memory>
#include <mutex>
#include <queue>
#include <string>

struct VerifyEmailResponse {
    int error = ErrorCodes::EmailServiceFailed;
    std::string email;
};

class VerifyEmailServiceClient : public Singleton<VerifyEmailServiceClient>
{
    friend class Singleton<VerifyEmailServiceClient>;

public:
    ~VerifyEmailServiceClient();
    VerifyEmailResponse GetVerifyCode(const std::string &email);

private:
    struct PooledConnection {
        net::io_context io_context;
        tcp::resolver resolver;
        beast::tcp_stream stream;
        bool connected;
        std::size_t request_count;

        PooledConnection()
            : resolver(io_context), stream(io_context), connected(false), request_count(0) {}
    };

    VerifyEmailServiceClient();
    std::shared_ptr<PooledConnection> AcquireConnection();
    void ReturnConnection(const std::shared_ptr<PooledConnection> &connection);
    bool EnsureConnected(const std::shared_ptr<PooledConnection> &connection);
    void ResetConnection(const std::shared_ptr<PooledConnection> &connection);

    std::mutex _pool_mutex;
    std::condition_variable _pool_condition;
    std::queue<std::shared_ptr<PooledConnection>> _connection_pool;
    std::string _host;
    std::string _port;
    std::string _path;
    std::size_t _max_requests_per_connection;
    std::chrono::seconds _connect_timeout;
    std::chrono::seconds _io_timeout;
    bool _stopped;
};

#endif // VERIFY_EMAIL_SERVICE_CLIENT_H
