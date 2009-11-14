#ifndef __CLIENTSERVERPLUGIN_H__
#define __CLIENTSERVERPLUGIN_H__

#include "OpenSteerUT/PluginArray.h"

//-----------------------------------------------------------------------------
class ClientServerPlugin : public OpenSteer::PluginArray
{
	ET_DECLARE_BASE(OpenSteer::PluginArray);
public:

	ClientServerPlugin();
	virtual ~ClientServerPlugin();

	//---------------------------------------------------------------------
	// interface AbstractPlugin
	virtual const char *name(void) const; 

	virtual void initGui( void* pkUserdata );


};



#endif // __CLIENTSERVERPLUGIN_H__