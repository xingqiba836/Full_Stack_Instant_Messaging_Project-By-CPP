#include "HttpConnection.h"
#include "LogicSystem.h"
#include <sstream>

HttpConnection::HttpConnection(net::io_context &io_context)
    : _socket(io_context) {
}

tcp::socket &HttpConnection::GetSocket() {
    return _socket;
}

void HttpConnection::StartReadingRequest()
{
    auto self = shared_from_this();
    http::async_read(_socket, _buffer, _request, [self](beast::error_code error_code,
        std::size_t bytes_transferred) {
            try {
                if (error_code) {
                    std::cout << "http read error is " << error_code.what() << std::endl;
                    return;
                }

                //处理读到的数据
                boost::ignore_unused(bytes_transferred);
                self->HandleRequest();
                self->CheckDeadline();
            }
            catch (std::exception &exception) {
                std::cout << "exception is " << exception.what() << std::endl;
            }
        }
    );
}

void HttpConnection::HandleRequest() {
    // 设置版本
    _response.version(_request.version());
    // 设置为短链接
    _response.keep_alive(false);

    // 记录请求开始
    std::cout << "Handling request: " << _request.method_string().data() << " " << _request.target() << std::endl;
    
    if (_request.method() == http::verb::get) {
        ParseGetParameters();
        // 记录URL解析结果
        std::cout << "Parsed URL: " << _get_url << std::endl;
        for (const auto& param : _get_params) {
            std::cout << "Query parameter: " << param.first << " = " << param.second << std::endl;
        }
        
        bool success = LogicSystem::GetInstance()->HandleGetRequest(_get_url, shared_from_this());
        if (!success) {
            _response.result(http::status::not_found);
            _response.set(http::field::content_type, "text/plain");
            beast::ostream(_response.body()) << "url not found\r\n";
            // 记录未找到URL的警告
            std::cerr << "Warning: URL not found - " << _get_url << std::endl;
            WriteResponse();
            return;
        }

        _response.result(http::status::ok);
        _response.set(http::field::server, "GateServer");
        // 记录成功响应
        std::cout << "Sending response: 200 OK" << std::endl;
        WriteResponse();
        return;
    }

    if (_request.method() == http::verb::post) {
        std::string request_path(_request.target());
        bool success = LogicSystem::GetInstance()->HandlePostRequest(request_path, shared_from_this());
        if (!success) {
            _response.result(http::status::not_found);
            _response.set(http::field::content_type, "text/plain");
            beast::ostream(_response.body()) << "url not found\r\n";
            WriteResponse();
            return;
        }

        _response.result(http::status::ok);
        _response.set(http::field::server, "GateServer");
        WriteResponse();
        return;
    }
    
    // 处理非GET请求
    _response.result(http::status::method_not_allowed);
    _response.set(http::field::content_type, "text/plain");
    beast::ostream(_response.body()) << "Method not allowed\r\n";
    // 记录方法不允许的错误
    std::cerr << "Error: Method not allowed - " << _request.method_string().data() << std::endl;
    WriteResponse();
}

void HttpConnection::WriteResponse() {
    auto self = shared_from_this();
    _response.content_length(_response.body().size());
    
    // 记录响应体内容
    std::cout << "Response body: " << beast::make_printable(_response.body().data()) << std::endl;
    
    http::async_write(
        _socket,
        _response,
        [self](beast::error_code ec, std::size_t)
        {
            if (ec) {
                // 记录写入错误
                std::cerr << "Error writing response: " << ec.message() << std::endl;
            }
            
            // 记录响应发送完成
            std::cout << "Response sent to client" << std::endl;
            
            self->_socket.shutdown(tcp::socket::shutdown_send, ec);
            self->deadline_.cancel();
        });
}

void HttpConnection::CheckDeadline() {
    auto self = shared_from_this();

    deadline_.async_wait(
        [self](beast::error_code ec)
        {
            if (!ec)
            {
                // Close socket to cancel any outstanding operation.
                self->_socket.close(ec);
            }
        });
}

void HttpConnection::ParseGetParameters() {
    _get_params.clear();
    auto uri = _request.target();
    std::cout << "Raw URI: " << uri << std::endl;

    auto query_pos = uri.find('?');
    if (query_pos == std::string::npos) {
        _get_url = uri;
        std::cout << "No query parameters found, path: " << _get_url << std::endl;
        return;
    }

    _get_url = uri.substr(0, query_pos);
    std::string query_string = uri.substr(query_pos + 1);
    std::cout << "Base URL: " << _get_url << std::endl;
    std::cout << "Query string: " << query_string << std::endl;

    // 使用 stringstream 正确解析
    std::stringstream ss(query_string);
    std::string pair;
    while (std::getline(ss, pair, '&')) {
        size_t eq_pos = pair.find('=');
        if (eq_pos != std::string::npos) {
            std::string key = UrlDecode(pair.substr(0, eq_pos));
            std::string value = UrlDecode(pair.substr(eq_pos + 1));
            _get_params[key] = value;
            std::cout << "Parsed parameter: " << key << " = " << value << std::endl;
        } else {
            // 处理没有值的参数（如 ?key&value=123 中的 key）
            std::string key = UrlDecode(pair);
            _get_params[key] = "";
            std::cout << "Parsed parameter (no value): " << key << " = " << "" << std::endl;
        }
    }

    std::cout << "Total parameters: " << _get_params.size() << std::endl;
    for (const auto& param : _get_params) {
        std::cout << "Final parameter: " << param.first << " = " << param.second << std::endl;
    }
}