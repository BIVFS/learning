#pragma once

#include <atomic>
#ifndef TCP_SERVER_LIB_H__
#define TCP_SERVER_LIB_H__

#include "../i_server.h"

#include <system_error>
#include <vector>
#include <map>
#include <mutex>
#include <list>

#include <netinet/in.h>

#include "../i_connection_params.h"

namespace net_connection_lib
{

class TcpServer final : public IServer
{
public:
     explicit TcpServer( std::unique_ptr<IProtocol>& protocol, std::unique_ptr<IConnectionParam>& connectionParam );
     ~TcpServer();

     virtual void Stop() noexcept override;

     virtual std::error_code Listen() noexcept override;

     virtual std::error_code Accept( SocketId& id ) noexcept override;

     virtual std::error_code Close( const SocketId& id ) noexcept override;

     virtual std::error_code Read( const SocketId id, std::vector<uint8_t>& buff ) noexcept override;

     virtual std::error_code Write( const SocketId id, const std::vector<uint8_t>& buff ) noexcept override;

private:
     inline bool IsListen() const
          { return ( -1 != openConnections_.at( 0 ) ); }

     std::error_code GetSocketById( const SocketId id, int& fd );

     std::error_code CheckConnection( const SocketId id, bool storeLock );

     std::error_code Select( const int fd ) const;

     std::error_code ValidateData( const std::vector<uint8_t>& data ) const;

private:
     TcpServer( const TcpServer& ) = delete;
     TcpServer( TcpServer&& ) = delete;
     TcpServer& operator=( const TcpServer& ) = delete;
     TcpServer& operator=( TcpServer&& ) = delete;

private:
     struct sockaddr_in address_;

     std::mutex connectionMgmt_;
};

} // net_connection_lib

#endif // TCP_SERVER_LIB_H__
