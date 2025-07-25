#include "default_connection_params.h"

namespace net_connection_lib
{

namespace
{
static constexpr auto SOCKET_KEEPALIVE = 1;  // Включение проверок соединения keepalive
static constexpr auto SOCKET_KEEPIDLE  = 5;  // Таймаут до начала проверок keepalive, в секундах
static constexpr auto SOCKET_KEEPCNT   = 2;  // Количество проверок соединения до закрытия
static constexpr auto SOCKET_KEEPINTVL = 5;  // Таймаут между проверками соединения

static constexpr auto MAX_SERVER_CONNECTION  = 1; // Лимит принятых соединений
static constexpr auto SOCK_RW_TIMEOUT        = 1; // Таймаут сокета на операции чтения/записи
static constexpr auto MAX_RW_ATTEMPTS        = 3; // Лимит ошибок при чтении/записи
}

size_t DefaultConnectionParam::GetMaxConnection() { return 10; }

size_t DefaultConnectionParam::GetConnectTimeout() { return 1; }
size_t DefaultConnectionParam::GetAcceptTimeout() { return 1; }

size_t DefaultConnectionParam::GetReadTimeout() { return 1; }
size_t DefaultConnectionParam::GetWriteTimeout() { return 1; }

int DefaultConnectionParam::GetSocketKeepAlive() { return SOCKET_KEEPALIVE; }
int DefaultConnectionParam::GetSocketKeepIdle() { return SOCKET_KEEPIDLE; }
int DefaultConnectionParam::GetSocketKeepCnt() { return SOCKET_KEEPCNT; }
int DefaultConnectionParam::GetSocketKeepIntvl() { return SOCKET_KEEPINTVL; }
int DefaultConnectionParam::GetSocketMaxSrvConnection() { return MAX_SERVER_CONNECTION; }
int DefaultConnectionParam::GetSocketRwTimeout() { return SOCK_RW_TIMEOUT; }
int DefaultConnectionParam::GetSocketMaxRwAttempts() { return MAX_RW_ATTEMPTS; }

std::string DefaultConnectionParam::NetIface() { return "lo"; }

std::error_code DefaultConnectionParam::Clone() { return {}; }
std::error_code DefaultConnectionParam::Initialize() { return {}; }

} // namespace net_connection_lib
