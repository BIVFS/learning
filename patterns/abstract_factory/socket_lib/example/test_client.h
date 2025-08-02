#pragma once

#ifndef TEST_CLIENT_H__
#define TEST_CLIENT_H__

#include <system_error>
#include <thread>
#include <atomic>
#include <memory>

#include "../i_connection.h"

namespace my_client
{

namespace ncl = ::net_connection_lib;

class Client final
{
public:
     explicit Client();
     ~Client();

     std::error_code Start() noexcept;
     void Stop() noexcept;
private:
     void ConnectProcess() noexcept;

private:
     std::atomic_bool stop_;
     std::shared_ptr<ncl::IClient> client_;
     std::thread process_;
};

} // namespace my_client

#endif // TEST_CLIENT_H__

