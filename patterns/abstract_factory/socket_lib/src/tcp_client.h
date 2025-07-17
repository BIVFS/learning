#pragma once

#ifndef TCP_CLIENT_LIB_H__
#define TCP_CLIENT_LIB_H__

#include "../i_client.h"

#include <system_error>
#include <string>
#include <vector>

namespace net_connection_lib
{

class TcpClient final : public IClient
{
public:
     TcpClient();
     ~TcpClient();

     virtual std::error_code Connect( const std::string& ip, uint16_t port ) override;

     virtual std::error_code Read( std::vector<uint8_t>& buff ) override;

     virtual std::error_code Write( const std::vector<uint8_t>& buff ) override;

private:
     TcpClient( const TcpClient& ) = delete;
     TcpClient( TcpClient&& ) = delete;
     TcpClient& operator=( const TcpClient& ) = delete;
     TcpClient& operator=( TcpClient&& ) = delete;
};

} // namespace net_connection_lib

#endif // TCP_CLIENT_LIB_H__
