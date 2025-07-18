#include "../i_connection.h"

#include <iostream>

int main()
{
     namespace ncl = ::net_connection_lib;
     auto factory = ncl::IConnectionFactory::CreateFactory( ncl::ConnectionType::Tcp );

     std::cout << "Stop" << std::endl;

     return EXIT_SUCCESS;
}
