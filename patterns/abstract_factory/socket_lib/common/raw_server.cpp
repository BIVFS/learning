#include "raw_server.h"

#include "../i_connection_params.h"

namespace net_connection_lib
{

RawServer::RawServer( std::unique_ptr<IProtocol>& protocol, std::unique_ptr<IConnectionParam>& connectionParam )
: IServer( protocol, connectionParam )
{
}

RawServer::~RawServer()
{
}

std::error_code RawServer::Listen() noexcept
{
     return std::make_error_code( std::errc::not_supported );
}

std::error_code RawServer::Accept( SocketId& id ) noexcept
{
     return std::make_error_code( std::errc::not_supported );
}

std::error_code RawServer::Close( const SocketId& id ) noexcept
{
     return std::make_error_code( std::errc::not_supported );
}

std::error_code RawServer::Read( const SocketId id, std::vector<uint8_t>& buff ) noexcept
{
     return std::make_error_code( std::errc::not_supported );
}

std::error_code RawServer::Write( const SocketId id, const std::vector<uint8_t>& buff ) noexcept
{
     return std::make_error_code( std::errc::not_supported );
}

} // net_connection_lib
