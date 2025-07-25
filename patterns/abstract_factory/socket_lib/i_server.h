#pragma once

#ifndef I_SERVER_LIB_H__
#define I_SERVER_LIB_H__

#include <system_error>
#include <map>
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

     virtual inline void Stop() noexcept
     try
     {
          stop_ = true;

          for( auto& it : openConnections_ )
          {
               close( it.second );
          }
          openConnections_.clear();
     }
     catch( ... )
     {
     }

     virtual std::error_code Listen() noexcept = 0;

     virtual std::error_code Accept( SocketId& id ) noexcept = 0;

     virtual std::error_code Close( const SocketId& id ) noexcept = 0;

     virtual std::error_code Read( const SocketId id, std::vector<uint8_t>& buff ) noexcept = 0;

     virtual std::error_code Write( const SocketId id, const std::vector<uint8_t>& buff ) noexcept = 0;

private:
     IServer( const IServer& ) = delete;
     IServer( IServer&& ) = delete;
     IServer& operator=( const IServer& ) = delete;
     IServer& operator=( IServer&& ) = delete;

protected:
     IServer( std::unique_ptr<IProtocol>& protocol, std::unique_ptr<IConnectionParam>& connectionParam )
          : stop_( false ) { openConnections_[0] = -1; protocol_.swap( protocol ); connectionParam_.swap( connectionParam ); }

protected:
     std::atomic_bool stop_;
     std::map<SocketId, int> openConnections_;

     std::unique_ptr<IProtocol> protocol_;
     std::unique_ptr<IConnectionParam> connectionParam_;
};

} // net_connection_lib

#endif // I_SERVER_LIB_H__
