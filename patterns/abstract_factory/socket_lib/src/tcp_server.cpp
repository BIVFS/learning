#include "tcp_server.h"

namespace net_connection_lib
{

TcpServer::TcpServer()
{
}

TcpServer::~TcpServer()
{
}

std::error_code TcpServer::Listen( const std::string& ip, uint16_t port )
{
     return std::make_error_code( std::errc::not_supported );
}

std::error_code TcpServer::Accept( SocketId& id )
{
     return std::make_error_code( std::errc::not_supported );
}

std::error_code TcpServer::Read( SocketId id, std::vector<uint8_t>& buff )
{
     return std::make_error_code( std::errc::not_supported );
}

std::error_code TcpServer::Write( SocketId id, const std::vector<uint8_t>& buff )
{
     return std::make_error_code( std::errc::not_supported );
}

} // net_connection_lib
