#pragma once
#include "PeerPlugin.h"
#include "ClientPlugin.h"
#include "opensteer/plugins/SamplePlugins.h"


class NetPeerBoidPlugin : public PeerPlugin<OpenSteer::BoidsPlugIn>
{
public:
	NetPeerBoidPlugin(){};
	virtual ~NetPeerBoidPlugin(){};

	virtual const char* name (void){return "NetPeerBoidPlugin";};

	 bool requestInitialSelection (void) {return true;}
	 virtual float selectionOrderSortKey (void) { return 2.0f ;}
};

class NetClientBoidPlugin : public ClientPlugin<OpenSteer::BoidsPlugIn>
{
public:
	NetClientBoidPlugin(){};
	virtual ~NetClientBoidPlugin(){};

	virtual const char* name (void){return "NetClientBoidPlugin";};

	virtual float selectionOrderSortKey (void) { return 3.0f ;}
};