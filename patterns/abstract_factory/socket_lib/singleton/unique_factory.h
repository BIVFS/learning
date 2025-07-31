#pragma once
#ifndef SINGLETON_UNIQUE_FACTOTY_LIB_H__
#define SINGLETON_UNIQUE_FACTOTY_LIB_H__

#include <memory>
#include <system_error>
#include <map>

namespace net_connection_lib
{

     //TODO
class Singleton // : public std::enable_shared_from_this<Singleton>
{
private:
     Singleton() = delete;
     Singleton( const Singleton& ) = delete;
     Singleton( Singleton&& ) = delete;
     Singleton& operator=( const Singleton& ) = delete;
     Singleton& operator=( Singleton&& ) = delete;

public:
     static std::shared_ptr<Singleton> Instance( const std::string& name );

     explicit Singleton( const std::string& name );

     virtual ~Singleton() = default;

protected:
     std::error_code Register( const std::string& name );

private:
     static std::shared_ptr<Singleton> LookUp( const std::string& name );

//private:
     //static std::shared_ptr<Singleton> instance_;
     //static std::map<std::string,std::shared_ptr<Singleton> > singletonTable_;
};

} // namespace net_connection_lib

#endif // SINGLETON_UNIQUE_FACTOTY_LIB_H__
