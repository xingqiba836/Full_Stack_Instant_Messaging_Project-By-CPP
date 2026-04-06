// urlcode.cpp
#include "urlcode.h"
#include <cctype>
#include <sstream>
#include <iomanip>

unsigned char ToHex(unsigned char x) {
    return x > 9 ? x + 55 : x + 48;
}

unsigned char FromHex(unsigned char x) {
    unsigned char y;
    if (x >= 'A' && x <= 'Z')
        y = x - 'A' + 10;
    else if (x >= 'a' && x <= 'z')
        y = x - 'a' + 10;
    else if (x >= '0' && x <= '9')
        y = x - '0';
    else
        y = 0;
    return y;
}

std::string UrlEncode(const std::string& str) {
    std::string result;
    for (unsigned char c : str) {
        if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
            // 不需要编码的字符
            result += c;
        } else if (c == ' ') {
            // 空格编码为 %20 或 +（这里使用 %20）
            result += "%20";
        } else {
            // 其他字符编码为 %XX
            result += '%';
            result += ToHex(c >> 4);
            result += ToHex(c & 0x0F);
        }
    }
    return result;
}

std::string UrlDecode(const std::string& str) {
    std::string result;
    size_t i = 0;
    while (i < str.length()) {
        if (str[i] == '%' && i + 2 < str.length()) {
            // 解码 %XX
            unsigned char high = FromHex(str[i + 1]);
            unsigned char low = FromHex(str[i + 2]);
            result += (high << 4) | low;
            i += 3;
        } else if (str[i] == '+') {
            // 将 + 解码为空格
            result += ' ';
            i++;
        } else {
            result += str[i];
            i++;
        }
    }
    return result;
}