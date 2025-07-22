#pragma once

#ifndef PROTOCOL_CONNECTION_LIB_H__
#define PROTOCOL_CONNECTION_LIB_H__

#include <system_error>
#include <vector>
#include <cstdint>
#include <cstddef>

namespace net_connection_lib
{

class IProtocol
{
public:
     explicit IProtocol() = default;
     virtual ~IProtocol() = default;

     virtual size_t GetHeaderSize() = 0;
     virtual std::error_code GetPayloadSize( const std::vector<uint8_t>& header, size_t& payloadSize ) = 0;

     virtual std::error_code Serialize() = 0;
     virtual std::error_code Deserialize( const std::vector<uint8_t>& data ) = 0;

     virtual std::error_code Clone() = 0;
     virtual std::error_code Initialize() = 0;
};

} // namespace net_connection_lib

#endif // PROTOCOL_CONNECTION_LIB_H__
