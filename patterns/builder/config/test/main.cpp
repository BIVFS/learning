#include <iostream>

#include "../parser.h"
#include "../data_maker.h"
#include "../ini_data_maker.h"
#include <memory>

namespace cp = cfg_parser;

int main()
{
     auto builder = std::make_shared<cp::IniDataMaker>();
     cp::Parser parser( builder );
     if( auto ec = parser.ReadFile( "/tmp/test_config.ini" ) )
     {
          std::cout << ec.message() << std::endl;
          return EXIT_FAILURE;
     }

     auto params = parser.GetParams();

     std::cout << params->size() << std::endl;

     return EXIT_SUCCESS;
}
