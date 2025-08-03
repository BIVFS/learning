#pragma once

#ifndef TCP_CLIENT_LIB_H__
#define TCP_CLIENT_LIB_H__

#include "../i_client.h"

#include <system_error>
#include <string>
#include <vector>

#include <netinet/in.h>

#include "../i_connection_params.h"
#include "../prototype/i_prototype_poller.h"

namespace net_connection_lib
{

     //TODO noexcept и const Read Write
class TcpClient final : public IClient
{
public:
     explicit TcpClient( std::unique_ptr<IProtocol>& protocol, std::unique_ptr<IConnectionParam>& connectionParam );
     ~TcpClient();

     virtual std::error_code Connect() override;

     virtual std::error_code Read( std::vector<uint8_t>& buff ) override;

     virtual std::error_code Write( const std::vector<uint8_t>& buff ) override;

private:
     inline bool IsEstablished() const { return ( -1 != socket_ ); }

     std::error_code ValidateData( const std::vector<uint8_t>& data ) const;

private:
     TcpClient() = delete;
     TcpClient( const TcpClient& ) = delete;
     TcpClient( TcpClient&& ) = delete;
     TcpClient& operator=( const TcpClient& ) = delete;
     TcpClient& operator=( TcpClient&& ) = delete;

private:
     struct sockaddr_in address_;

     std::shared_ptr<Poller> poll_;
};

} // namespace net_connection_lib

#endif // TCP_CLIENT_LIB_H__
