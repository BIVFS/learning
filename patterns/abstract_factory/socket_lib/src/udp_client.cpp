#include "udp_client.h"
#include <system_error>

namespace net_connection_lib
{

UdpClient::UdpClient()
{
}

UdpClient::~UdpClient()
{
}

std::error_code UdpClient::Connect( const std::string& ip, uint16_t port )
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
