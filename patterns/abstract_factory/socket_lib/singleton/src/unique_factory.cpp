#include "../unique_factory.h"

#include <memory>
#include <stdexcept>
#include <system_error>

namespace net_connection_lib
{

namespace
{
static std::shared_ptr<Singleton> instance_;
static std::shared_ptr<std::map<std::string,Singleton*> > singletonTable_;
}

std::shared_ptr<Singleton> Singleton::Instance( const std::string& name )
{
     if( !instance_ )
     {
          instance_ = LookUp( name );
     }

     return instance_;
}

std::shared_ptr<Singleton> Singleton::LookUp( const std::string& name )
try
{
     return std::shared_ptr<Singleton>( singletonTable_->at( name ) );
}
catch( ... )
{
     return nullptr;
}

Singleton::Singleton( const std::string& name )
{
     if( auto ec = Register( name ) )
     {
          throw std::runtime_error( "Failed to register singleton, error: " + ec.message() );
     }
}

std::error_code Singleton::Register( const std::string& name )
try
{
     if( !singletonTable_ )
     {
          singletonTable_ = std::make_shared<std::map<std::string,Singleton*> >();
     }

     //TODO
     //singletonTable_[name] = shared_from_this();
     auto& ref = *singletonTable_;
     ref[name] = this;
     return {};
}
catch( ... )
{
     return std::make_error_code( std::errc::not_enough_memory );
}

} // namespace net_connection_lib
