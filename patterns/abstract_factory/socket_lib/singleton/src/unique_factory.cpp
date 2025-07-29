#include "../unique_factory.h"

namespace net_connection_lib
{

namespace
{
static std::shared_ptr<Singleton> instance_ = nullptr;
//static Singleton::instance_ = nullptr;
}

std::shared_ptr<Singleton> Singleton::Instance()
{
     if( !instance_ )
     {
          //TODO
          throw;
     }

     return instance_;
}

std::error_code Singleton::Register( const std::string& name )
{
     return std::make_error_code( std::errc::not_supported );
}

std::error_code Singleton::LookUp( const std::string& name )
{
     return std::make_error_code( std::errc::not_supported );
}

} // namespace net_connection_lib
