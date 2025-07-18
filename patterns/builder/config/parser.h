#pragma once

#ifndef CONFIG_PARSER_PATTERN_BUILDER_H__
#define CONFIG_PARSER_PATTERN_BUILDER_H__

#include <system_error>
#include <memory>

#include "data_maker.h"
#include "parameter.h"

namespace cfg_parser
{

class Parser
{
public:
     explicit Parser( std::shared_ptr<DataMaker> builder );
     void SetBuilder( std::shared_ptr<DataMaker> builder );

     std::error_code ReadFile( const std::string& filepath );
     std::error_code WriteFile( const std::string& filepath );

     inline std::shared_ptr<Params> GetParams() { return params_; };

     //TODO Getters and Setters

private:
     Parser() = delete;
     Parser( const Parser& ) = delete;
     Parser( Parser&& ) = delete;
     Parser& operator=( const Parser& ) = delete;
     Parser& operator=( Parser&& ) = delete;

private:
     std::shared_ptr<DataMaker> builder_;
     std::shared_ptr<Params> params_;
};

} // namespace cfg_parser

#endif // CONFIG_PARSER_PATTERN_BUILDER_H__
