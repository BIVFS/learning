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

     virtual size_t GetMaxConnection() override;

     virtual size_t GetConnectTimeout() override;
     virtual size_t GetAcceptTimeout() override;

     virtual size_t GetReadTimeout() override;
     virtual size_t GetWriteTimeout() override;

     virtual int GetSocketKeepAlive() override;
     virtual int GetSocketKeepIdle() override;
     virtual int GetSocketKeepCnt() override;
     virtual int GetSocketKeepIntvl() override;
     virtual int GetSocketMaxSrvConnection() override;
     virtual int GetSocketRwTimeout() override;
     virtual int GetSocketMaxRwAttempts() override;

     virtual std::string NetIface() override;

     virtual std::error_code Clone() override;
     virtual std::error_code Initialize() override;
};

} // namespace net_connection_lib

#endif // DEFAULT_PROTOCOL_CONNECTION_LIB_H__
