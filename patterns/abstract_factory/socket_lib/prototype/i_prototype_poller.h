#pragma once

#ifndef PROTORYPE_POLL_LIB_H__
#define PROTORYPE_POLL_LIB_H__

#include <memory>
#include <system_error>

namespace net_connection_lib
{

class Poller
{
private:
     Poller() = delete;
     Poller& operator=( Poller&& ) = delete;
     Poller( Poller&& ) = delete;

public:
     explicit Poller( size_t timeout );
     virtua ~Poller() = default;

     Poller& operator=( const Poller& );
     Poller( const Poller& );
     virtual std::shared_ptr<Poller> Clone() const noexcept;
     virtual std::error_code Initialize() noexcept;
};

} // namespace net_connection_lib

#endif // PROTORYPE_POLL_LIB_H__
