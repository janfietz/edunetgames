#pragma once
#include "networkplugin.h"

template < class PluginClass = OpenSteer::PlugIn  >
class PeerPlugin :
	public NetworkPlugIn<PluginClass>
{
public:

	PeerPlugin(void){};
	virtual ~PeerPlugin(void){};

	virtual const char* name (void){return "PeerPlugin";};	
	virtual void redraw (const float currentTime,
		const float elapsedTime);

	virtual void CreateContent();
	virtual void DeleteContent();
	
};

//-----------------------------------------------------------------------------
template < class PluginClass >
void PeerPlugin<PluginClass>::redraw (const float currentTime,
	const float elapsedTime)
{
	this->m_kGamePlugIn.redraw( currentTime, elapsedTime);
}

//-----------------------------------------------------------------------------
template < class PluginClass >
void PeerPlugin<PluginClass>::CreateContent()
{
	this->m_kGamePlugIn.open();
}
//-----------------------------------------------------------------------------
template < class PluginClass >
void PeerPlugin<PluginClass>::DeleteContent()
{
		this->m_kGamePlugIn.close();
}
