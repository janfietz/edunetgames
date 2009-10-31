#ifndef __EMPTYRPCPLUGIN_H__
#define __EMPTYRPCPLUGIN_H__

#include "Tutorial_01/plugins/EmptyPlugin.h"
#include "Tutorial_02/plugins/ClientPlugin.h"
#include "Tutorial_02/plugins/PeerPlugin.h"

// ----------------------------------------------------------------------------
class EmptyRpcPlugin : public PeerPlugin<EduNetGames::EmptyPlugIn>
{
	ET_DECLARE_BASE(PeerPlugin<EduNetGames::EmptyPlugIn>);
public:
	EmptyRpcPlugin();
	virtual ~EmptyRpcPlugin(){};

	virtual const char* name (void) const {return "EmptyRpcPlugin";};

	 bool requestInitialSelection (void) const {return true;}
	 virtual float selectionOrderSortKey (void) const { return 2.0f ;}
	 
	virtual bool needRedraw ( void ) const { return false; }
	
	void handleFunctionKeys (int keyNumber);
	
	virtual void StartNetworkSession( void );

private:
	
};

#endif // __EMPTYRPCPLUGIN_H__