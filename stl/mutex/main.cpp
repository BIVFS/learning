#include <chrono>
#include <iostream>
#include <mutex>
#include <shared_mutex>
#include <thread>

std::mutex g_mtx;
std::recursive_mutex g_r_mtx;

std::timed_mutex g_t_mtx;

//TODO нет кода проверок этих типов мьютексов
std::recursive_timed_mutex g_r_t_mtx;
std::shared_timed_mutex g_sh_t_mtx;

int main( int argc, char* argv[] )
{
     if( 2 > argc )
     {
          std::cerr << "Invalid options" << std::endl;
          return EXIT_FAILURE;
     }

     std::string mode( argv[1] );

     std::thread t;

     if( 0 == mode.compare( "mutex" ) )
     {
          std::cout << "Main start" << std::endl;
          std::lock_guard<decltype( g_mtx )> lock1( g_mtx );
          std::cout << "Lock g_mtx (MAIN)" << std::endl;

          t = std::thread( []
               {
                    std::cout << "Try to lock g_mtx" << std::endl;
                    std::lock_guard<decltype( g_mtx )> lock2( g_mtx );
                    std::cout << "Lock g_mtx" << std::endl;
               }
          );
          std::cout << "Sleep for 5 seconds (MAIN)" << std::endl;
          std::this_thread::sleep_for( std::chrono::seconds( 5 ) );
          std::cout << "End of lock scope (MAIN)" << std::endl;
     }
     else if( 0 == mode.compare( "timed_mutex" ) )
     {
          std::cout << "Main start" << std::endl;
          std::lock_guard<decltype( g_t_mtx )> lock1( g_t_mtx );
          std::cout << "Lock g_t_mtx (MAIN)" << std::endl;

          t = std::thread( []
               {
                    while( true )
                    {
                         std::cout << "Try to lock g_t_mtx for 3 seconds" << std::endl;
                         std::unique_lock<decltype( g_t_mtx )> lock2( g_t_mtx, std::defer_lock );
                         if( lock2.try_lock_for( std::chrono::seconds( 3 ) ) )
                         {
                              std::cout << "Lock g_t_mtx" << std::endl;
                              break;
                         }
                         else
                         {
                              std::cout << "Failure to lock g_t_mtx" << std::endl;
                         }
                    }
               }
          );
          std::cout << "Sleep for 5 seconds (MAIN)" << std::endl;
          std::this_thread::sleep_for( std::chrono::seconds( 5 ) );
          std::cout << "End of lock scope (MAIN)" << std::endl;
     }
     else if( 0 == mode.compare( "recursive_mutex" ) )
     {
          std::cout << "Main start" << std::endl;
          { // std::mutex
               std::unique_lock<decltype( g_mtx )> lock1( g_mtx, std::defer_lock );
               for( size_t i = 0; i < 2; ++i )
               {
                    try
                    {
                         if( lock1.try_lock() )
                         {
                              std::cout << "Lock g_mtx (MAIN)" << std::endl;
                         }
                         else
                         {
                              std::cout << "Failure to lock g_mtx (MAIN)" << std::endl;
                              break;
                         }
                    }
                    catch( ... )
                    { // в случае повторного захвата может бросить исключение
                         std::cout << "Failure to lock g_mtx (MAIN)" << std::endl;
                         break;
                    }
               }
          }

          std::unique_lock<decltype( g_r_mtx )> lock1;
          // std::recursive_mutex
          for( size_t i = 0; i < 2; ++i )
          {
               try
               {
                    if( g_r_mtx.try_lock() )
                    {
                         std::unique_lock<decltype( g_r_mtx )>( g_r_mtx, std::adopt_lock ).swap( lock1 );
                         std::cout << "Lock g_r_mtx (MAIN)" << std::endl;
                    }
                    else
                    {
                         std::cout << "Failure to lock g_r_mtx (MAIN)" << std::endl;
                         break;
                    }
               }
               catch( ... )
               { // в случае повторного захвата может бросить исключение
                    std::cout << "Failure to lock g_r_mtx (MAIN)" << std::endl;
                    break;
               }
          }

          t = std::thread( []
               {
                    std::cout << "Try to lock g_r_mtx" << std::endl;
                    std::lock_guard<decltype( g_r_mtx )> lock2( g_r_mtx );
                    std::cout << "Lock g_r_mtx" << std::endl;
               }
          );
          std::cout << "Sleep for 5 seconds (MAIN)" << std::endl;
          std::this_thread::sleep_for( std::chrono::seconds( 5 ) );
          std::cout << "End of lock scope (MAIN)" << std::endl;
     }

     if( t.joinable() )
     {
          t.join();
     }

     return EXIT_SUCCESS;
}
