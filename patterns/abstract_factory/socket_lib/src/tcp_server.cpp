#include "tcp_server.h"

#include <ctime>
#include <cstring>
#include <arpa/inet.h>
#include <memory>
#include <sys/socket.h>
#include <netinet/in.h>
#include <system_error>
#include <unistd.h>
#include <vector>
#include <functional>
#include <zlib.h>

#include "../i_connection_params.h"

namespace net_connection_lib
{

namespace
{

class RAII final
{
public:
     RAII( std::function<void()> foo = [](){} ) noexcept
     try { foo_ = foo; }
     catch( ... ) {}

     ~RAII() noexcept
     try { foo_(); }
     catch( ... ) {}
private:
     std::function<void()> foo_;
};

}

TcpServer::TcpServer( std::unique_ptr<IProtocol>& protocol, std::unique_ptr<IConnectionParam>& connectionParam )
: IServer( protocol, connectionParam )
{
     std::memset( &address_, 0, sizeof( address_ ) );
}

TcpServer::~TcpServer()
{
}

std::error_code TcpServer::Select( SocketId id )
{
     if( !IsListen() && openConnections_.end() != openConnections_.find( id ) )
     {
          return std::make_error_code( std::errc::bad_file_descriptor );
     }

     struct timeval tv{ 1, 0 };
     fd_set readSet;

     FD_ZERO( &readSet );
     FD_SET( openConnections_[id], &readSet);

     {
          while( !stop_ )
          {
               const int rv = select( openConnections_[id] + 1, &readSet, 0, 0, &tv );
               switch( rv )
               {
                    case -1:  return { errno, std::system_category() };
                    case 0:   continue;
                    default:  break;
               }

               break;
          }
     }

     if( stop_ )
     {
          return std::make_error_code( std::errc::operation_canceled );
     }

     return {};
}

std::error_code TcpServer::Listen( const std::string& ip, uint16_t port )
{
     hasActiveOperation_ = true;
     RAII raii( [this](){ hasActiveOperation_ = false; } );

     if( -1 == ( openConnections_[0] = socket( AF_INET, SOCK_STREAM, 0 ) ) )
     {
          return { errno, std::system_category() };
     }
#ifdef DEBUG
     printf( "%s[%u]: Open main socket: %d\n", __FILE__, __LINE__, openConnections_[0] );
#endif // DEBUG
     address_.sin_family = AF_INET;
     address_.sin_addr.s_addr = INADDR_ANY;
     address_.sin_port = htons( port );

     if( -1 == bind( openConnections_[0], reinterpret_cast<struct sockaddr*>( &address_ ), sizeof( address_ ) ) )
     {
          return { errno, std::system_category() };
     }

#ifdef DEBUG
     printf( "%s[%u]: Listen on socket: %d, ip: %s, port: %u\n", __FILE__, __LINE__, openConnections_[0], ip.c_str(), port );
#endif // DEBUG
     if( -1 == listen( openConnections_[0], connectionParam_->GetMaxConnection() ) )
     {
          return { errno, std::system_category() };
     }

     return {};
}

std::error_code TcpServer::Accept( SocketId& id )
{
     hasActiveOperation_ = true;
     RAII raii( [this](){ hasActiveOperation_ = false; } );

#ifdef DEBUG
     printf( "%s[%u]: Wait connection on socket: %d\n", __FILE__, __LINE__, openConnections_[0] );
#endif // DEBUG
     if( auto ec = Select( 0 ) )
     {
          return ec;
     }
#ifdef DEBUG
     printf( "%s[%u]: Connection trying detected\n", __FILE__, __LINE__ );
#endif // DEBUG
     socklen_t addrLen = sizeof( address_ );
     int newSocket = -1;
     if( -1 == ( newSocket = accept( openConnections_[0],
          reinterpret_cast<struct sockaddr*>( &address_ ), reinterpret_cast<socklen_t*>( &addrLen ) ) ) )
     {
          return { errno, std::system_category() };
     }
#ifdef DEBUG
     printf( "%s[%u]: Accepted connection\n", __FILE__, __LINE__ );
#endif // DEBUG
     std::time_t timestamp = std::time( nullptr );

     std::vector<uint8_t> seed( sizeof( newSocket ) + sizeof( timestamp) );
     memcpy( seed.data(), &newSocket, sizeof( newSocket ) );
     memcpy( seed.data() + sizeof( newSocket ), &timestamp, sizeof( timestamp ) );

     uint32_t crc = crc32( 0L, Z_NULL, 0 );
     crc = crc32( crc, seed.data(), seed.size() );
     if( 0 == crc ) // главный сокет с id = 0, вряд ли сюда попадем, но стоит предусмотреть
     {
          return std::make_error_code( std::errc::file_exists );
     }

     {
          openConnections_[crc] = newSocket;
     }

     id = crc;
#ifdef DEBUG
     printf( "%s[%u]: New connection socket: %d (id = %u)\n", __FILE__, __LINE__, newSocket, id );
#endif // DEBUG
     return {};
}

std::error_code TcpServer::Read( SocketId id, std::vector<uint8_t>& buff )
{
     hasActiveOperation_ = true;
     RAII raii( [this](){ hasActiveOperation_ = false; } );

     if( !IsListen() && openConnections_.end() != openConnections_.find( id ) )
     {
          return std::make_error_code( std::errc::bad_file_descriptor );
     }

#ifdef DEBUG
     printf( "%s[%u]: Wait data to read on socket %d (id = %u)\n", __FILE__, __LINE__, openConnections_[id], id );
#endif // DEBUG

     if( auto ec = Select( id ) )
     {
          return ec;
     }
     struct sockaddr_in srcAddress;
     socklen_t addrLen = sizeof( srcAddress );

     std::vector<uint8_t> header( protocol_->GetHeaderSize() );
#ifdef DEBUG
     printf( "%s[%u]: Has data to read. Try to receive data from socket %d\n", __FILE__, __LINE__, openConnections_[id] );
#endif // DEBUG
     if( -1 == recvfrom( openConnections_[ id ],
          header.data(), header.size(), 0, reinterpret_cast<struct sockaddr*>( &srcAddress ), &addrLen ) )
     {
          return { errno, std::system_category() };
     }
#ifdef DEBUG
     printf( "%s[%u]: Header was read, size: %lu\n", __FILE__, __LINE__, header.size() );
#endif // DEBUG
     size_t pldSize = 0;
     if( auto ec = protocol_->GetPayloadSize( header, pldSize ) )
     {
          return ec;
     }
#ifdef DEBUG
     printf( "%s[%u]: Payload size: %lu\n", __FILE__, __LINE__, pldSize );
#endif // DEBUG

     if( auto ec = Select( id ) )
     {
          return ec;
     }
#ifdef DEBUG
     printf( "%s[%u]: Has data to read after header. Try to receive data from socket %d\n",
                                                                           __FILE__, __LINE__, openConnections_[id] );
#endif // DEBUG
     std::vector<uint8_t> payload( pldSize );
     if( -1 == recvfrom( openConnections_[ id ],
          payload.data(), payload.size(), 0, reinterpret_cast<struct sockaddr*>( &srcAddress ), &addrLen ) )
     {
          return { errno, std::system_category() };
     }
#ifdef DEBUG
     printf( "%s[%u]: Payload was read\n", __FILE__, __LINE__ );
#endif // DEBUG

     buff.clear();
     buff.insert( buff.end(), header.begin(), header.end() );
     buff.insert( buff.end(), payload.begin(), payload.end() );

     return {};
}

std::error_code TcpServer::Write( SocketId id, const std::vector<uint8_t>& buff )
{
     hasActiveOperation_ = true;
     RAII raii( [this](){ hasActiveOperation_ = false; } );

#ifdef DEBUG
     printf( "%s[%u]: Try to write data, size: %lu\n", __FILE__, __LINE__, buff.size() );
#endif // DEBUG
     if( !IsListen() && openConnections_.end() != openConnections_.find( id ) )
     {
          return std::make_error_code( std::errc::bad_file_descriptor );
     }

     std::vector<uint8_t> dataToSend( protocol_->GetHeaderSize() + buff.size() );
     const auto payloadSize = buff.size();
     std::memcpy( dataToSend.data(), &payloadSize, sizeof( payloadSize ) );
     std::copy( buff.begin(), buff.end(), dataToSend.begin() + protocol_->GetHeaderSize() );
#ifdef DEBUG
     printf( "%s[%u]: Data size after header insert: %lu\n", __FILE__, __LINE__, dataToSend.size() );
#endif // DEBUG
     if( auto ec = ValidateData( dataToSend ) )
     {
          return ec;
     }

     socklen_t addrLen = sizeof( address_ );
     if( -1 == sendto( openConnections_[ id ],
          dataToSend.data(), dataToSend.size(), 0, reinterpret_cast<const struct sockaddr*>( &address_ ), addrLen ) )
     {
          return { errno, std::system_category() };
     }
#ifdef DEBUG
     printf( "%s[%u]: Data was send\n", __FILE__, __LINE__ );
#endif // DEBUG
     return {};
}

std::error_code TcpServer::ValidateData( const std::vector<uint8_t>& data )
{
#ifdef DEBUG
     printf( "%s[%u]: Packet to validate (size = %lu):\n", __FILE__, __LINE__, data.size() );
     for( const auto& it : data )
     {
          printf( "0x%x ", static_cast<int>( it ) );
     }
     printf( "\n" );
#endif // DEBUG
     size_t expectedDataSize = 0;
     if( auto ec = protocol_->GetPayloadSize( { data.data(), data.data() + protocol_->GetHeaderSize() }, expectedDataSize ) )
     {
          return ec;
     }
     expectedDataSize += protocol_->GetHeaderSize();
#ifdef DEBUG
     printf( "%s[%u]: Expected packet size: %lu\n", __FILE__, __LINE__, expectedDataSize );
#endif // DEBUG

     if( data.size() != expectedDataSize )
     {
          return std::make_error_code( std::errc::bad_message );
     }

     return {};
}

} // net_connection_lib
