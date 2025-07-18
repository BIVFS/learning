#pragma once

#ifndef SNMP_DATA_MAKER_PATTERN_BUILDER_H__
#define SNMP_DATA_MAKER_PATTERN_BUILDER_H__

#include <system_error>
#include <tuple>
#include <string>

#include "data_maker.h"
#include "parameter.h"

namespace cfg_parser
{

class SnmpDataMaker final : public DataMaker
{
public:
     explicit SnmpDataMaker() = default;
     ~SnmpDataMaker() = default;

     virtual inline std::error_code AddParam( const std::string& line )
               { std::ignore = line; return std::make_error_code( std::errc::not_supported ); };
     virtual inline std::error_code AddParam( ParamType type, const std::string& value )
               { std::ignore = type; std::ignore = value; return std::make_error_code( std::errc::not_supported ); };
};

} // namespace cfg_parser

#endif // SNMP_DATA_MAKER_PATTERN_BUILDER_H__
