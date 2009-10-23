#pragma once
#include "PeerPlugin.h"
#include "opensteer/plugins/SamplePlugins.h"


class NetBoidPlugin : public PeerPlugin<OpenSteer::BoidsPlugIn>
{
public:
	NetBoidPlugin(){};
	virtual ~NetBoidPlugin(){};

	 bool requestInitialSelection (void) {return true;}
};