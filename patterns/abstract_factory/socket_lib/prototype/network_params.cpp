#include <memory>
#include <stdexcept>

#include "../../network_params.h"
#include "../simple_protocol.h"
#include "../default_connection_params.h"

namespace net_connection_lib
{

std::unique_ptr<IConnectionParam> NetworkParams::CreateConnectionParamsPrototype( ConnectionParamSet paramSet )
{
     switch( paramSet )
     {
          case ConnectionParamSet::Default: return std::make_unique<DefaultConnectionParam>();
          default : throw std::logic_error( "Unexpected connection params" );
     }
}

std::unique_ptr<IProtocol> NetworkParams::CreateProtocolPrototype( ProtocolType type )
{
     switch( type )
     {
          case ProtocolType::Simple: return std::make_unique<SimpleProtocol>();
          default : throw std::logic_error( "Unexpected protocol type" );
     }
}

} // namespace net_connection_lib
