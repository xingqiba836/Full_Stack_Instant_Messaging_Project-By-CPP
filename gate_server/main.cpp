#include <boost/beast/websocket.hpp>
#include "CServer.h"
#include "LogicSystem.h"
#include "HttpConnection.h"

int main()
{
    try
    {
        unsigned short port = static_cast<unsigned short>(8080);
        net::io_context ioc{ 1 };
        
        // 记录服务器启动
        std::cout << "Gate server starting on port 8080..." << std::endl;
        
        // 设置信号处理
        boost::asio::signal_set signals(ioc, SIGINT, SIGTERM);
        signals.async_wait([&ioc](const boost::system::error_code& error, int signal_number) {
            if (error) {
                return;
            }
            
            // 记录服务器停止
            std::cout << "Shutting down server..." << std::endl;
            ioc.stop();
            });
            
        // 创建并启动服务器
        std::make_shared<HttpServer>(ioc, port)->StartAcceptingConnections();
        std::cout << "Server is running..." << std::endl;
        
        // 运行IO上下文
        ioc.run();
    }
    catch (std::exception const& e)
    {
        // 记录致命错误
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
}
