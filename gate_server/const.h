#ifndef CONST_H
#define CONST_H

#include <boost/beast/http.hpp>
#include <boost/beast.hpp>
#include <boost/asio.hpp>
#include <memory>
#include <iostream>
#include <functional>
#include <map>
#include <unordered_map>
#include <cassert>
#include <cctype>
#include <chrono>
#include <boost/system/error_code.hpp>
#include "json/json.h"

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

enum ErrorCodes {
    Success = 0,
    Error_Json = 1001,  // Json解析错误
    EmailServiceFailed = 1002,   // 邮件服务请求错误
    VarifyExpired = 1003,       // 验证码过期或不存在
    VarifyCodeErr = 1004,       // 验证码错误
    UserExist = 1005,           // 用户已存在
};

#endif // CONST_H