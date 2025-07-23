#include "tcp_client.h"
#include <algorithm>
#include <cstring>
#include <iostream>
#include <system_error>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <vector>

#include "../i_connection_params.h"

namespace net_connection_lib
{

TcpClient::TcpClient( std::unique_ptr<IProtocol>& protocol, std::unique_ptr<IConnectionParam>& connectionParam )
: IClient( protocol, connectionParam )
{
     std::memset( &address_, 0, sizeof( address_ ) );
}

TcpClient::~TcpClient()
{
}

std::error_code TcpClient::Connect( const std::string& ip, uint16_t port )
{
     if( -1 == ( socket_ = socket( AF_INET, SOCK_STREAM, 0 ) ) )
     {
          return { errno, std::system_category() };
     }
#ifdef DEBUG
     printf( "%s[%u]: Open socket: %d\n", __FILE__, __LINE__, socket_ );
#endif // DEBUG
     address_.sin_family = AF_INET;
     address_.sin_port = htons( port );

     if( -1 == inet_pton( AF_INET, ip.c_str(), &address_.sin_addr ) )
     {
          return { errno, std::system_category() };
     }

     if( 0 != connect( socket_, reinterpret_cast<struct sockaddr*>( &address_ ), sizeof( address_ ) ) )
     {
          return { errno, std::system_category() };
     }
#ifdef DEBUG
     printf( "%s[%u]: Connected by socket: %d, ip: %s, port: %u\n", __FILE__, __LINE__, socket_, ip.c_str(), port );
#endif // DEBUG

     return {};
}

std::error_code TcpClient::Select()
{
     struct timeval tv{ 1, 0 };
     fd_set readSet;

     FD_ZERO( &readSet );
     FD_SET( socket_, &readSet);

     while( !stop_ )
     {
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

     return {};
}

std::error_code TcpClient::Read( std::vector<uint8_t>& buff )
{
     if( !IsEstablished() )
     {
          return std::make_error_code( std::errc::bad_file_descriptor );
     }
#ifdef DEBUG
     printf( "%s[%u]: Wait data to read\n", __FILE__, __LINE__ );
#endif // DEBUG
     if( auto ec = Select() )
     {
          return ec;
     }
#ifdef DEBUG
     printf( "%s[%u]: Has data to read\n", __FILE__, __LINE__ );
#endif // DEBUG
     struct sockaddr_in srcAddress;
     socklen_t addrLen = sizeof( srcAddress );

     std::vector<uint8_t> header( protocol_->GetHeaderSize() );
     if( -1 == recvfrom(
          socket_, header.data(), header.size(), 0, reinterpret_cast<struct sockaddr*>( &srcAddress ), &addrLen ) )
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
     if( auto ec = Select() )
     {
          return ec;
     }
#ifdef DEBUG
     printf( "%s[%u]: Has data to read after header\n", __FILE__, __LINE__ );
#endif // DEBUG
     std::vector<uint8_t> payload( pldSize );
     if( -1 == recvfrom(
          socket_, payload.data(), payload.size(), 0, reinterpret_cast<struct sockaddr*>( &srcAddress ), &addrLen ) )
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

std::error_code TcpClient::Write( const std::vector<uint8_t>& buff )
{
#ifdef DEBUG
     printf( "%s[%u]: Try to write data, size: %lu\n", __FILE__, __LINE__, buff.size() );
#endif // DEBUG
     if( !IsEstablished() )
     {
          return std::make_error_code( std::errc::bad_file_descriptor );
     }

     const size_t hSize = protocol_->GetHeaderSize();
     std::vector<uint8_t> dataToSend( hSize + buff.size() );
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
     if( -1 == sendto(
          socket_, dataToSend.data(), dataToSend.size(), 0, reinterpret_cast<const struct sockaddr*>( &address_ ), addrLen ) )
     {
          return { errno, std::system_category() };
     }
#ifdef DEBUG
     printf( "%s[%u]: Data was send\n", __FILE__, __LINE__ );
#endif // DEBUG
     return {};
}

std::error_code TcpClient::ValidateData( const std::vector<uint8_t>& data )
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
#if DEBUG
     printf( "%s[%u]: Expected packet size: %lu\n", __FILE__, __LINE__, expectedDataSize );
#endif // DEBUG

     if( data.size() != expectedDataSize )
     {
          return std::make_error_code( std::errc::bad_message );
     }

     return {};
}

} // namespace net_connection_lib
