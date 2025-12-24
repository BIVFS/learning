#include <chrono>
#include <exception>
#include <iostream>
#include <future>
#include <stdexcept>
#include <thread>

struct Foo
{
     int field1;
     int field2;
};

int main()
{
     std::promise<std::string> p1;
     std::thread th( [&p1]()
          {
               std::cout << "Start thread" << std::endl;
               std::this_thread::sleep_for( std::chrono::seconds( 3 ) );
               p1.set_value( "Completed" );
          }
     );

     auto tp1 = std::chrono::steady_clock::now();
     std::future<std::string> f1 = p1.get_future();
     f1.wait();
     auto tp2 = std::chrono::steady_clock::now();
     std::cout << "Duration = " << std::chrono::duration_cast<std::chrono::seconds>( tp2 - tp1 ).count() << std::endl;
     std::cout << "Promise value " << f1.get() << std::endl;


     std::promise<Foo> p2;
     std::thread th2( [&p2]()
          {
               try
               {
                    Foo foo { 0, 0 };
                    throw std::runtime_error( "ERROR" );
                    p2.set_value( foo );
               }
               catch( ... )
               {
                    p2.set_exception( std::current_exception() );
               }
          }
     );

     std::future<Foo> f2 = p2.get_future();
     f2.wait();

     try
     {
          f2.get();
     }
     catch( const std::runtime_error& ex )
     {
          std::cout << ex.what() << std::endl;
     }
     catch( ... )
     {
          std::cout << "Unknown exception" << std::endl;
     }

     th.join();
     th2.join();

     return EXIT_SUCCESS;
}
