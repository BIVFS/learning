#include "raw_client.h"
#include <system_error>

#include "../i_connection_params.h"

namespace net_connection_lib
{

RawClient::RawClient( std::unique_ptr<IProtocol>& protocol, std::unique_ptr<IConnectionParam>& connectionParam )
: IClient( protocol, connectionParam )
{
}

RawClient::~RawClient()
{
}

std::error_code RawClient::Connect()
{
     return std::make_error_code( std::errc::not_supported );
}

std::error_code RawClient::Read( std::vector<uint8_t>& buff )
{
     return std::make_error_code( std::errc::not_supported );
}

std::error_code RawClient::Write( const std::vector<uint8_t>& buff )
{
     return std::make_error_code( std::errc::not_supported );
}

} // namespace net_connection_lib
