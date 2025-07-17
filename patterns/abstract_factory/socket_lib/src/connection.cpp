#include "../i_connection.h"

#include "tcp_client.h"
#include "tcp_server.h"

#include "udp_client.h"
#include "udp_server.h"

#include "raw_client.h"
#include "raw_server.h"

#include <memory>
#include <stdexcept>

namespace net_connection_lib
{

class TcpConnectionFactory final : public IConnectionFactory
{
public:
     TcpConnectionFactory() = default;

     virtual std::shared_ptr<IClient> CreateClient() override
     {
          return std::shared_ptr<TcpClient>();
     }

     virtual std::shared_ptr<IServer> CreateServer() override
     {
          return std::shared_ptr<TcpServer>();
     }
};

class UdpConnectionFactory final : public IConnectionFactory
{
public:
     UdpConnectionFactory() = default;

     virtual std::shared_ptr<IClient> CreateClient() override
     {
          return std::shared_ptr<UdpClient>();
     }

     virtual std::shared_ptr<IServer> CreateServer() override
     {
          return std::shared_ptr<UdpServer>();
     }
};

class RawConnectionFactory final : public IConnectionFactory
{
public:
     RawConnectionFactory() = default;

     virtual std::shared_ptr<IClient> CreateClient() override
     {
          return std::shared_ptr<RawClient>();
     }

     virtual std::shared_ptr<IServer> CreateServer() override
     {
          return std::shared_ptr<RawServer>();
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
