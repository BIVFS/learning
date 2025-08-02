#include "i_prototype_poller.h"
#include <system_error>

namespace net_connection_lib
{

Poller& Poller::operator=( const Poller& other )
{
     if( this != &other )
     {
          this->timeout_ = other.timeout_;
     }
     return *this;
}

Poller::Poller( const Poller& other )
{
     this->timeout_ = other.timeout_;
}

std::shared_ptr<Poller> Poller::Clone() const noexcept
{
     const Poller& ref = *this;
     return std::make_shared<Poller>( ref );
}

std::error_code Poller::Initialize( int fd ) noexcept
{
     fd_ = fd;
     return {};
}

std::error_code Poller::Poll() noexcept
{
     return std::make_error_code( std::errc::not_supported );
}

} // namespace net_connection_lib
