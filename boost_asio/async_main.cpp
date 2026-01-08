#include <chrono>
#include <iostream>
#include <algorithm>
#include <thread>
#include <vector>
#include <csignal>

#include <boost/asio/buffer.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/address_v4.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/placeholders.hpp>
#include <boost/asio/registered_buffer.hpp>
#include <boost/smart_ptr/make_shared_array.hpp>
#include <boost/smart_ptr/make_shared_object.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/system/detail/error_code.hpp>
#include <boost/bind.hpp>
#include <boost/thread/futures/future_error.hpp>

constexpr auto LOCAL_ADDR     = "127.0.0.1"; // Локальный адрес
constexpr uint16_t LOCAL_PORT = 55333; // Порт для соединения


std::string g_mode;
boost::shared_ptr<boost::asio::io_service> g_ioSrv;
boost::shared_ptr<boost::asio::ip::tcp::endpoint> g_ep;

namespace my_server
{

boost::shared_ptr<boost::asio::ip::tcp::acceptor> g_acceptor; // указатель на объект принимающий подключения от клиентов
boost::shared_ptr<std::vector<uint8_t>> g_server_r_buff; // указатель на буфер для чтения на сервере

/// @brief Запуск приема подключений клиентов
/// @param[in] psock указатель на сокет, к которому будет выполнено подключение
void StartAccept( boost::shared_ptr<boost::asio::ip::tcp::socket> psock );

/// @brief Обработчик подключения клиента
/// @param[in] psock указатель на сокет, к которому подключился клиент
/// @param[in] ec код ошибки операции принятия подключения клиента
void ServerAcceptor( boost::shared_ptr<boost::asio::ip::tcp::socket> psock, const boost::system::error_code& ec );

/// @brief Обработчик чтения данных от клиента
/// @param[in] psock указатель на сокет, с которого были прочитаны данные
/// @param[in] ec код ошибки чтения данных с сокета подключенного клиента
/// @param[in] nbytes количество прочитанных байт
void ServerReadHandler( boost::shared_ptr<boost::asio::ip::tcp::socket> psock, const boost::system::error_code& ec, const std::size_t& nbytes );

/// @brief Обработчик записи данных в сокет
/// @param[in] ec код ошибки записи
/// @param[in] nbytes количество записанных байт
void ServerWriteHandler( const boost::system::error_code& ec, std::size_t nbytes );

} // namespace my_server

namespace my_client
{

boost::shared_ptr<std::vector<uint8_t>> g_client_r_buff; // указатель на буффер для чтения на клиенте

/// @brief Обработчик подключения к серверу
/// @param[in] psock указатель на сокет, который подключен к серверу
/// @param[in] ec код ошибки подключения
void ConnectHandler( boost::shared_ptr<boost::asio::ip::tcp::socket> psock, const boost::system::error_code& ec );

/// @brief Обработчик чтения данных от сервера
/// @param[in] ec код ошибки чтения
/// @param[in] nbytes количество считанных байт
void ClientReadHandler( const boost::system::error_code& ec, std::size_t nbytes );

/// @brief Обработчик отправки данных на сервер
/// @param[in] psock указатель на подключенный к серверу сокет
/// @param[in] ec код ошибки отправки
/// @param[in] nbytes количество отправленных байт
void ClientWriteHandler( boost::shared_ptr<boost::asio::ip::tcp::socket> psock, const boost::system::error_code& ec, std::size_t nbytes );

} // namespace my_client

bool ValidateAndApplyOpts( const int argc, const char* const argv[] )
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

     g_mode.swap( mode );

     return true;
}

namespace my_server
{

void StartAccept( boost::shared_ptr<boost::asio::ip::tcp::socket> psock )
{
     g_acceptor->async_accept( *psock, boost::bind( ServerAcceptor, psock, boost::placeholders::_1 ) );
}

void ServerAcceptor( boost::shared_ptr<boost::asio::ip::tcp::socket> psock, const boost::system::error_code& ec )
{
     if( ec )
     {
          std::cerr << "Error while accept: " <<ec.what() << std::endl;
          return;
     }

     boost::make_shared<std::vector<uint8_t>>( 10, 0 ).swap( g_server_r_buff );
     psock->async_receive( boost::asio::buffer( *g_server_r_buff ), boost::asio::ip::tcp::socket::message_end_of_record,
          boost::bind( ServerReadHandler, psock, boost::placeholders::_1, boost::placeholders::_2 ) );

     boost::shared_ptr<boost::asio::ip::tcp::socket> newPsock = boost::make_shared<boost::asio::ip::tcp::socket>( *g_ioSrv );
     StartAccept( newPsock );
}

void ServerReadHandler( boost::shared_ptr<boost::asio::ip::tcp::socket> psock, const boost::system::error_code& ec, const std::size_t& nbytes )
{
     if( ec )
     {
          std::cerr << "Error while SERVER read: " << ec.what() << std::endl;
          return;
     }

     std::cout << "SERVER receive: " << nbytes << " bytes" << std::endl;
     std::string msg( g_server_r_buff->data(), g_server_r_buff->data() + g_server_r_buff->size() );
     std::cout << "SERVER received message: " << msg << std::endl;

     psock->async_send( boost::asio::buffer( "9876556789" ), boost::asio::ip::tcp::socket::message_end_of_record, ServerWriteHandler );
}

void ServerWriteHandler( const boost::system::error_code& ec, std::size_t nbytes )
{
     if( ec )
     {
          std::cerr << "Error while SERVER send: " << ec.what() << std::endl;
          return;
     }

     std::cout << "SERVER transfer: " << nbytes << " bytes" << std::endl;
}

} // namespace my_server

namespace my_client
{

void ConnectHandler( boost::shared_ptr<boost::asio::ip::tcp::socket> psock, const boost::system::error_code& ec )
{
     if( ec )
     {
          std::cerr << "Error while connect: " <<ec.what() << std::endl;
          return;
     }

     std::string msg( "1234567891" );
     psock->async_send( boost::asio::buffer( msg.data(), 10 ), boost::asio::ip::tcp::socket::message_end_of_record,
          boost::bind( ClientWriteHandler, psock, boost::placeholders::_1, boost::placeholders::_2 ) );
}

void ClientReadHandler( boost::shared_ptr<boost::asio::ip::tcp::socket> psock, const boost::system::error_code& ec, std::size_t nbytes )
{
     if( ec )
     {
          std::cerr << "Error while CLIENT read: " <<ec.what() << std::endl;
          return;
     }

     std::cout << "CLIENT receive: " << nbytes << " bytes" << std::endl;

     std::string msg( g_client_r_buff->data(), g_client_r_buff->data() + g_client_r_buff->size() );
     std::cout << "CLIENT receive message: " << msg << std::endl;

     boost::system::error_code error;
     psock->close( error );

     std::this_thread::sleep_for( std::chrono::milliseconds( 10 ) );
     if( error )
     {
          std::cerr << "Error while close CLIENT" << std::endl;
          return;
     }

     psock->async_connect( *g_ep, boost::bind( my_client::ConnectHandler, psock, boost::placeholders::_1  ) );
}

void ClientWriteHandler( boost::shared_ptr<boost::asio::ip::tcp::socket> psock, const boost::system::error_code& ec, std::size_t nbytes )
{
     if( ec )
     {
          std::cerr << "Error while CLIENT send: " <<ec.what() << std::endl;
          return;
     }

     std::cout << "CLIENT transferer: " << nbytes << " bytes" << std::endl;

     boost::make_shared<std::vector<uint8_t>>( 10, 0 ).swap( g_client_r_buff );
     psock->async_receive( boost::asio::buffer( *g_client_r_buff ), boost::asio::ip::tcp::socket::message_end_of_record,
          boost::bind( ClientReadHandler, psock, boost::placeholders::_1, boost::placeholders::_2 ) );
}

} // namespace my_client


void SigHandler( int signo )
{
     switch( signo )
     {
          case SIGINT:
          case SIGTERM:
          {
               std::cout << "Interrupted" << std::endl;
               g_ioSrv->stop();
               break;
          }
          default:
          {
               break;
          }
     }
}

bool RegisterSigHandler( int signo )
{
     struct sigaction act;
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

     sigfillset( &( act.sa_mask ) );
     act.sa_handler = SigHandler;

     return 0 == sigaction( signo, &act, nullptr );
}

int main( int argc, char* argv[] )
{
     if( !ValidateAndApplyOpts( argc, argv ) )
     {
          std::cerr << "Invalid options" << std::endl;
          return EXIT_FAILURE;
     }

     if( !RegisterSigHandler( SIGINT ) || !RegisterSigHandler( SIGTERM ) )
     {
          std::cerr << "Failed to register signal handler" << std::endl;
          return EXIT_FAILURE;
     }

     g_ioSrv = boost::make_shared<boost::asio::io_service>();
     g_ep = boost::make_shared<boost::asio::ip::tcp::endpoint>( boost::asio::ip::make_address_v4( LOCAL_ADDR ), LOCAL_PORT );

     if( 0 == g_mode.compare( "server" ) )
     {
          boost::shared_ptr<boost::asio::ip::tcp::socket> psock = boost::make_shared<boost::asio::ip::tcp::socket>( *g_ioSrv );
          my_server::g_acceptor = boost::make_shared<boost::asio::ip::tcp::acceptor>( *g_ioSrv, *g_ep );
          my_server::StartAccept( psock );
          g_ioSrv->run();
     }
     else if( 0 == g_mode.compare( "client" ) )
     {
          boost::shared_ptr<boost::asio::ip::tcp::socket> psock = boost::make_shared<boost::asio::ip::tcp::socket>( *g_ioSrv );
          psock->async_connect( *g_ep, boost::bind( my_client::ConnectHandler, psock, boost::placeholders::_1  ) );
          g_ioSrv->run();
     }

     std::cout << "EXIT" << std::endl;

     return EXIT_SUCCESS;
}
