#ifndef ASIO_IO_SERVICE_POOL_H
#define ASIO_IO_SERVICE_POOL_H

#include <boost/asio.hpp>
#include <memory>
#include <thread>
#include <vector>
#include "singleton.h"

class AsioIOServicePool : public Singleton<AsioIOServicePool>
{
    friend class Singleton<AsioIOServicePool>;

public:
    using IOService = boost::asio::io_context;
    using Work = boost::asio::executor_work_guard<IOService::executor_type>;
    using WorkPtr = std::unique_ptr<Work>;

    ~AsioIOServicePool();
    AsioIOServicePool(const AsioIOServicePool &) = delete;
    AsioIOServicePool &operator=(const AsioIOServicePool &) = delete;

    boost::asio::io_context &GetIOService();
    void Stop();

private:
    AsioIOServicePool(std::size_t size = std::thread::hardware_concurrency());
    std::vector<IOService> _io_services;
    std::vector<WorkPtr> _works;
    std::vector<std::thread> _threads;
    std::size_t _next_io_service;
};

#endif // ASIO_IO_SERVICE_POOL_H
