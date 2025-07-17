#include "tcp_client.h"
#include <system_error>

namespace net_connection_lib
{

TcpClient::TcpClient()
{
}

TcpClient::~TcpClient()
{
}

std::error_code TcpClient::Connect( const std::string& ip, uint16_t port )
{
     return std::make_error_code( std::errc::not_supported );
}

std::error_code TcpClient::Read( std::vector<uint8_t>& buff )
{
     return std::make_error_code( std::errc::not_supported );
}

std::error_code TcpClient::Write( const std::vector<uint8_t>& buff )
{
     return std::make_error_code( std::errc::not_supported );
}

} // namespace net_connection_lib
