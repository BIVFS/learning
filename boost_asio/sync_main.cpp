#include <algorithm>
#include <boost/asio/buffer.hpp>
#include <boost/asio/deadline_timer.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/address_v4.hpp>
#include <boost/chrono/duration.hpp>
#include <boost/date_time/posix_time/posix_time_duration.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/system/detail/errc.hpp>
#include <boost/system/detail/error_code.hpp>
#include <boost/thread.hpp>
#include <cctype>
#include <iostream>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <list>
#include <string>
#include <vector>
#include <utility>
#include <boost/lexical_cast.hpp>

constexpr auto LOCAL_ADDR     = "127.0.0.1"; // Локальный адрес
constexpr uint16_t LOCAL_PORT = 55333; // Порт для соединения

std::atomic_bool g_stop( false );

std::list<std::pair<size_t, boost::thread>> g_thrds;
std::mutex g_thrd_mtx;

bool ValidateOpts( const int argc, const char* const argv[] )
{
     if( 2 > argc )
     {
          std::cerr << "Not enough options" << std::endl;
          return false;
     }

     std::string mode( argv[1] );
     std::transform( mode.begin(), mode.end(), mode.begin(), []( int ch ) { return static_cast<char> ( std::tolower( ch ) ); } );

     if( 0 != mode.compare( "client" ) && 0 != mode.compare( "server" ) )
     {
          std::cerr << "Unexpected mode: " << mode << std::endl;
          std::cerr << "You should use \"server\" or \"client\"" << std::endl;
          return false;
     }

     return true;
}

size_t ThreadIdToInt( boost::thread::id& id )
try
{
     std::string idStr = boost::lexical_cast<std::string>( id );
     // If the output is hex (e.g., "0x1234"), you may need stoul with base 16
     return std::stoul( idStr, nullptr, 16 );
}
catch( ... )
{
     return 0;
}

void ThreadHandler( boost::shared_ptr<boost::asio::ip::tcp::socket> psock )
{
     boost::system::error_code ec;
     std::vector<char> buff( 10, 0 );

     while( !g_stop )
     {
          if( 0 >= psock->available() )
          {
               boost::this_thread::sleep_for( boost::chrono::milliseconds( 10 ) );
               continue;
          }
          else
          {
               break;
          }
     }

     boost::asio::socket_base::message_flags flags = boost::asio::socket_base::message_end_of_record;
     size_t nbytes = psock->receive( boost::asio::buffer( buff ) , flags, ec );
     switch( ec.value() )
     {
          case boost::system::errc::success:
          {
               break;
          }
          case boost::system::errc::broken_pipe:
          {
               psock->close( ec );
               if( ec )
               {
                    std::cout << "Close failed" << std::endl;
               }
               return;
          }
          default:
          {
               std::cerr << "Receive failed: " << ec.what() << std::endl;
               break;
          }
     }

     std::cout << "Receive " << nbytes << " bytes" << std::endl;
     std::cout << "Receive: " << std::string(  buff.data(), buff.data() + nbytes ) << std::endl;

     psock->send( boost::asio::buffer( "1234567899" ), flags, ec );
     switch( ec.value() )
     {
          case boost::system::errc::success:
          {
               break;
          }
          case boost::system::errc::broken_pipe:
          {
               psock->close( ec );
               if( ec )
               {
                    std::cout << "Close failed" << std::endl;
               }
               return;
          }
          default:
          {
               std::cerr << "Send failed: " << ec.what() << std::endl;
               break;
          }
     }

     std::cout << "Send 1234567899" << std::endl;

     psock->close( ec );
     if( ec )
     {
          std::cout << "Close failed" << std::endl;
     }

     auto thrdId = boost::this_thread::get_id();
     size_t id = ThreadIdToInt( thrdId );
     std::cout << "Thread " << id << " shutdown" << std::endl;

     std::lock_guard<std::mutex> lock( g_thrd_mtx );
     for( auto& it : g_thrds )
     {
          if( id == it.first )
          {
               it.first = 0;
          }
     }
}

void SigHandler( int signo )
{
     switch( signo )
     {
          case SIGTERM:
          case SIGINT:
          {
               g_stop = true;
               std::cout << "Stopped\n";
               break;
          }
          default:
          {
               break;
          }
     }
}

bool RegSigHandler( int signo )
{
     struct sigaction act;
     sigfillset( &act.sa_mask );
     switch( signo )
     {
          case SIGINT:
          case SIGTERM:
          {
               act.sa_flags = SA_NOCLDSTOP;
               break;
          }
          default:
          {
               act.sa_flags = SA_NOCLDSTOP | SA_RESTART;
               break;
          }
     }
     act.sa_handler = SigHandler;

     return 0 == sigaction( signo, &act, nullptr );
}

int main( int argc, char* argv[] )
{
     if( !ValidateOpts( argc, argv ) )
     {
          return EXIT_FAILURE;
     }

     if( !RegSigHandler( SIGINT ) )
     {
          std::cerr << "Failed to register SIGINT" << std::endl;
          return EXIT_FAILURE;
     }

     boost::asio::io_service ioSrv;
     boost::asio::ip::tcp::endpoint ep( boost::asio::ip::make_address_v4( LOCAL_ADDR ), LOCAL_PORT );

     std::string mode( argv[1] );
     std::transform( mode.begin(), mode.end(), mode.begin(), []( int ch ) { return static_cast<char> ( std::tolower( ch ) ); } );

     boost::asio::deadline_timer t( ioSrv, boost::posix_time::seconds( 60 ) );
     t.async_wait( []( const boost::system::error_code& ec ) { (void)ec; g_stop = true; std::cout << "Stopped\n"; } );

     if( 0 == mode.compare( "server" ) )
     {
          boost::asio::ip::tcp::acceptor acc( ioSrv, ep );

          std::list<boost::thread> thrds;
          size_t threadCnt = 0;
          while( !g_stop )
          {
               auto sockPtr = boost::make_shared<boost::asio::ip::tcp::socket>( ioSrv );
               boost::system::error_code ec;
               acc.accept( *sockPtr, ec );
               if( ec )
               {
                    std::cerr << "Accept failed: " << ec.what() << std::endl;
                    continue;
               }
               g_thrds.emplace_back( std::make_pair( threadCnt, boost::bind( ThreadHandler, sockPtr ) ) );

               auto it = --g_thrds.end();
               auto thrdId = it->second.get_id();

               std::cout << "Thread " << thrdId << " create" << std::endl;
               it->first = ThreadIdToInt( thrdId );

               {
                    std::lock_guard<std::mutex> lock( g_thrd_mtx );
                    for( decltype( g_thrds )::iterator it = g_thrds.begin(); it != g_thrds.end(); ++it )
                    {
                         if( 0 == it->first ) // если поток без ID, то он завершился
                         {
                              it->second.join();

                              if( it == g_thrds.begin() )
                              {
                                   g_thrds.erase( it );
                                   it = g_thrds.begin();
                              }
                              else
                              {
                                   auto newIt = --it;
                                   g_thrds.erase( it );
                                   it = newIt;
                              }
                         }
                    }
               }
          }

          for( auto& it : g_thrds )
          {
               it.second.join();
          }
     }
     else if( 0 == mode.compare( "client" ) )
     {
          boost::asio::ip::tcp::socket sock( ioSrv );
          while( !g_stop )
          {
               std::string input;

               std::cout << "Input data to send:" << std::endl;
               std::cin >> input;

               if( input.size() > 10 )
               {
                    std::cerr << "Too big input, maximum 10" << std::endl;
                    continue;
               }
               if( g_stop )
               {
                    std::cout << "Interrupted" << std::endl;
                    return EXIT_SUCCESS;
               }

               size_t padding = input.size() % 10;
               if( 0 != padding )
               {
                    input.insert( input.end(), 10 - padding, '0' );
               }

               boost::system::error_code ec;
               sock.connect( ep, ec );
               if( ec )
               {
                    std::cerr << "Connect failed: " << ec.what() << std::endl;
                    continue;
               }

               boost::asio::socket_base::message_flags flags = boost::asio::socket_base::message_end_of_record;
               sock.send( boost::asio::buffer( input ), flags, ec );
               if( ec )
               {
                    std::cerr << "Send failed: " << ec.what() << std::endl;
                    continue;
               }
               std::cout << "Send: \"" << input << "\"" << std::endl;

               std::vector<char> buff( 10, 0 );
               size_t nbyte = sock.receive( boost::asio::buffer( buff ) , flags, ec );
               if( ec )
               {
                    std::cerr << "Receive failed: " << ec.what() << std::endl;
                    continue;
               }

               std::string msg( buff.data(), buff.data() + nbyte );
               std::cout << "Received: " << msg << std::endl;

               sock.close( ec );
               if( ec )
               {
                    std::cout << "Close failed" << std::endl;
               }
          }
     }

     return EXIT_SUCCESS;
}
