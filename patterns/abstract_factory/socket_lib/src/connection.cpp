#include "../i_connection.h"

#include "tcp_client.h"
#include "tcp_server.h"

#include "udp_client.h"
#include "udp_server.h"

#include "raw_client.h"
#include "raw_server.h"

#include "../i_protocol.h"

#include <memory>
#include <stdexcept>

namespace net_connection_lib
{

class TcpConnectionFactory final : public IConnectionFactory
{
public:
     TcpConnectionFactory() = default;
     ~TcpConnectionFactory() = default;

     virtual std::shared_ptr<IClient> CreateClient(
          std::unique_ptr<IProtocol>& protocol, std::unique_ptr<IConnectionParam>& param ) override
     {
          return std::make_shared<TcpClient>( protocol, param );
     }

     virtual std::shared_ptr<IServer> CreateServer(
          std::unique_ptr<IProtocol>& protocol, std::unique_ptr<IConnectionParam>& param ) override
     {
          return std::make_shared<TcpServer>( protocol, param );
     }
};

class UdpConnectionFactory final : public IConnectionFactory
{
public:
     UdpConnectionFactory() = default;
     ~UdpConnectionFactory() = default;

     virtual std::shared_ptr<IClient> CreateClient(
          std::unique_ptr<IProtocol>& protocol, std::unique_ptr<IConnectionParam>& param ) override
     {
          return std::make_shared<UdpClient>( protocol, param );
     }

     virtual std::shared_ptr<IServer> CreateServer(
          std::unique_ptr<IProtocol>& protocol, std::unique_ptr<IConnectionParam>& param ) override
     {
          return std::make_shared<UdpServer>( protocol, param );
     }
};

class RawConnectionFactory final : public IConnectionFactory
{
public:
     RawConnectionFactory() = default;
     ~RawConnectionFactory() = default;

     virtual std::shared_ptr<IClient> CreateClient(
          std::unique_ptr<IProtocol>& protocol, std::unique_ptr<IConnectionParam>& param ) override
     {
          return std::make_shared<RawClient>( protocol, param );
     }

     virtual std::shared_ptr<IServer> CreateServer(
          std::unique_ptr<IProtocol>& protocol, std::unique_ptr<IConnectionParam>& param ) override
     {
          return std::make_shared<RawServer>( protocol, param );
     }
};

std::unique_ptr<IConnectionFactory> IConnectionFactory::CreateFactory( ConnectionType type )
{
     switch( type )
     {
          case ConnectionType::Tcp: return std::make_unique<TcpConnectionFactory>();
          case ConnectionType::Udp: return std::make_unique<UdpConnectionFactory>();
          case ConnectionType::Raw: return std::make_unique<RawConnectionFactory>();
          default:
          {
               throw std::logic_error( "NOT IMPLEMENTED" );
          }
     }
}

} // namespace net_connection_lib
