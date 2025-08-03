#include "i_prototype_poller.h"
#include <memory>
#include <system_error>

#include "src/select.h"
#include "src/poll_wrapper.h"

namespace net_connection_lib
{

std::shared_ptr<Poller> Poller::MakePoller( PollType type, size_t timeout ) noexcept
try
{
     switch( type )
     {
          case PollType::Select:
          {
               return MakeSelect( timeout );
          }
          case PollType::Poll:
          {
               return MakePoll( timeout );
          }
          default:
          {
               return nullptr;
          }
     }
}
catch( ... )
{
     return nullptr;
}

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
     auto ptr = this->Allocate();
     ptr->timeout_ = this->timeout_;
     return ptr;
}

std::error_code Poller::Initialize() noexcept
{
     return {};
}

std::error_code Poller::Poll( int fd ) noexcept
{
     return std::make_error_code( std::errc::not_supported );
}

} // namespace net_connection_lib
