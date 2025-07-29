#pragma once
#ifndef SINGLETON_UNIQUE_FACTOTY_LIB_H__
#define SINGLETON_UNIQUE_FACTOTY_LIB_H__

#include <memory>
#include <system_error>

namespace net_connection_lib
{

class Singleton
{
public:
     static std::shared_ptr<Singleton> Instance();
     std::error_code Register( const std::string& name );

private:
     std::error_code LookUp( const std::string& name );
};

} // namespace net_connection_lib

#endif // SINGLETON_UNIQUE_FACTOTY_LIB_H__
