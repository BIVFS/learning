#pragma once

#ifndef PARAM_PATTERN_BUILDER_H__
#define PARAM_PATTERN_BUILDER_H__

#include <cstdint>
#include <utility>
#include <map>
#include <string>


namespace cfg_parser
{

enum class ParamType : uint8_t
{
     LogLevel = 0,
     Timeout,
     Mode
};

using Param = std::pair<ParamType,std::string>;
using Params = std::map<Param::first_type, Param::second_type>;

} // namespace cfg_parser

#endif // PARAM_PATTERN_BUILDER_H__
