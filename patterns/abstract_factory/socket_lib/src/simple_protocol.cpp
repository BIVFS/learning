#include "simple_protocol.h"

namespace net_connection_lib
{

namespace
{
constexpr size_t HEADER_SIZE = sizeof( uint32_t );
}

size_t SimpleProtocol::GetHeaderSize()
{
     return HEADER_SIZE;
}

std::error_code SimpleProtocol::GetPayloadSize( const std::vector<uint8_t>& header, size_t& payloadSize )
{
     if( HEADER_SIZE != header.size() )
     {
          return std::make_error_code( std::errc::bad_message );
     }
     payloadSize = *( reinterpret_cast<const uint32_t*>( header.data() ) );
     return {};
}

std::error_code SimpleProtocol::Serialize()
{
     return std::make_error_code( std::errc::not_supported );
}

std::error_code SimpleProtocol::Deserialize( const std::vector<uint8_t>& data )
{
     return std::make_error_code( std::errc::not_supported );
}

std::error_code SimpleProtocol::Clone()
{
     return {};
}

std::error_code SimpleProtocol::Initialize()
{
     return {};
}

} // namespace net_connection_lib
