#include <algorithm>
#include <iostream>
#include <memory>
#include <numeric>
#include <thread>
#include <vector>
#include <iomanip>
#include <atomic>

#include "../i_connection.h"
#include "../network_params.h"


int main()
{
     namespace ncl = ::net_connection_lib;
     auto connectionFactory = ncl::IConnectionFactory::CreateFactory( ncl::ConnectionType::Tcp );
     std::cout << typeid( connectionFactory.get() ).name() << std::endl;
     std::atomic_bool g_stop;
     g_stop = false;

     ncl::NetworkParams netParams;
     auto protocol = netParams.CreateProtocolPrototype( ncl::ProtocolType::Simple );
     auto connectParams = netParams.CreateConnectionParamsPrototype( ncl::ConnectionParamSet::Default );

     auto server = connectionFactory->CreateServer( protocol, connectParams );
     if( !server )
     {
          std::cerr << "Server: Invalid server ptr" << std::endl;
          return EXIT_FAILURE;
     }
     std::thread serverThrd( [&g_stop, &server] ()
          {
               std::cout << "Server: start listen" << std::endl;
               if( auto ec = server->Listen( "127.0.0.1", 5555 ) )
               {
                    std::cerr << ec.message() << std::endl;
                    return;
               }
               std::cout << "Server: Listen\n";

               while( !g_stop )
               {
                    ncl::IServer::SocketId id = 0;
                    if( auto ec = server->Accept( id ) )
                    {
                         std::cerr << "Server: " << ec.message() << std::endl;
                         return;
                    }
                    std::cout << "Server: Accept\n";
                    std::vector<uint8_t> buff( 10 );
                    if( auto ec = server->Read( id, buff ) )
                    {
                         std::cerr << "Server: " << ec.message() << std::endl;
                         return;
                    }
                    std::cout << "Server: Read\n";

                    std::cout << "Server receive: ";
                    for( const auto& it : buff )
                    {
                         std::cout << std::setw( 5 ) << "0x" << static_cast<int>( it );
                    }
                    std::cout << std::endl;

                    std::vector<uint8_t> sendBuff( 10 );
                    std::iota( sendBuff.begin(), sendBuff.end(), 11 );

                    if( auto ec = server->Write( id, sendBuff ) )
                    {
                         std::cerr << "Server: " << ec.message() << std::endl;
                         return;
                    }
                    std::cout << "Server: Write\n";
               }
               std::cout << "End server" << std::endl;
          }
     );

     sleep( 1 );

     std::shared_ptr<ncl::IClient> client1;
     std::thread clientThrd1( [&netParams,&client1,&connectionFactory,&g_stop] ()
          {
               auto protocolClient = netParams.CreateProtocolPrototype( ncl::ProtocolType::Simple );
               auto connectParamsClient = netParams.CreateConnectionParamsPrototype( ncl::ConnectionParamSet::Default );
               while( !g_stop )
               {
                    client1 = connectionFactory->CreateClient( protocolClient, connectParamsClient );
                    if( !client1 )
                    {
                         std::cerr << "Client 1: Invalid client 1 ptr" << std::endl;
                         return;
                    }

                    if( auto ec = client1->Connect( "127.0.0.1", 5555 ) )
                    {
                         std::cerr << "Client 1:" << ec.message() << std::endl;
                         return;
                    }
                    std::cout << "Client 1 connect\n";
                    std::vector<uint8_t> buff( 10 );
                    std::iota( buff.begin(), buff.end(), 1 );
                    if( auto ec = client1->Write( buff ) )
                    {
                         std::cerr << "Client 1:" << ec.message() << std::endl;
                         return;
                    }
                    std::cout << "Client 1 Write\n";
                    if( auto ec = client1->Read( buff ) )
                    {
                         std::cerr << "Client 1:" << ec.message() << std::endl;
                         return;
                    }
                    std::cout << "Client 1 Read\n";
                    std::cout << "Client 1 receive: ";
                    for( const auto& it : buff )
                    {
                         std::cout << std::setw( 5 ) << "0x" << static_cast<int>( it );
                    }
                    std::cout << std::endl;

                    std::cout << "End client 1" << std::endl;
               }
          }
     );

#if 0
     std::shared_ptr<ncl::IClient> client2;
     std::thread clientThrd2( [&netParams,&client2,&connectionFactory,&g_stop] ()
          {
               auto protocolClient = netParams.CreateProtocolPrototype( ncl::ProtocolType::Simple );
               auto connectParamsClient = netParams.CreateConnectionParamsPrototype( ncl::ConnectionParamSet::Default );
               while( !g_stop )
               {
                    client2 = connectionFactory->CreateClient( protocolClient, connectParamsClient );
                    if( !client2 )
                    {
                         std::cerr << "Client 2: Invalid client 2 ptr" << std::endl;
                         return;
                    }

                    if( auto ec = client2->Connect( "127.0.0.1", 5555 ) )
                    {
                         std::cerr << "Client 2:" << ec.message() << std::endl;
                         return;
                    }
                    std::cout << "Client 2 connect\n";
                    std::vector<uint8_t> buff( 10 );
                    std::iota( buff.begin(), buff.end(), 10 );
                    std::reverse( buff.begin(), buff.end() );
                    if( auto ec = client2->Write( buff ) )
                    {
                         std::cerr << "Client 2:" << ec.message() << std::endl;
                         return;
                    }
                    std::cout << "Client 2 Write\n";
                    if( auto ec = client2->Read( buff ) )
                    {
                         std::cerr << "Client 2:" << ec.message() << std::endl;
                         return;
                    }
                    std::cout << "Client 2 Read\n";
                    std::cout << "Client 2 receive: ";
                    for( const auto& it : buff )
                    {
                         std::cout << std::setw( 5 ) << "0x" << static_cast<int>( it );
                    }
                    std::cout << std::endl;

                    std::cout << "End client 2" << std::endl;
               }
          }
     );
#endif


     std::cout << "TODO1\n";

     int var = 0;
     std::cin >> var;
     std::cout << "TODO2\n";

     g_stop = true;

     server->Stop();
     client1->Stop();
//     client2->Stop();


     clientThrd1.join();
//     clientThrd2.join();
     serverThrd.join();
     std::cout << "TODO3\n";


     return EXIT_SUCCESS;
}
