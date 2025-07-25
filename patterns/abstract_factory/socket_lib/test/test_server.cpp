#include "test_server.h"

#include <iostream>
#include <iomanip>
#include <memory>
#include <numeric>

#include "../i_connection.h"
#include "../network_params.h"

namespace my_server
{

Server::Server()
: stop_( false )
{
     namespace ncl = ::net_connection_lib;
     auto connectionFactory = ncl::IConnectionFactory::CreateFactory( ncl::ConnectionType::Tcp );

     ncl::NetworkParams netParams;
     auto protocol = netParams.CreateProtocolPrototype( ncl::ProtocolType::Simple );
     auto connectParams = netParams.CreateConnectionParamsPrototype( ncl::ConnectionParamSet::Default );

     server_ = connectionFactory->CreateServer( protocol, connectParams );
     if( !server_ )
     {
          throw std::runtime_error( "Failed to create server" );
     }
}

Server::~Server()
try
{
     Stop();
}
catch( ... )
{
}

std::error_code Server::Start() noexcept
try
{
     if( auto ec = server_->Listen() )
     {
          return ec;
     }

     acceptProcess_ = std::thread( &Server::AcceptProcess, this );

     return {};
}
catch( ... )
{
     return std::make_error_code( std::errc::not_enough_memory );
}

void Server::Stop() noexcept
try
{
     if( server_ )
     {
          server_->Stop();
     }
     stop_ = true;

     if( acceptProcess_.joinable() )
     {
          acceptProcess_.join();
     }

     for( auto& it : clientProcess_ )
     {
          if( it.second && it.second->joinable() )
          {
               it.second->join();
          }
     }
}
catch( ... )
{
     std::cerr << __FILE__ << ":" << __LINE__ << "-" << "Catch exception" << std::endl;
}

void Server::AcceptProcess() noexcept
try
{
     while( !stop_ )
     {
          ncl::IServer::SocketId id = 0;
          if( auto ec = server_->Accept( id ) )
          {
               std::cerr << "Server: " << ec.message() << std::endl;
               return;
          }

          clientProcess_[id] = std::make_unique<std::thread>( &Server::ClientProcess, this, id );
     }
}
catch( ... )
{
     std::cerr << __FILE__ << ":" << __LINE__ << "-" << "Catch exception" << std::endl;
}

void Server::ClientProcess( ncl::IServer::SocketId id ) noexcept
try
{
     while( !stop_ )
     {
          std::vector<uint8_t> buff( 10 );
          if( auto ec = server_->Read( id, buff ) )
          {
               std::cerr << "Server: " << ec.message() << std::endl;
               break;
          }

          std::cout << "Server receive: ";
          for( const auto& it : buff )
          {
               std::cout << std::setw( 5 ) << "0x" << static_cast<int>( it );
          }
          std::cout << std::endl;

          std::vector<uint8_t> sendBuff( 10 );
          std::iota( sendBuff.begin(), sendBuff.end(), 11 );

          if( auto ec = server_->Write( id, sendBuff ) )
          {
               std::cerr << "Server: " << ec.message() << std::endl;
               break;
          }
     }
     server_->Close( id );
}
catch( ... )
{
     std::cerr << __FILE__ << ":" << __LINE__ << "-" << "Catch exception" << std::endl;
}

} // namespace my_server
