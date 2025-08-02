#include "i_prototype_poller.h"

namespace net_connection_lib
{

class Select final : public Poller
{
private:
     Select() = delete;
     Select& operator=( Select&& ) = delete;
     Select( Select&& ) = delete;

public:
     explicit Select( size_t timeout );
     ~Select() = default;

     virtual std::shared_ptr<Select> Clone() const override noexcept;
     virtual std::error_code Initialize( int fd ) override noexcept;
     virtual std::error_code Poll() noexcept;

private:
     size_t timeout_;
};

Select::Select( size_t timeout )
: fd_( -1 )
, timeout_( timeout )
{
}

std::shared_ptr<Select> Select::Clone()
{
     return std::make_shared_ptr<Select>( timeout_ );
}

std::error_code Select::Initialize( int fd )
{
}

} // namespace net_connection_lib
