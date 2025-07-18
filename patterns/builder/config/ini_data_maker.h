#pragma once

#ifndef INI_DATA_MAKER_PATTERN_BUILDER_H__
#define INI_DATA_MAKER_PATTERN_BUILDER_H__

#include <system_error>
#include <tuple>
#include <string>

#include "data_maker.h"
#include "parameter.h"

namespace cfg_parser
{

class IniDataMaker final : public DataMaker
{
public:
     explicit IniDataMaker() = default;
     ~IniDataMaker() = default;

     virtual inline std::error_code AddParam( const std::string& line )
               { std::ignore = line; return std::make_error_code( std::errc::not_supported ); };
     virtual inline std::error_code AddParam( ParamType type, const std::string& value )
               { std::ignore = type; std::ignore = value; return std::make_error_code( std::errc::not_supported ); };
};

} // namespace cfg_parser

#endif // INI_DATA_MAKER_PATTERN_BUILDER_H__
