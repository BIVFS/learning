#pragma once

#ifndef TCP_CLIENT_LIB_H__
#define TCP_CLIENT_LIB_H__

#include "../i_client.h"

#include <system_error>
#include <string>
#include <vector>

#include <netinet/in.h>

#include "../i_connection_params.h"

namespace net_connection_lib
{

class TcpClient final : public IClient
{
public:
     explicit TcpClient( std::unique_ptr<IProtocol>& protocol, std::unique_ptr<IConnectionParam>& connectionParam );
     ~TcpClient();

     virtual std::error_code Connect( const std::string& ip, uint16_t port ) override;

     virtual std::error_code Read( std::vector<uint8_t>& buff ) override;

     virtual std::error_code Write( const std::vector<uint8_t>& buff ) override;

private:
     inline bool IsEstablished() const { return ( -1 != socket_ ); }

     // TODO Добавить делегировать опрос отдельному объекту
     std::error_code Select();

     std::error_code ValidateData( const std::vector<uint8_t>& data );

private:
     TcpClient() = delete;
     TcpClient( const TcpClient& ) = delete;
     TcpClient( TcpClient&& ) = delete;
     TcpClient& operator=( const TcpClient& ) = delete;
     TcpClient& operator=( TcpClient&& ) = delete;

private:
     struct sockaddr_in address_;
};

} // namespace net_connection_lib

#endif // TCP_CLIENT_LIB_H__
