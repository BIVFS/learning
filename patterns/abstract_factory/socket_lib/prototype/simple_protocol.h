#pragma once

#ifndef SIMPLE_PROTOCOL_CONNECTION_LIB_H__
#define SIMPLE_PROTOCOL_CONNECTION_LIB_H__

#include <system_error>
#include <vector>
#include <cstdint>

#include "../i_protocol.h"

namespace net_connection_lib
{

class SimpleProtocol final : public IProtocol
{
public:
     explicit SimpleProtocol() = default;
     ~SimpleProtocol() = default;

     virtual inline size_t GetHeaderSize() override;

     virtual inline std::error_code GetPayloadSize( const std::vector<uint8_t>& header, size_t& payloadSize ) override;

     virtual std::error_code Serialize() override;

     virtual std::error_code Deserialize( const std::vector<uint8_t>& data ) override;

     virtual std::error_code Clone() override;

     virtual std::error_code Initialize() override;
};

} // namespace net_connection_lib

#endif // SIMPLE_PROTOCOL_CONNECTION_LIB_H__
