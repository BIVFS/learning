#pragma once

#ifndef TEST_SERVER_H__
#define TEST_SERVER_H__

#include <system_error>
#include <thread>
#include <atomic>
#include <memory>

#include "../i_connection.h"

namespace my_server
{

namespace ncl = ::net_connection_lib;

class Server final
{
public:
     explicit Server();
     ~Server();

     std::error_code Start( const std::string& ip, uint16_t port ) noexcept;
     void Stop() noexcept;
private:
     void AcceptProcess() noexcept;
private:
     std::atomic_bool stop_;
     std::shared_ptr<ncl::IServer> server_;
     std::thread acceptProcess_;
};

} // namespace my_server

#endif // TEST_SERVER_H__
