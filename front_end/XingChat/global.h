#ifndef GLOBAL_H
#define GLOBAL_H

#include <QString>

enum ReqId {
    ID_GET_VARIFY_CODE = 1001, // 获取验证码
    ID_REG_USER = 1002,        // 注册用户
};

enum ErrorCodes {
    SUCCESS = 0,
    ERR_JSON = 1,   // Json解析失败
    ERR_NETWORK = 2,
    // 与 gate_server 返回的 JSON error 字段一致
    ERR_SERVER_JSON = 1001,
    ERR_EMAIL_SERVICE = 1002,
    ERR_VERIFY_EXPIRED = 1003,
    ERR_VERIFY_CODE = 1004,
    ERR_USER_EXIST = 1005,
};

enum Modules {
    REGISTERMOD = 0,
};

extern QString gate_url_prefix;

#endif // GLOBAL_H
