#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <limits>
#include <cinttypes>
#include <mutex>
#include <thread>

std::mutex g_mtx1;
std::mutex g_mtx2;

void run_test( int i )
{
     int delta;// = 123; TODO
     char* mem = new char[1024];
     strcpy(mem, "i = ");
     printf( "%s %d\n", mem, i + delta );
     delete[] mem;
}

int main()
{
     int i = 0;
     //for( i = 0; i < 10; i++ )
     std::thread t( []
          {
               int j = 0;
               while( true )
               {
                    std::lock_guard<std::mutex> lock2( g_mtx2 );
                    std::this_thread::sleep_for( std::chrono::milliseconds( 10 ) );
                    std::lock_guard<std::mutex> lock1( g_mtx1 );
                    ++j;
                    run_test( 256 + j );
                    j = j % std::numeric_limits<uint8_t>::max();
                    std::this_thread::sleep_for( std::chrono::milliseconds( 10 ) );
               }
          }
     );

     while( true )
     {
          std::lock_guard<std::mutex> lock1( g_mtx1 );
          std::lock_guard<std::mutex> lock2( g_mtx2 );
          ++i;
          run_test( i );
          i = i  % std::numeric_limits<uint8_t>::max();
          std::this_thread::sleep_for( std::chrono::milliseconds( 10 ) );
     }
     t.join();
     return EXIT_SUCCESS;
}
