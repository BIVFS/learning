#pragma once

#ifndef UDP_CLIENT_LIB_H__
#define UDP_CLIENT_LIB_H__

#include "../i_client.h"

#include <system_error>
#include <string>
#include <vector>

namespace net_connection_lib
{

class UdpClient final : public IClient
{
public:
     UdpClient();
     ~UdpClient();

     virtual std::error_code Connect( const std::string& ip, uint16_t port ) override;

     virtual std::error_code Read( std::vector<uint8_t>& buff ) override;

     virtual std::error_code Write( const std::vector<uint8_t>& buff ) override;

private:
     UdpClient( const UdpClient& ) = delete;
     UdpClient( UdpClient&& ) = delete;
     UdpClient& operator=( const UdpClient& ) = delete;
     UdpClient& operator=( UdpClient&& ) = delete;
};

} // namespace net_connection_lib

#endif // UDP_CLIENT_LIB_H__
