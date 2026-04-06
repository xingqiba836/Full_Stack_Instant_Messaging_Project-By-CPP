#ifndef LOGICSYSTEM_H
#define LOGICSYSTEM_H

#include <memory>
#include <mutex>
#include <iostream>
#include <functional>
#include <map>
#include "const.h"
#include "singleton.h"

class HttpConnection;
class LogicSystem;

typedef std::function<void(std::shared_ptr<HttpConnection>)> HttpHandler;

class LogicSystem : public Singleton<LogicSystem>
{
    friend class Singleton<LogicSystem>;
    friend class HttpConnection;
public:
    ~LogicSystem();
    bool HandleGetRequest(std::string, std::shared_ptr<HttpConnection>);
    bool HandlePostRequest(std::string, std::shared_ptr<HttpConnection>);
    void RegisterGetHandler(std::string, HttpHandler handler);
    void RegisterPostHandler(std::string, HttpHandler handler);
private:
    LogicSystem();
    std::map<std::string, HttpHandler> _post_handlers;
    std::map<std::string, HttpHandler> _get_handlers;
};

#endif // LOGICSYSTEM_H