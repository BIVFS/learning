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

     virtual int GetSocketKeepAlive() = 0;        // Статус проверок соединения keepalive
     virtual int GetSocketKeepIdle() = 0;         // Таймаут до начала проверок keepalive, в секундах
     virtual int GetSocketKeepCnt() = 0;          // Количество проверок соединения до закрытия
     virtual int GetSocketKeepIntvl() = 0;        // Таймаут между проверками соединения
     virtual int GetSocketMaxSrvConnection() = 0; // Лимит принятых соединений
     virtual int GetSocketRwTimeout() = 0;        // Таймаут сокета на операции чтения/записи
     virtual int GetSocketMaxRwAttempts() = 0;    // Лимит ошибок при чтении/записи

     virtual std::string GetIP() = 0;
     virtual uint16_t GetPort() = 0;
     virtual std::string GetNetIface() = 0;

     virtual std::error_code Clone() = 0;
     virtual std::error_code Initialize() = 0;

};

} // namespace net_connection_lib

#endif // PROTOCOL_CONNECTION_LIB_H__
