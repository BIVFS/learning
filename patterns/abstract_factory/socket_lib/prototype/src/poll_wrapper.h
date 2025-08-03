#pragma once

#ifndef WRAPPER_POLL_LIB_H__
#define WRAPPER_POLL_LIB_H__

#include "../i_prototype_poller.h"

#include <memory>

namespace net_connection_lib
{

std::shared_ptr<Poller> MakePoll( size_t timeout );

} // namespace net_connection_lib

#endif // WRAPPER_POLL_LIB_H__
