#include "tcp_client.h"
#include <algorithm>
#include <cstring>
#include <iterator>
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

     if( auto ec = Select() )
     {
          return ec;
     }

     struct sockaddr_in srcAddress;
     socklen_t addrLen = sizeof( srcAddress );

     std::vector<uint8_t> header( protocol_->GetHeaderSize() );
     if( -1 == recvfrom(
          socket_, header.data(), header.size(), 0, reinterpret_cast<struct sockaddr*>( &srcAddress ), &addrLen ) )
     {
          return { errno, std::system_category() };
     }

     size_t pldSize = 0;
     if( auto ec = protocol_->GetPayloadSize( header, pldSize ) )
     {
          return ec;
     }

     if( auto ec = Select() )
     {
          return ec;
     }

     std::vector<uint8_t> payload( pldSize );
     if( -1 == recvfrom(
          socket_, payload.data(), payload.size(), 0, reinterpret_cast<struct sockaddr*>( &srcAddress ), &addrLen ) )
     {
          return { errno, std::system_category() };
     }

     buff.clear();
     buff.insert( buff.end(), header.begin(), header.end() );
     buff.insert( buff.end(), payload.begin(), payload.end() );

     return {};
}

std::error_code TcpClient::Write( const std::vector<uint8_t>& buff )
{
     if( !IsEstablished() )
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
     if( -1 == sendto(
          socket_, dataToSend.data(), dataToSend.size(), 0, reinterpret_cast<const struct sockaddr*>( &address_ ), addrLen ) )
     {
          return { errno, std::system_category() };
     }

     return {};
}

std::error_code TcpClient::ValidateData( const std::vector<uint8_t>& data )
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

} // namespace net_connection_lib
