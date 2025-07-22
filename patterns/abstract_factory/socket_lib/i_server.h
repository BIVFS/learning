#pragma once

#ifndef I_SERVER_LIB_H__
#define I_SERVER_LIB_H__

#include <system_error>
#include <map>
#include <string>
#include <vector>
#include <atomic>
#include <memory>
#include <unistd.h>

#include "i_protocol.h"
#include "i_connection_params.h"

namespace net_connection_lib
{

class IServer
{
public:
     using SocketId = uint32_t;
public:
     virtual ~IServer() { Stop(); };

     inline void Stop()
     {
          stop_ = true;
          sleep( 1 );
          for( auto& it : openConnections_ )
          {
               close( it.second );
               openConnections_.erase( it.first );
          }
          close( socket_ );
          socket_ = -1;
     };

     virtual std::error_code Listen( const std::string& ip, uint16_t port ) = 0;

     virtual std::error_code Accept( SocketId& id ) = 0;

     virtual std::error_code Read( SocketId id, std::vector<uint8_t>& buff ) = 0;

     virtual std::error_code Write( SocketId id, const std::vector<uint8_t>& buff ) = 0;

private:
     IServer( const IServer& ) = delete;
     IServer( IServer&& ) = delete;
     IServer& operator=( const IServer& ) = delete;
     IServer& operator=( IServer&& ) = delete;

protected:
     IServer( std::unique_ptr<IProtocol>& protocol, std::unique_ptr<IConnectionParam>& connectionParam )
          : socket_( -1 ), stop_( false ) { protocol_.swap( protocol ); connectionParam_.swap( connectionParam ); }

protected:
     int socket_;
     std::atomic_bool stop_;
     std::map<SocketId, int> openConnections_;

     std::unique_ptr<IProtocol> protocol_;
     std::unique_ptr<IConnectionParam> connectionParam_;
};

} // net_connection_lib

#endif // I_SERVER_LIB_H__
