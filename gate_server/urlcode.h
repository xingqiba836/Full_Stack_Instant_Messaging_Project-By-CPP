// urlcode.h
#ifndef URLCODE_H
#define URLCODE_H

#include <string>

unsigned char ToHex(unsigned char x);
unsigned char FromHex(unsigned char x);
std::string UrlEncode(const std::string& str);
std::string UrlDecode(const std::string& str);

#endif // URLCODE_H