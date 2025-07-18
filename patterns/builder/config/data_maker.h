#pragma once

#ifndef DATA_MAKER_PATTERN_BUILDER_H__
#define DATA_MAKER_PATTERN_BUILDER_H__

#include <system_error>
#include <tuple>
#include <string>
#include <memory>
#include <vector>

#include "parameter.h"

namespace cfg_parser
{

class DataMaker
{
public:
     explicit DataMaker() { params_ = std::make_shared<Params>(); }
     virtual ~DataMaker() = default;

     virtual void ResetData() { params_->clear(); } ;

     virtual inline std::error_code AddParam( const std::string& line )
                                                            { std::ignore = line; return {}; };
     virtual inline std::error_code AddParam( ParamType type, const std::string& value )
                                                            { std::ignore = type; std::ignore = value; return {}; };

     virtual inline std::shared_ptr<Params> GetParams() { return params_; };

     virtual inline std::vector<std::string> GetData() { return {}; };

private:
     std::shared_ptr<Params> params_;
};

} // namespace cfg_parser

#endif // DATA_MAKER_PATTERN_BUILDER_H__
