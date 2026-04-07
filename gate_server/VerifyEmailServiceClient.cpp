#include "VerifyEmailServiceClient.h"

#include "ConfigMgr.h"
#include <cstdlib>
#include <iostream>
#include <sstream>

VerifyEmailServiceClient::VerifyEmailServiceClient()
    : _host(gConfigMgr["VarifyServer"]["Host"].empty() ? "127.0.0.1" : gConfigMgr["VarifyServer"]["Host"]),
      _port(gConfigMgr["VarifyServer"]["Port"].empty() ? "5000" : gConfigMgr["VarifyServer"]["Port"]),
      _path(gConfigMgr["VarifyServer"]["Path"].empty() ? "/api/send_verify_code" : gConfigMgr["VarifyServer"]["Path"]),
      _max_requests_per_connection(100),
      _connect_timeout(std::chrono::seconds(2)),
      _io_timeout(std::chrono::seconds(3)),
      _stopped(false)
{
    const std::string pool_size_string = gConfigMgr["VarifyServer"]["HttpPoolSize"];
    const std::string max_requests_string = gConfigMgr["VarifyServer"]["MaxRequestsPerConnection"];
    const std::string connect_timeout_string = gConfigMgr["VarifyServer"]["ConnectTimeoutSeconds"];
    const std::string io_timeout_string = gConfigMgr["VarifyServer"]["IoTimeoutSeconds"];
    std::size_t pool_size = 4;
    if (!pool_size_string.empty()) {
        const int parsed = std::atoi(pool_size_string.c_str());
        if (parsed > 0) {
            pool_size = static_cast<std::size_t>(parsed);
        }
    }
    if (!max_requests_string.empty()) {
        const int parsed = std::atoi(max_requests_string.c_str());
        if (parsed > 0) {
            _max_requests_per_connection = static_cast<std::size_t>(parsed);
        }
    }
    if (!connect_timeout_string.empty()) {
        const int parsed = std::atoi(connect_timeout_string.c_str());
        if (parsed > 0) {
            _connect_timeout = std::chrono::seconds(parsed);
        }
    }
    if (!io_timeout_string.empty()) {
        const int parsed = std::atoi(io_timeout_string.c_str());
        if (parsed > 0) {
            _io_timeout = std::chrono::seconds(parsed);
        }
    }

    for (std::size_t index = 0; index < pool_size; ++index) {
        _connection_pool.push(std::make_shared<PooledConnection>());
    }
}

VerifyEmailServiceClient::~VerifyEmailServiceClient()
{
    std::lock_guard<std::mutex> lock(_pool_mutex);
    _stopped = true;
    while (!_connection_pool.empty()) {
        auto connection = _connection_pool.front();
        _connection_pool.pop();
        ResetConnection(connection);
    }
    _pool_condition.notify_all();
}

std::shared_ptr<VerifyEmailServiceClient::PooledConnection> VerifyEmailServiceClient::AcquireConnection()
{
    std::unique_lock<std::mutex> lock(_pool_mutex);
    _pool_condition.wait(lock, [this]() {
        return _stopped || !_connection_pool.empty();
    });

    if (_stopped) {
        return nullptr;
    }

    auto connection = _connection_pool.front();
    _connection_pool.pop();
    return connection;
}

void VerifyEmailServiceClient::ReturnConnection(const std::shared_ptr<PooledConnection> &connection)
{
    if (!connection) {
        return;
    }

    std::lock_guard<std::mutex> lock(_pool_mutex);
    if (_stopped) {
        return;
    }
    _connection_pool.push(connection);
    _pool_condition.notify_one();
}

bool VerifyEmailServiceClient::EnsureConnected(const std::shared_ptr<PooledConnection> &connection)
{
    if (connection->connected && connection->stream.socket().is_open()) {
        return true;
    }

    beast::error_code error_code;
    connection->stream.socket().close(error_code);
    auto const results = connection->resolver.resolve(_host, _port, error_code);
    if (error_code) {
        return false;
    }

    connection->stream.expires_after(_connect_timeout);
    connection->stream.connect(results, error_code);
    if (error_code) {
        return false;
    }

    connection->connected = true;
    connection->request_count = 0;
    return true;
}

void VerifyEmailServiceClient::ResetConnection(const std::shared_ptr<PooledConnection> &connection)
{
    if (!connection) {
        return;
    }
    beast::error_code error_code;
    connection->stream.socket().shutdown(tcp::socket::shutdown_both, error_code);
    connection->stream.socket().close(error_code);
    connection->connected = false;
    connection->request_count = 0;
}

VerifyEmailResponse VerifyEmailServiceClient::GetVerifyCode(const std::string &email)
{
    VerifyEmailResponse response;
    response.email = email;
    auto connection = AcquireConnection();
    if (!connection) {
        response.error = ErrorCodes::EmailServiceFailed;
        return response;
    }

    try {
        if (!EnsureConnected(connection)) {
            response.error = ErrorCodes::EmailServiceFailed;
            ResetConnection(connection);
            ReturnConnection(connection);
            return response;
        }

        Json::Value request_json;
        request_json["email"] = email;
        Json::StreamWriterBuilder writer_builder;
        writer_builder["indentation"] = "";
        const std::string request_body = Json::writeString(writer_builder, request_json);

        http::request<http::string_body> request{http::verb::post, _path, 11};
        request.set(http::field::host, _host);
        request.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
        request.set(http::field::content_type, "application/json");
        request.set(http::field::connection, "keep-alive");
        request.body() = request_body;
        request.prepare_payload();

        connection->stream.expires_after(_io_timeout);
        http::write(connection->stream, request);

        beast::flat_buffer buffer;
        http::response<http::string_body> http_response;
        connection->stream.expires_after(_io_timeout);
        http::read(connection->stream, buffer, http_response);

        if (http_response.result() != http::status::ok) {
            response.error = ErrorCodes::EmailServiceFailed;
        } else {
            Json::Value response_json;
            Json::CharReaderBuilder reader_builder;
            std::string errors;
            std::istringstream response_stream(http_response.body());
            if (!Json::parseFromStream(reader_builder, response_stream, &response_json, &errors)) {
                response.error = ErrorCodes::Error_Json;
            } else {
                response.error = response_json.get("error", ErrorCodes::EmailServiceFailed).asInt();
            }
        }

        connection->request_count++;
        if (connection->request_count >= _max_requests_per_connection) {
            // Periodically recreate keep-alive connections to avoid long-lived stale sockets.
            ResetConnection(connection);
        }
    } catch (const std::exception &exception) {
        std::cerr << "[VerifyEmailServiceClient] http request failed: " << exception.what() << std::endl;
        response.error = ErrorCodes::EmailServiceFailed;
        ResetConnection(connection);
    }

    ReturnConnection(connection);
    return response;
}
