#include "LogicSystem.h"
#include "HttpConnection.h"
#include "RedisMgr.h"
#include "VerifyEmailServiceClient.h"

LogicSystem::~LogicSystem() {}

void LogicSystem::RegisterGetHandler(std::string url, HttpHandler handler) {
    _get_handlers.insert(make_pair(url, handler));
}

void LogicSystem::RegisterPostHandler(std::string url, HttpHandler handler) {
    _post_handlers.insert(make_pair(url, handler));
}

LogicSystem::LogicSystem() {
    RegisterGetHandler("/get_test", [](std::shared_ptr<HttpConnection> connection) {
        beast::ostream(connection->_response.body()) << "receive get_test req " << std::endl;
        int i = 0;
        for (auto &elem : connection->_get_params) {
            i++;
            beast::ostream(connection->_response.body()) << "param" << i << " key is " << elem.first;
            beast::ostream(connection->_response.body()) << ", value is " << elem.second << std::endl;
        }
    });

    RegisterPostHandler("/get_varifycode", [](std::shared_ptr<HttpConnection> connection) {
        auto body_string = boost::beast::buffers_to_string(connection->_request.body().data());
        std::cout << "receive body is " << body_string << std::endl;
        std::cout << "receive body size is " << body_string.size() << std::endl;

        connection->_response.set(http::field::content_type, "application/json");

        Json::Value response_json;
        Json::Reader json_reader;
        Json::Value request_json;
        bool parse_success = json_reader.parse(
            body_string.c_str(),
            body_string.c_str() + body_string.size(),
            request_json,
            false);
        if (!parse_success) {
            std::cout << "Failed to parse JSON data!" << std::endl;
            std::cout << json_reader.getFormattedErrorMessages() << std::endl;
            response_json["error"] = ErrorCodes::Error_Json;
            std::string json_string = response_json.toStyledString();
            beast::ostream(connection->_response.body()) << json_string;
            return;
        }

        auto email = request_json["email"].asString();
        VerifyEmailResponse verify_response = VerifyEmailServiceClient::GetInstance()->GetVerifyCode(email);
        std::cout << "email is " << email << std::endl;
        response_json["error"] = verify_response.error;
        response_json["email"] = request_json["email"];
        std::string json_string = response_json.toStyledString();
        beast::ostream(connection->_response.body()) << json_string;
    });

    RegisterPostHandler("/user_register", [](std::shared_ptr<HttpConnection> connection) {
        auto body_str = boost::beast::buffers_to_string(connection->_request.body().data());
        std::cout << "receive body is " << body_str << std::endl;
        connection->_response.set(http::field::content_type, "application/json");

        Json::Value root;
        Json::Reader reader;
        Json::Value src_root;
        bool parse_success = reader.parse(body_str, src_root);
        if (!parse_success) {
            std::cout << "Failed to parse JSON data!" << std::endl;
            root["error"] = ErrorCodes::Error_Json;
            beast::ostream(connection->_response.body()) << root.toStyledString();
            return;
        }

        const std::string user = src_root.get("user", "").asString();
        const std::string email = src_root.get("email", "").asString();
        const std::string passwd = src_root.get("passwd", "").asString();
        const std::string confirm = src_root.get("confirm", "").asString();
        const std::string varifycode = src_root.get("varifycode", "").asString();

        if (user.empty() || email.empty() || passwd.empty() || confirm.empty() || varifycode.empty()) {
            root["error"] = ErrorCodes::Error_Json;
            beast::ostream(connection->_response.body()) << root.toStyledString();
            return;
        }

        if (passwd != confirm) {
            root["error"] = ErrorCodes::Error_Json;
            beast::ostream(connection->_response.body()) << root.toStyledString();
            return;
        }

        const std::string code_key = std::string("code_") + email;
        std::string redis_code;
        const bool got_code = RedisMgr::GetInstance()->Get(code_key, redis_code);
        if (!got_code) {
            std::cout << " get varify code expired" << std::endl;
            root["error"] = ErrorCodes::VarifyExpired;
            beast::ostream(connection->_response.body()) << root.toStyledString();
            return;
        }

        if (redis_code != varifycode) {
            std::cout << " varify code error" << std::endl;
            root["error"] = ErrorCodes::VarifyCodeErr;
            beast::ostream(connection->_response.body()) << root.toStyledString();
            return;
        }

        if (RedisMgr::GetInstance()->ExistsKey(user)) {
            std::cout << " user exist" << std::endl;
            root["error"] = ErrorCodes::UserExist;
            beast::ostream(connection->_response.body()) << root.toStyledString();
            return;
        }

        if (!RedisMgr::GetInstance()->Set(user, email)) {
            root["error"] = ErrorCodes::EmailServiceFailed;
            beast::ostream(connection->_response.body()) << root.toStyledString();
            return;
        }

        RedisMgr::GetInstance()->Del(code_key);

        root["error"] = ErrorCodes::Success;
        root["email"] = email;
        root["user"] = user;
        beast::ostream(connection->_response.body()) << root.toStyledString();
    });
}

bool LogicSystem::HandleGetRequest(std::string path, std::shared_ptr<HttpConnection> con) {
    if (_get_handlers.find(path) == _get_handlers.end()) {
        // 记录未找到处理程序的请求
        std::cerr << "No handler found for URL: " << path << std::endl;
        return false;
    }

    // 记录请求处理
    std::cout << "Found handler for URL: " << path << std::endl;
    _get_handlers[path](con);
    return true;
}

bool LogicSystem::HandlePostRequest(std::string path, std::shared_ptr<HttpConnection> con) {
    if (_post_handlers.find(path) == _post_handlers.end()) {
        std::cerr << "No POST handler found for URL: " << path << std::endl;
        return false;
    }

    std::cout << "Found POST handler for URL: " << path << std::endl;
    _post_handlers[path](con);
    return true;
}