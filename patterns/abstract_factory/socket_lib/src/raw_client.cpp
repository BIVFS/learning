#include "raw_client.h"
#include <system_error>

namespace net_connection_lib
{

RawClient::RawClient()
{
}

RawClient::~RawClient()
{
}

std::error_code RawClient::Connect( const std::string& ip, uint16_t port )
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
