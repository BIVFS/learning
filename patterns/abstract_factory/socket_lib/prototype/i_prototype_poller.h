#pragma once

#ifndef PROTORYPE_POLL_LIB_H__
#define PROTORYPE_POLL_LIB_H__

#include <memory>
#include <system_error>

namespace net_connection_lib
{

enum class PollType : uint8_t
{
     Select = 0,
     Poll
};

class Poller
{
private:
     Poller() = delete;
     Poller& operator=( Poller&& ) = delete;
     Poller( Poller&& ) = delete;

public:
     static std::shared_ptr<Poller> MakePoller( PollType type, size_t timeout ) noexcept;

     virtual ~Poller() = default;

     Poller& operator=( const Poller& );
     Poller( const Poller& );
     virtual std::shared_ptr<Poller> Clone() const noexcept;
     virtual std::error_code Initialize() noexcept;
     virtual std::error_code Poll( int fd ) noexcept;

protected:
     explicit Poller( size_t timeout ) : timeout_( timeout ) {}

protected:
     size_t timeout_;
};

} // namespace net_connection_lib

#endif // PROTORYPE_POLL_LIB_H__
