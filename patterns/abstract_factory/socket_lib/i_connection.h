#pragma once
#ifndef I_CONNECTION_LIB_H__
#define I_CONNECTION_LIB_H__

#include <memory>

#include "i_client.h"
#include "i_server.h"

#include "i_protocol.h"
#include "i_connection_params.h"

namespace net_connection_lib
{

enum class ConnectionType : uint8_t
{
     Tcp = 0,
     Udp,
     Raw
};

class IConnectionFactory
{
public:
     static std::unique_ptr<IConnectionFactory> CreateFactory( ConnectionType type );

     virtual ~IConnectionFactory() = default;

     virtual std::shared_ptr<IClient> CreateClient(
          std::unique_ptr<IProtocol>& protocol, std::unique_ptr<IConnectionParam>& param ) = 0;
     virtual std::shared_ptr<IServer> CreateServer(
          std::unique_ptr<IProtocol>& protocol, std::unique_ptr<IConnectionParam>& param ) = 0;

private:
     IConnectionFactory( const IConnectionFactory& ) = delete;
     IConnectionFactory( IConnectionFactory&& ) = delete;
     IConnectionFactory& operator=( const IConnectionFactory& ) = delete;
     IConnectionFactory& operator=( IConnectionFactory&& ) = delete;

protected:
     explicit IConnectionFactory() = default;
};

} // namespace net_connection_lib

#endif // I_CONNECTION_LIB_H__
