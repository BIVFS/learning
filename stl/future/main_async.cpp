#include <chrono>
#include <iostream>
#include <future>
#include <thread>

int main()
{
     std::future<int> f = std::async( []() { std::cout << "rerurn 8" << std::endl;  return 8; } );

     std::future<int> f2 = std::async( std::launch::deferred, []() { std::cout << "return 3" << std::endl; return 3; } );

     std::cout << "sleep" << std::endl;

     std::this_thread::sleep_for( std::chrono::seconds( 3 ) );
     std::cout << "after sleep" << std::endl;

     std::cout << f2.get() << std::endl;
     std::cout << f.get() << std::endl;

     return EXIT_SUCCESS;
}
