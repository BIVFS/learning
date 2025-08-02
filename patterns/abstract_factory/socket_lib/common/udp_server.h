#pragma once

#ifndef UDP_SERVER_LIB_H__
#define UDP_SERVER_LIB_H__

#include "../i_server.h"

#include <system_error>
#include <vector>
#include <map>

#include "../i_connection_params.h"

namespace net_connection_lib
{

class UdpServer final : public IServer
{
public:
     UdpServer( std::unique_ptr<IProtocol>& protocol, std::unique_ptr<IConnectionParam>& connectionParam );
     ~UdpServer();

     virtual std::error_code Listen() noexcept override;

     virtual std::error_code Accept( SocketId& id ) noexcept override;

     virtual std::error_code Close( const SocketId& id ) noexcept override;

     virtual std::error_code Read( const SocketId id, std::vector<uint8_t>& buff ) noexcept override;

     virtual std::error_code Write( const SocketId id, const std::vector<uint8_t>& buff ) noexcept override;

private:
     std::map<SocketId, int> openConnections_;

private:
     UdpServer( const UdpServer& ) = delete;
     UdpServer( UdpServer&& ) = delete;
     UdpServer& operator=( const UdpServer& ) = delete;
     UdpServer& operator=( UdpServer&& ) = delete;
};

} // net_connection_lib

#endif // UDP_SERVER_LIB_H__
