#include "AsioIOServicePool.h"
#include <iostream>

AsioIOServicePool::AsioIOServicePool(std::size_t size)
    : _io_services(size), _works(size), _next_io_service(0)
{
    for (std::size_t i = 0; i < size; ++i) {
        _works[i] = std::make_unique<Work>(_io_services[i].get_executor());
    }

    for (std::size_t i = 0; i < _io_services.size(); ++i) {
        _threads.emplace_back([this, i]() {
            _io_services[i].run();
        });
    }
}

AsioIOServicePool::~AsioIOServicePool()
{
    Stop();
    std::cout << "AsioIOServicePool destruct" << std::endl;
}

boost::asio::io_context &AsioIOServicePool::GetIOService()
{
    auto &service = _io_services[_next_io_service++];
    if (_next_io_service == _io_services.size()) {
        _next_io_service = 0;
    }
    return service;
}

void AsioIOServicePool::Stop()
{
    for (auto &work : _works) {
        if (work) {
            work->get_executor().context().stop();
            work.reset();
        }
    }

    for (auto &thread : _threads) {
        if (thread.joinable()) {
            thread.join();
        }
    }
}
