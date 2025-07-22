#pragma once

#ifndef DEFAULT_PARAMS_CONNECTION_LIB_H__
#define DEFAULT_PARAMS_CONNECTION_LIB_H__

#include "../i_connection_params.h"

namespace net_connection_lib
{

class DefaultConnectionParam final : public IConnectionParam
{
public:
     explicit DefaultConnectionParam() = default;
     ~DefaultConnectionParam() = default;

     virtual inline size_t GetMaxConnection() override { return 10; }

     virtual inline size_t GetConnectTimeout() override { return 1; }
     virtual inline size_t GetAcceptTimeout() override { return 1; }

     virtual inline size_t GetReadTimeout() override { return 1; }
     virtual inline size_t GetWriteTimeout() override { return 1; }

     virtual inline std::error_code Clone() override { return {}; }
     virtual inline std::error_code Initialize() override { return {}; }
};

} // namespace net_connection_lib

#endif // DEFAULT_PROTOCOL_CONNECTION_LIB_H__
