#pragma once

#ifndef I_SERVER_LIB_H__
#define I_SERVER_LIB_H__

#include <system_error>
#include <map>
#include <string>
#include <vector>
#include <atomic>

namespace net_connection_lib
{

class IServer
{
public:
     using SocketId = uint32_t;
public:
     virtual ~IServer() { Stop(); };

     inline void Stop() { stop_ = true; };

     virtual std::error_code Listen( const std::string& ip, uint16_t port ) = 0;

     virtual std::error_code Accept( SocketId& id ) = 0;

     virtual std::error_code Read( SocketId id, std::vector<uint8_t>& buff ) = 0;

     virtual std::error_code Write( SocketId id, const std::vector<uint8_t>& buff ) = 0;

private:
     std::atomic_bool stop_;
     std::map<SocketId, int> openConnections_;

private:
     IServer( const IServer& ) = delete;
     IServer( IServer&& ) = delete;
     IServer& operator=( const IServer& ) = delete;
     IServer& operator=( IServer&& ) = delete;

protected:
     IServer() = default;
};

} // net_connection_lib

#endif // I_SERVER_LIB_H__
