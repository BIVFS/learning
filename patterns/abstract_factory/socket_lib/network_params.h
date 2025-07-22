#pragma once

#ifndef NETWORK_PARAMS_CONNECTION_LIB_H__
#define NETWORK_PARAMS_CONNECTION_LIB_H__

#include <memory>

#include "i_protocol.h"
#include "i_connection_params.h"

namespace net_connection_lib
{

enum class ProtocolType : uint8_t
{
     Simple = 0
};

enum class ConnectionParamSet : uint8_t
{
     Default = 0
};

class NetworkParams
{
public:
     explicit NetworkParams() = default;
     virtual ~NetworkParams() = default;

     virtual std::unique_ptr<IConnectionParam> CreateConnectionParamsPrototype( ConnectionParamSet paramSet );
     virtual std::unique_ptr<IProtocol> CreateProtocolPrototype( ProtocolType type );
};

} // namespace net_connection_lib

#endif // NETWORK_PARAMS_CONNECTION_LIB_H__
