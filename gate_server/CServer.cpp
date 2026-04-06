#include "CServer.h"

HttpServer::HttpServer(boost::asio::io_context &io_context, unsigned short &port)
    : _acceptor(io_context, tcp::endpoint(tcp::v4(), port)), _io_context(io_context), _socket(io_context)
{

}

void HttpServer::StartAcceptingConnections()
{
    auto self = shared_from_this();
    _acceptor.async_accept(_socket, [self](beast::error_code error_code) {
        try {
            //记录接受连接开始
            std::cout << "Accepting new connection" << std::endl;
            
            //出错则放弃这个连接，继续监听新链接
            if (error_code) {
                //记录错误
                std::cerr << "Error accepting connection: " << error_code.message() << std::endl;
                self->StartAcceptingConnections();
                return;
            }

            //创建并启动新连接
            auto new_connection = std::make_shared<HttpConnection>(std::move(self->_socket));
            std::cout << "New connection established" << std::endl;
            new_connection->StartReadingRequest();
            
            //继续监听
            self->StartAcceptingConnections();
        }
        catch (std::exception &exception) {
            //记录异常
            std::cerr << "Exception in connection handling: " << exception.what() << std::endl;
            self->StartAcceptingConnections();
        }
    });
}
