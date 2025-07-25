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

     std::error_code Start() noexcept;
     void Stop() noexcept;
private:
     void AcceptProcess() noexcept;
     void ClientProcess( ncl::IServer::SocketId id ) noexcept;
private:
     std::atomic_bool stop_;
     std::shared_ptr<ncl::IServer> server_;
     std::thread acceptProcess_;
     std::map<ncl::IServer::SocketId,std::unique_ptr<std::thread> > clientProcess_;
};

} // namespace my_server

#endif // TEST_SERVER_H__
