#include "../parser.h"

#include <fstream>
#include <memory>
#include <vector>

namespace cfg_parser
{

Parser::Parser( std::shared_ptr<DataMaker> builder )
{
     builder_ = builder;
     params_ = std::make_shared<Params>();
}

std::error_code Parser::ReadFile( const std::string& filepath )
{
     std::ifstream ifs( filepath );
     if( !ifs.is_open() )
     {
          return std::make_error_code( std::errc::io_error );
     }

     std::string line;
     while( !ifs.eof() && getline( ifs, line ) )
     {
          if( builder_->AddParam( line ) )
          {
               return std::make_error_code( std::errc::io_error );
          }
     }

     params_ = builder_->GetParams();

     return {};
}

std::error_code Parser::WriteFile( const std::string& filepath )
{
     builder_->ResetData();
     for( const auto& it : *params_ )
     {
          if( builder_->AddParam( it.first, it.second ) )
          {
               return std::make_error_code( std::errc::io_error );
          }
     }

     const std::vector<std::string> data = builder_->GetData();

     std::ofstream ofs( filepath, std::ios_base::out | std::ios_base::binary | std::ios_base::trunc );
     if( !ofs.is_open() )
     {
          return std::make_error_code( std::errc::io_error );
     }

     for( const auto& it : data )
     {
          ofs << it;
     }

     if( !ofs.good() )
     {
          return std::make_error_code( std::errc::io_error );
     }

     return {};
}

} // namespace cfg_parser
