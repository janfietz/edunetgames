#pragma once
#include "networkplugin.h"

template < class PluginClass = OpenSteer::PlugIn  >
class ClientPlugin :
	public NetworkPlugIn<PluginClass>
{
public:
	ClientPlugin(void){};
	virtual ~ClientPlugin(void){};

	virtual const char* name (void){return "ClientPlugin";};

};
