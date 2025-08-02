#pragma once

#ifndef RAW_CLIENT_LIB_H__
#define RAW_CLIENT_LIB_H__

#include "../i_client.h"

#include <system_error>
#include <vector>

#include "../i_connection_params.h"

namespace net_connection_lib
{

class RawClient final : public IClient
{
public:
     RawClient( std::unique_ptr<IProtocol>& protocol, std::unique_ptr<IConnectionParam>& connectionParam );
     ~RawClient();

     virtual std::error_code Connect() override;

     virtual std::error_code Read( std::vector<uint8_t>& buff ) override;

     virtual std::error_code Write( const std::vector<uint8_t>& buff ) override;

private:
     RawClient( const RawClient& ) = delete;
     RawClient( RawClient&& ) = delete;
     RawClient& operator=( const RawClient& ) = delete;
     RawClient& operator=( RawClient&& ) = delete;
};

} // namespace net_connection_lib

#endif // RAW_CLIENT_LIB_H__
