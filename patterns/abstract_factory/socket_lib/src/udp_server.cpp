#include "udp_server.h"

#include "../i_connection_params.h"

namespace net_connection_lib
{

UdpServer::UdpServer( std::unique_ptr<IProtocol>& protocol, std::unique_ptr<IConnectionParam>& connectionParam )
: IServer( protocol, connectionParam )
{
}

UdpServer::~UdpServer()
{
}

std::error_code UdpServer::Listen( const std::string& ip, uint16_t port )
{
     return std::make_error_code( std::errc::not_supported );
}

std::error_code UdpServer::Accept( SocketId& id )
{
     return std::make_error_code( std::errc::not_supported );
}

std::error_code UdpServer::Read( SocketId id, std::vector<uint8_t>& buff )
{
     return std::make_error_code( std::errc::not_supported );
}

std::error_code UdpServer::Write( SocketId id, const std::vector<uint8_t>& buff )
{
     return std::make_error_code( std::errc::not_supported );
}

} // net_connection_lib
