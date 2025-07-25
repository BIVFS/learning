#pragma once

#ifndef I_CLIENT_LIB_H__
#define I_CLIENT_LIB_H__

#include <system_error>
#include <vector>
#include <atomic>
#include <memory>
#include <unistd.h>

#include "i_protocol.h"
#include "i_connection_params.h"

namespace net_connection_lib
{

class IClient
{
public:
     virtual ~IClient()
     {
          Stop();
     }

     inline void Stop()
     {
          stop_ = true;
          sleep( 1 );
          close( socket_ );
          socket_ = -1;
     };

     virtual std::error_code Connect() = 0;

     virtual std::error_code Read( std::vector<uint8_t>& buff ) = 0;

     virtual std::error_code Write( const std::vector<uint8_t>& buff ) = 0;

private:
     IClient() = delete;
     IClient( const IClient& ) = delete;
     IClient( IClient& ) = delete;
     IClient& operator=( const IClient& ) = delete;
     IClient& operator=( IClient&& ) = delete;

protected:
     IClient( std::unique_ptr<IProtocol>& protocol, std::unique_ptr<IConnectionParam>& connectionParam )
          : socket_( -1 ), stop_( false ) { protocol_.swap( protocol ); connectionParam_.swap( connectionParam ); }

protected:
     int socket_;
     std::atomic_bool stop_;
     std::unique_ptr<IProtocol> protocol_;
     std::unique_ptr<IConnectionParam> connectionParam_;
};

} // namespace net_connection_lib

#endif // I_CLIENT_LIB_H__
