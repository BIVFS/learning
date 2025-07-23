#pragma once

#ifndef TCP_SERVER_LIB_H__
#define TCP_SERVER_LIB_H__

#include "../i_server.h"

#include <system_error>
#include <vector>
#include <map>

#include <netinet/in.h>

#include "../i_connection_params.h"

namespace net_connection_lib
{

class TcpServer final : public IServer
{
public:
     explicit TcpServer( std::unique_ptr<IProtocol>& protocol, std::unique_ptr<IConnectionParam>& connectionParam );
     ~TcpServer();

     virtual std::error_code Listen( const std::string& ip, uint16_t port ) override;

     virtual std::error_code Accept( SocketId& id ) override;

     virtual std::error_code Read( SocketId id, std::vector<uint8_t>& buff ) override;

     virtual std::error_code Write( SocketId id, const std::vector<uint8_t>& buff ) override;

private:
     inline bool IsListen() const
          { return ( -1 != openConnections_.at( 0 ) ); }

     std::error_code Select( SocketId id );

     std::error_code ValidateData( const std::vector<uint8_t>& data );

private:
     TcpServer( const TcpServer& ) = delete;
     TcpServer( TcpServer&& ) = delete;
     TcpServer& operator=( const TcpServer& ) = delete;
     TcpServer& operator=( TcpServer&& ) = delete;

private:
     struct sockaddr_in address_;
};

} // net_connection_lib

#endif // TCP_SERVER_LIB_H__
