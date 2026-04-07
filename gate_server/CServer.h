#include <boost/asio.hpp>
#include <memory>
#include "const.h"
#include "HttpConnection.h"

class HttpServer : public std::enable_shared_from_this<HttpServer>
{
public:
    HttpServer(boost::asio::io_context &io_context, unsigned short &port);
    void StartAcceptingConnections();

private:
    tcp::acceptor _acceptor;
};