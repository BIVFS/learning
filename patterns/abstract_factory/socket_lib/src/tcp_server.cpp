#include "tcp_server.h"

#include <ctime>
#include <cstring>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <system_error>
#include <unistd.h>
#include <vector>
#include <zlib.h>

#include "../i_connection_params.h"

namespace net_connection_lib
{

constexpr int MAX_CLIENTS = 10; //TODO

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

     if( stop_ )
     {
          return std::make_error_code( std::errc::operation_canceled );
     }

     return {};
}

std::error_code TcpServer::Listen( const std::string& ip, uint16_t port )
{
     if( -1 == ( socket_ = socket( AF_INET, SOCK_STREAM, 0 ) ) )
     {
          return { errno, std::system_category() };
     }
     address_.sin_family = AF_INET;
     address_.sin_addr.s_addr = INADDR_ANY;
     address_.sin_port = htons( port );

     if( -1 == bind( socket_, reinterpret_cast<struct sockaddr*>( &address_ ), sizeof( address_ ) ) )
     {
          return { errno, std::system_category() };
     }

     if( -1 == listen( socket_, connectionParam_->GetMaxConnection() ) )
     {
          return { errno, std::system_category() };
     }

     return {};
}

std::error_code TcpServer::Accept( SocketId& id )
{
     struct timeval tv{ 1, 0 };

     while( !stop_ )
     {
          fd_set readSet;

          FD_ZERO( &readSet );
          FD_SET( socket_, &readSet);
          const int rv = select( socket_ + 1, &readSet, 0, 0, &tv );
          switch( rv )
          {
               case -1:  return { errno, std::system_category() };
               case 0:   continue;
               default:  break;
          }

          break;
     }

     if( stop_ )
     {
          return std::make_error_code( std::errc::operation_canceled );
     }

     socklen_t addrLen = sizeof( address_ );
     int newSocket = -1;
     if( -1 == ( newSocket = accept( socket_,
          reinterpret_cast<struct sockaddr*>( &address_ ), reinterpret_cast<socklen_t*>( &addrLen ) ) ) )
     {
          return { errno, std::system_category() };
     }

     std::time_t timestamp = std::time( nullptr );

     std::vector<uint8_t> seed( sizeof( newSocket ) + sizeof( timestamp) );
     memcpy( seed.data(), &newSocket, sizeof( newSocket ) );
     memcpy( seed.data() + sizeof( newSocket ), &timestamp, sizeof( timestamp ) );

     uint32_t crc = crc32( 0L, Z_NULL, 0 );
     crc = crc32( crc, seed.data(), seed.size() );
     openConnections_[crc] = newSocket;

     id = crc;

     return {};
}

std::error_code TcpServer::Read( SocketId id, std::vector<uint8_t>& buff )
{
     if( !IsListen() && openConnections_.end() != openConnections_.find( id ) )
     {
          return std::make_error_code( std::errc::bad_file_descriptor );
     }

     if( auto ec = Select( id ) )
     {
          return ec;
     }

     struct sockaddr_in srcAddress;
     socklen_t addrLen = sizeof( srcAddress );

     std::vector<uint8_t> header( protocol_->GetHeaderSize() );
     if( -1 == recvfrom( openConnections_[ id ],
          header.data(), header.size(), 0, reinterpret_cast<struct sockaddr*>( &srcAddress ), &addrLen ) )
     {
          return { errno, std::system_category() };
     }

     size_t pldSize = 0;
     if( auto ec = protocol_->GetPayloadSize( header, pldSize ) )
     {
          return ec;
     }

     if( auto ec = Select( id ) )
     {
          return ec;
     }

     std::vector<uint8_t> payload( pldSize );
     if( -1 == recvfrom( openConnections_[ id ],
          payload.data(), payload.size(), 0, reinterpret_cast<struct sockaddr*>( &srcAddress ), &addrLen ) )
     {
          return { errno, std::system_category() };
     }

     buff.clear();
     buff.insert( buff.end(), header.begin(), header.end() );
     buff.insert( buff.end(), payload.begin(), payload.end() );

     return {};
}

std::error_code TcpServer::Write( SocketId id, const std::vector<uint8_t>& buff )
{
     if( !IsListen() && openConnections_.end() != openConnections_.find( id ) )
     {
          return std::make_error_code( std::errc::bad_file_descriptor );
     }

     std::vector<uint8_t> dataToSend( protocol_->GetHeaderSize() + buff.size() );
     const auto headerSize = protocol_->GetHeaderSize();
     std::memcpy( dataToSend.data(), &headerSize, sizeof( headerSize ) );
     std::copy( buff.begin(), buff.end(), dataToSend.end() );

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

     return {};
}

std::error_code TcpServer::ValidateData( const std::vector<uint8_t>& data )
{
     size_t expectedDataSize = 0;
     if( auto ec = protocol_->GetPayloadSize( data, expectedDataSize ) )
     {
          return ec;
     }
     expectedDataSize += protocol_->GetHeaderSize();

     if( data.size() != expectedDataSize )
     {
          return std::make_error_code( std::errc::bad_message );
     }

     return {};
}

} // net_connection_lib
