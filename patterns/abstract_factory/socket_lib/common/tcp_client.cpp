#include "tcp_client.h"

#include <algorithm>
#include <cstring>
#include <iostream>
#include <system_error>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <fcntl.h>

#include <unistd.h>
#include <vector>

#include "../i_connection_params.h"

namespace net_connection_lib
{

namespace
{

/// @brief Выставление блокирующего/неблокирующего обращения к дескриптору
/// @param[in] fd - открытый файловый дескриптор
/// @param[in] blocking - флаг блокирующее/неблокирующее поведение дескриптора при обращении к нему
/// @return Код ошибки
std::error_code SetSocketBlocking( int fd, bool blocking )
{
     if( fd < 0 )
     {
          return std::make_error_code( std::errc::bad_file_descriptor );
     }

     errno = 0;
     int flags = fcntl( fd, F_GETFL, 0 );
     if( flags == -1 )
     {
          const int errnoLocal = errno;
          return { errnoLocal, std::system_category() };
     }
     flags = blocking ? ( flags & ~O_NONBLOCK ) : ( flags | O_NONBLOCK );
     errno = 0;
     if( 0 != fcntl( fd, F_SETFL, flags ) )
     {
          const int errnoLocal = errno;
          return { errnoLocal, std::system_category() };
     }

     return {};
}

}

TcpClient::TcpClient( std::unique_ptr<IProtocol>& protocol, std::unique_ptr<IConnectionParam>& connectionParam )
: IClient( protocol, connectionParam )
{
     std::memset( &address_, 0, sizeof( address_ ) );
     poll_ = Poller::MakePoller( PollType::Select, 1 );
}

TcpClient::~TcpClient()
{
}

std::error_code TcpClient::Connect()
{
     errno = 0;
     if( -1 == ( socket_ = socket( AF_INET, SOCK_STREAM, 0 ) ) )
     {
          const int errnoLocal = errno;
          return { errnoLocal, std::system_category() };
     }
#ifdef DEBUG
     printf( "%s[%u]: Open socket: %d\n", __FILE__, __LINE__, socket_ );
#endif // DEBUG

     const std::string iface( connectionParam_->GetNetIface() );
     errno = 0;
     if( 0 != setsockopt( socket_, SOL_SOCKET, SO_BINDTODEVICE, iface.c_str(), iface.size() + 1 ) )
     {
          const int errnoLocal = errno;
          return { errnoLocal, std::system_category() };
     }

     address_.sin_family = AF_INET;
     address_.sin_port = htons( connectionParam_->GetPort() );
     const std::string ip( connectionParam_->GetIP() );

     errno = 0;
     if( -1 == inet_pton( AF_INET, ip.c_str(), &address_.sin_addr ) )
     {
          const int errnoLocal = errno;
          return { errnoLocal, std::system_category() };
     }

     if( auto ec = SetSocketBlocking( socket_, false ) )
     {
          return ec;
     }

     bool result = false;
     while( !stop_ )
     {
          errno = 0;
          if( 0 != connect( socket_, reinterpret_cast<struct sockaddr*>( &address_ ), sizeof( address_ ) ) )
          {
               const int errnoLocal = errno;
               switch( errnoLocal )
               {
                    case 0:
                    case EALREADY:
                    case EINPROGRESS:
                    {
#ifdef DEBUG
                         // фактически это не ошибка, сокет не блокирующий, поэтому мы попали на то состояние, когда
                         // подключение в процессе
                         printf( "%s[%u]: Connection datail: %s (%d)\n", __FILE__, __LINE__, std::strerror( errnoLocal ), errnoLocal );
#endif // DEBUG
                         break;
                    }
                    case EISCONN:
                    {
                         result = true;
#ifdef DEBUG
                         printf( "%s[%u]: Connection success: %s (%d)\n", __FILE__, __LINE__, std::strerror( errnoLocal ), errnoLocal );
#endif // DEBUG
                         break;
                    }
                    default:
                    {
                         break;
                    }
               }
               char ipAddress[INET_ADDRSTRLEN] {};
               if( nullptr == inet_ntop( AF_INET, &( address_.sin_addr ), ipAddress, INET_ADDRSTRLEN ) )
               {
                    //TODO проверить
                    printf( "Server is not in a valid address family. Interger represantation of server %08X ", address_.sin_addr.s_addr );
               }
          }
          else
          {
               result = true;
          }

          if( result )
          {
               break;
          }
          sleep( 1 );
     }

     if( stop_ )
     {
          return std::make_error_code( std::errc::interrupted );
          //return std::make_error_code( std::errc::operation_canceled );
     }

     if( auto ec = SetSocketBlocking( socket_, true ) )
     {
          return ec;
     }

     int clientKeepalive = connectionParam_->GetSocketKeepAlive();
     int clientKeepidle  = connectionParam_->GetSocketKeepIdle();
     int clientKeepcnt   = connectionParam_->GetSocketKeepCnt();
     int clientKeepintvl = connectionParam_->GetSocketKeepIntvl();
     errno = 0;
     if( 0 != setsockopt( socket_, SOL_SOCKET, SO_KEEPALIVE, &clientKeepalive, sizeof( clientKeepalive ) ) )
     {
          const int errnoLocal = errno;
          //TODO проверить
          printf( "Set SO_KEEPALIVE opt failed, errno: %d (%s)\n", errnoLocal, std::strerror( errnoLocal ) );
          return { errnoLocal, std::system_category() };
     }
     errno = 0;
     if( 0 != setsockopt( socket_, IPPROTO_TCP, TCP_KEEPIDLE, &clientKeepidle, sizeof( clientKeepidle ) ) )
     {
          const int errnoLocal = errno;
          //TODO проверить
          printf( "Set TCP_KEEPIDLE opt failed, errno: %d (%s)\n", errnoLocal, std::strerror( errnoLocal ) );
          return { errnoLocal, std::system_category() };
     }
     errno = 0;
     if( 0 != setsockopt( socket_, IPPROTO_TCP, TCP_KEEPCNT, &clientKeepcnt, sizeof( clientKeepcnt ) ) )
     {
          const int errnoLocal = errno;
          //TODO проверить
          printf( "Set TCP_KEEPCNT opt failed, errno: %d (%s)\n", errnoLocal, std::strerror( errnoLocal ) );
          return { errnoLocal, std::system_category() };
     }
     errno = 0;
     if( 0 != setsockopt( socket_, IPPROTO_TCP, TCP_KEEPINTVL, &clientKeepintvl, sizeof( clientKeepintvl ) ) )
     {
          const int errnoLocal = errno;
          //TODO проверить
          printf( "Set TCP_KEEPINTVL opt failed, errno: %d (%s)\n", errnoLocal, std::strerror( errnoLocal ) );
          return { errnoLocal, std::system_category() };
     }

     struct timeval timeout;
     timeout.tv_sec = connectionParam_->GetSocketRwTimeout();
     timeout.tv_usec = 0;
     errno = 0;
     if( 0 != setsockopt( socket_, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof( timeout ) ) )
     {
          const int errnoLocal = errno;
          //TODO проверить
          printf( "Set SO_RCVTIMEO opt failed, errno: %d (%s)\n", errnoLocal, std::strerror( errnoLocal ) );
          return { errnoLocal, std::system_category() };
     }
     errno = 0;
     if( 0 != setsockopt( socket_, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof( timeout ) ) )
     {
          const int errnoLocal = errno;
          //TODO проверить
          printf( "Set SO_SNDTIMEO opt failed, errno: %d (%s)\n", errnoLocal, std::strerror( errnoLocal ) );
          return { errnoLocal, std::system_category() };
     }
#ifdef DEBUG
     printf( "%s[%u]: Connected by socket: %d, ip: %s, port: %u\n", __FILE__, __LINE__,
                                             socket_, ip.c_str(), connectionParam_->GetPort() );
#endif // DEBUG

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
     auto slct = poll_->Clone();
     if( auto ec = slct->Poll( socket_ ) )
     {
          return ec;
     }
#ifdef DEBUG
     printf( "%s[%u]: Has data to read\n", __FILE__, __LINE__ );
#endif // DEBUG
     struct sockaddr_in srcAddress;
     socklen_t addrLen = sizeof( srcAddress );

     std::vector<uint8_t> header( protocol_->GetHeaderSize() );
     errno = 0;
     if( -1 == recvfrom(
          socket_, header.data(), header.size(), 0, reinterpret_cast<struct sockaddr*>( &srcAddress ), &addrLen ) )
     {
          const int errnoLocal = errno;
          return { errnoLocal, std::system_category() };
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
     if( auto ec = slct->Poll( socket_ ) )
     {
          return ec;
     }
#ifdef DEBUG
     printf( "%s[%u]: Has data to read after header\n", __FILE__, __LINE__ );
#endif // DEBUG
     std::vector<uint8_t> payload( pldSize );
     errno = 0;
     if( -1 == recvfrom(
          socket_, payload.data(), payload.size(), 0, reinterpret_cast<struct sockaddr*>( &srcAddress ), &addrLen ) )
     {
          const int errnoLocal = errno;
          return { errnoLocal, std::system_category() };
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
     errno = 0;
     if( -1 == sendto(
          socket_, dataToSend.data(), dataToSend.size(), 0, reinterpret_cast<const struct sockaddr*>( &address_ ), addrLen ) )
     {
          const int errnoLocal = errno;
          return { errnoLocal, std::system_category() };
     }
#ifdef DEBUG
     printf( "%s[%u]: Data was send\n", __FILE__, __LINE__ );
#endif // DEBUG
     return {};
}

std::error_code TcpClient::ValidateData( const std::vector<uint8_t>& data ) const
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
