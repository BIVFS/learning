#include "raw_server.h"

namespace net_connection_lib
{

RawServer::RawServer()
{
}

RawServer::~RawServer()
{
}

std::error_code RawServer::Listen( const std::string& ip, uint16_t port )
{
     return std::make_error_code( std::errc::not_supported );
}

std::error_code RawServer::Accept( SocketId& id )
{
     return std::make_error_code( std::errc::not_supported );
}

std::error_code RawServer::Read( SocketId id, std::vector<uint8_t>& buff )
{
     return std::make_error_code( std::errc::not_supported );
}

std::error_code RawServer::Write( SocketId id, const std::vector<uint8_t>& buff )
{
     return std::make_error_code( std::errc::not_supported );
}

} // net_connection_lib

