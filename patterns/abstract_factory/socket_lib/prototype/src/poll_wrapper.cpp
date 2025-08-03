#include "../i_prototype_poller.h"

#include <atomic>
#include <memory>
#include <poll.h>

#include "poll_wrapper.h"

namespace net_connection_lib
{

class PollWrapper final : public Poller
{
private:
     PollWrapper() = delete;
     PollWrapper& operator=( PollWrapper&& ) = delete;
     PollWrapper( PollWrapper&& ) = delete;

public:
     explicit PollWrapper( size_t timeout );
     ~PollWrapper() = default;

     virtual std::shared_ptr<Poller> Allocate() const noexcept override;
     virtual std::error_code Poll( int fd ) noexcept override;

private:
     std::atomic_bool stop_;
};

std::shared_ptr<Poller> MakePoll( size_t timeout )
{
     return std::make_shared<PollWrapper>( timeout );
}

PollWrapper::PollWrapper( size_t timeout )
: Poller( timeout )
{
}

std::shared_ptr<Poller> PollWrapper::Allocate() const noexcept
try
{
     return std::make_shared<PollWrapper>( timeout_ );
}
catch( ... )
{
     return nullptr;
}

std::error_code PollWrapper::Poll( int fd ) noexcept
try
{
     if( 0 > fd )
     {
          return std::make_error_code( std::errc::bad_file_descriptor );
     }

     constexpr size_t FD_ARRAY_SIZE = 1;
     struct pollfd fds[FD_ARRAY_SIZE];
     fds[0].fd = fd;
     fds[0].events = POLLIN;

     while( !stop_ )
     {
          errno = 0;
          const int rv = poll( fds, FD_ARRAY_SIZE, timeout_ * 1000 );
          const int errnoLocal = errno;
          switch( rv )
          {
               case -1:  return { errnoLocal, std::system_category() };
               case 0:   continue;
               default:  break;
          }

          break;
     }


     if( stop_ )
     {
          return std::make_error_code( std::errc::operation_canceled );
     }

     return {};
}
catch( ... )
{
     return std::make_error_code( std::errc::io_error );
}

} // namespace net_connection_lib
