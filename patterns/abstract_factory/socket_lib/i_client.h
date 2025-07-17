#pragma once

#ifndef I_CLIENT_LIB_H__
#define I_CLIENT_LIB_H__

#include <system_error>
#include <string>
#include <vector>
#include <atomic>

namespace net_connection_lib
{

class IClient
{
public:
     virtual ~IClient() { Stop(); };

     inline void Stop() { stop_ = true; };

     virtual std::error_code Connect( const std::string& ip, uint16_t port ) = 0;

     virtual std::error_code Read( std::vector<uint8_t>& buff ) = 0;

     virtual std::error_code Write( const std::vector<uint8_t>& buff ) = 0;

private:
     std::atomic_bool stop_;

private:
     IClient( const IClient& ) = delete;
     IClient( IClient& ) = delete;
     IClient& operator=( const IClient& ) = delete;
     IClient& operator=( IClient&& ) = delete;

protected:
     IClient() = default;
};

} // namespace net_connection_lib

#endif // I_CLIENT_LIB_H__
