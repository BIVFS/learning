#include <iostream>
#include <cstring>

#include "test_server.h"
#include "test_client.h"


int main( int argc, char** argv )
{
     if( 2 != argc )
     {
          return EXIT_FAILURE;
     }

     if( 0 == strcmp( argv[1], "--server" ) )
     {
          my_server::Server server;
          server.Start( "127.0.0.1", 5555 );

          int var = 0;
          std::cin >> var;

          server.Stop();
     }
     else if( 0 == strcmp( argv[1], "--client" ) )
     {
          my_client::Client client;
          client.Start( "127.0.0.1", 5555 );

          int var = 0;
          std::cin >> var;

          client.Stop();
     }

     return EXIT_SUCCESS;
}
