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

namespace
{
static auto tcpFactory = IConnectionFactory::CreateFactory( ConnectionType::Tcp );
static auto udpFactory = IConnectionFactory::CreateFactory( ConnectionType::Udp );
static auto rawFactory = IConnectionFactory::CreateFactory( ConnectionType::Raw );
};

class TcpConnectionFactory final : public IConnectionFactory
{
private:
     TcpConnectionFactory() = delete;
public:
     TcpConnectionFactory( const std::string& name ) : IConnectionFactory( name ) {};
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
private:
     UdpConnectionFactory() = delete;
public:
     UdpConnectionFactory( const std::string& name ) : IConnectionFactory( name ) {};
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
private:
     RawConnectionFactory() = delete;
public:
     RawConnectionFactory( const std::string& name ) : IConnectionFactory( name ) {};
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

std::shared_ptr<IConnectionFactory> IConnectionFactory::CreateFactory( ConnectionType type )
{
     switch( type )
     {
          case ConnectionType::Tcp: return std::make_shared<TcpConnectionFactory>( SINGLETON_TCP );
          case ConnectionType::Udp: return std::make_shared<UdpConnectionFactory>( SINGLETON_UDP );
          case ConnectionType::Raw: return std::make_shared<RawConnectionFactory>( SINGLETON_RAW );
          default:
          {
               throw std::logic_error( "NOT IMPLEMENTED" );
          }
     }
}

} // namespace net_connection_lib
