#include "test_client.h"

#include <iostream>
#include <iomanip>
#include <numeric>

#include "../i_connection.h"
#include "../network_params.h"


namespace my_client
{

Client::Client()
: stop_( false )
{
     namespace ncl = ::net_connection_lib;
     //auto connectionFactory = ncl::IConnectionFactory::CreateFactory( ncl::ConnectionType::Tcp );
     auto factory = ncl::Singleton::Instance( ncl::SINGLETON_TCP );
     auto connectionFactory = dynamic_cast<ncl::IConnectionFactory*>( factory.get() );

     ncl::NetworkParams netParams;
     auto protocol = netParams.CreateProtocolPrototype( ncl::ProtocolType::Simple );
     auto connectParams = netParams.CreateConnectionParamsPrototype( ncl::ConnectionParamSet::Default );

     client_ = connectionFactory->CreateClient( protocol, connectParams );
     if( !client_ )
     {
          throw std::runtime_error( "Failed to create client" );
     }
}

Client::~Client()
try
{
     Stop();
}
catch( ... )
{
}

std::error_code Client::Start() noexcept
try
{
     process_ = std::thread( &Client::ConnectProcess, this );

     return {};
}
catch( ... )
{
     return std::make_error_code( std::errc::not_enough_memory );
}

void Client::Stop() noexcept
try
{
     if( client_ )
     {
          client_->Stop();
     }
     stop_ = true;

     if( process_.joinable() )
     {
          process_.join();
     }
}
catch( ... )
{
     std::cerr << __FILE__ << ":" << __LINE__ << "-" << "Catch exception" << std::endl;
}

void Client::ConnectProcess() noexcept
try
{
     if( auto ec = client_->Connect() )
     {
          std::cerr << "Client: " << ec.message() << std::endl;
          return;
     }
     while( !stop_ )
     {
          std::vector<uint8_t> buff( 10 );
          std::iota( buff.begin(), buff.end(), 1 );
          if( auto ec = client_->Write( buff ) )
          {
               std::cerr << "Client:" << ec.message() << std::endl;
               return;
          }
          if( auto ec = client_->Read( buff ) )
          {
               std::cerr << "Client:" << ec.message() << std::endl;
               return;
          }
          std::cout << "Client receive: ";
          for( const auto& it : buff )
          {
               std::cout << std::setw( 5 ) << "0x" << static_cast<int>( it );
          }
          std::cout << std::endl;
     }
}
catch( ... )
{
     std::cerr << __FILE__ << ":" << __LINE__ << "-" << "Catch exception" << std::endl;
}

} // namespace my_client
