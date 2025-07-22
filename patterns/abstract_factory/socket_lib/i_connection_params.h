#pragma once

#ifndef PARAMS_CONNECTION_LIB_H__
#define PARAMS_CONNECTION_LIB_H__

#include <cstddef>
#include "system_error"

namespace net_connection_lib
{

class IConnectionParam
{
public:
     explicit IConnectionParam() = default;
     virtual ~IConnectionParam() = default;

     virtual size_t GetMaxConnection() = 0;

     virtual size_t GetConnectTimeout() = 0;
     virtual size_t GetAcceptTimeout() = 0;

     virtual size_t GetReadTimeout() = 0;
     virtual size_t GetWriteTimeout() = 0;

     virtual std::error_code Clone() = 0;
     virtual std::error_code Initialize() = 0;
};

} // namespace net_connection_lib

#endif // PROTOCOL_CONNECTION_LIB_H__
