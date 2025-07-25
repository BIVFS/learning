#include "udp_client.h"
#include <system_error>

#include "../i_connection_params.h"

namespace net_connection_lib
{

UdpClient::UdpClient( std::unique_ptr<IProtocol>& protocol, std::unique_ptr<IConnectionParam>& connectionParam )
: IClient( protocol, connectionParam )
{
}

UdpClient::~UdpClient()
{
}

std::error_code UdpClient::Connect()
{
     return std::make_error_code( std::errc::not_supported );
}

std::error_code UdpClient::Read( std::vector<uint8_t>& buff )
{
     return std::make_error_code( std::errc::not_supported );
}

std::error_code UdpClient::Write( const std::vector<uint8_t>& buff )
{
     return std::make_error_code( std::errc::not_supported );
}

} // namespace net_connection_lib
