#include "../i_prototype_poller.h"

#include <atomic>
#include <memory>

#include "select.h"

namespace net_connection_lib
{

class SelectPoll final : public Poller
{
private:
     SelectPoll() = delete;
     SelectPoll& operator=( SelectPoll&& ) = delete;
     SelectPoll( SelectPoll&& ) = delete;

public:
     explicit SelectPoll( size_t timeout );
     ~SelectPoll() = default;

     virtual std::shared_ptr<Poller> Allocate() const noexcept override;
     virtual std::error_code Poll( int fd ) noexcept override;

private:
     std::atomic_bool stop_;
};

std::shared_ptr<Poller> MakeSelect( size_t timeout )
{
     //return new SelectPoll( timeout );
     return std::make_shared<SelectPoll>( timeout );
}

SelectPoll::SelectPoll( size_t timeout )
: Poller( timeout )
{
}

std::shared_ptr<Poller> SelectPoll::Allocate() const noexcept
try
{
     return std::make_shared<SelectPoll>( timeout_ );
}
catch( ... )
{
     return nullptr;
}

std::error_code SelectPoll::Poll( int fd ) noexcept
try
{
     if( 0 > fd )
     {
          return std::make_error_code( std::errc::bad_file_descriptor );
     }
     struct timeval tv{ static_cast<time_t>( timeout_ ), 0 };
     fd_set readSet;
     {
          while( !stop_ )
          {
               // Особенность select с его наборами дескрипторов в том, что при вызове select в наборах что-то
               // изменяется и при повторном select нужно создавать инициализировать их заново
               FD_ZERO( &readSet );
               FD_SET( fd, &readSet );

               errno = 0;
               const int rv = select( fd + 1, &readSet, 0, 0, &tv );
               const int errnoLocal = errno;
               switch( rv )
               {
                    case -1:  return { errnoLocal, std::system_category() };
                    case 0:   continue;
                    default:  break;
               }

               break;
          }
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
