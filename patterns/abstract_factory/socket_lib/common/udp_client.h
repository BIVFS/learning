#pragma once

#ifndef UDP_CLIENT_LIB_H__
#define UDP_CLIENT_LIB_H__

#include "../i_client.h"

#include <system_error>
#include <vector>

#include "../i_connection_params.h"

namespace net_connection_lib
{

class UdpClient final : public IClient
{
public:
     UdpClient( std::unique_ptr<IProtocol>& protocol, std::unique_ptr<IConnectionParam>& connectionParam );
     ~UdpClient();

     virtual std::error_code Connect() override;

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
