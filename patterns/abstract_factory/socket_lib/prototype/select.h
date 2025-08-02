#pragma once

#ifndef SELECT_POLL_LIB_H__
#define SELECT_POLL_LIB_H__

#include "i_prototype_poller.h"

#include <memory>

namespace net_connection_lib
{

std::shared_ptr<Poller> MakeSelect( size_t timeout );

} // namespace net_connection_lib

#endif // SELECT_POLL_LIB_H__
