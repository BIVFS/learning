#pragma once
#ifndef I_CONNECTION_LIB_H__
#define I_CONNECTION_LIB_H__

#include <memory>

#include "i_client.h"
#include "i_server.h"

#include "i_protocol.h"
#include "i_connection_params.h"

#include "singleton//unique_factory.h"

namespace net_connection_lib
{

constexpr auto SINGLETON_TCP = "tcp_connection_factory";
constexpr auto SINGLETON_UDP = "udp_connection_factory";
constexpr auto SINGLETON_RAW = "raw_connection_factory";

enum class ConnectionType : uint8_t
{
     Tcp = 0,
     Udp,
     Raw
};

class IConnectionFactory : public Singleton
{
public:
     virtual ~IConnectionFactory() = default;

     virtual std::shared_ptr<IClient> CreateClient(
          std::unique_ptr<IProtocol>& protocol, std::unique_ptr<IConnectionParam>& param ) = 0;
     virtual std::shared_ptr<IServer> CreateServer(
          std::unique_ptr<IProtocol>& protocol, std::unique_ptr<IConnectionParam>& param ) = 0;

private:
     IConnectionFactory() = delete;
     IConnectionFactory( const IConnectionFactory& ) = delete;
     IConnectionFactory( IConnectionFactory&& ) = delete;
     IConnectionFactory& operator=( const IConnectionFactory& ) = delete;
     IConnectionFactory& operator=( IConnectionFactory&& ) = delete;

protected:
     explicit IConnectionFactory( const std::string& name ) : Singleton( name ) {};
};

} // namespace net_connection_lib

#endif // I_CONNECTION_LIB_H__
