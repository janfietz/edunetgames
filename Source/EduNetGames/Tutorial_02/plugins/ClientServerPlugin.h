#ifndef __CLIENTSERVERPLUGIN_H__
#define __CLIENTSERVERPLUGIN_H__

#include "OpenSteerExtras/PlugInArray.h"

//-----------------------------------------------------------------------------
class ClientServerPlugin : public OpenSteer::PlugInArray
{
	ET_DECLARE_BASE(OpenSteer::PlugInArray);
public:

	ClientServerPlugin();
	virtual ~ClientServerPlugin();

	//---------------------------------------------------------------------
	// interface AbstractPlugIn
	virtual const char *name(void) const; 

};



#endif // __CLIENTSERVERPLUGIN_H__