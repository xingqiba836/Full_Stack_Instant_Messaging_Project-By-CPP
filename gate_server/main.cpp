#include <iostream>
#include <sstream>
#include "json/json.h"

int main()
{
    Json::Value root;
    root["id"] = 1001;
    root["data"] = "hello world";
    std::string request = root.toStyledString();
    std::cout << "request is " << request << std::endl;

    Json::Value root2;
    Json::CharReaderBuilder builder;
    std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
    std::string errors;

    if (reader->parse(request.c_str(),
                      request.c_str() + request.size(),
                      &root2, &errors)) {
        std::cout << "msg id is " << root2["id"].asInt()
                  << " msg is " << root2["data"].asString() << std::endl;
                      } else {
                          std::cerr << "Parse error: " << errors << std::endl;
                          return 1;
                      }

    return 0;
}
