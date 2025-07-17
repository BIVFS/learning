#pragma once

#ifndef RAW_SERVER_LIB_H__
#define RAW_SERVER_LIB_H__

#include "../i_server.h"

#include <system_error>
#include <vector>
#include <map>

namespace net_connection_lib
{

class RawServer final : public IServer
{
public:
     RawServer();
     ~RawServer();

     virtual std::error_code Listen( const std::string& ip, uint16_t port ) override;

     virtual std::error_code Accept( SocketId& id ) override;

     virtual std::error_code Read( SocketId id, std::vector<uint8_t>& buff ) override;

     virtual std::error_code Write( SocketId id, const std::vector<uint8_t>& buff ) override;

private:
     std::map<SocketId, int> openConnections_;

private:
     RawServer( const RawServer& ) = delete;
     RawServer( RawServer&& ) = delete;
     RawServer& operator=( const RawServer& ) = delete;
     RawServer& operator=( RawServer&& ) = delete;
};

} // net_connection_lib

#endif // RAW_SERVER_LIB_H__
