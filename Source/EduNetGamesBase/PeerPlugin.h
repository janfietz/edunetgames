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

	virtual void CreateContent( void );
	virtual void DeleteContent( void );

	virtual void StartNetworkSession( void );
	virtual void StopNetworkSession( void );
	
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
void PeerPlugin<PluginClass>::CreateContent( void )
{
	this->m_kGamePlugIn.open();
}
//-----------------------------------------------------------------------------
template < class PluginClass >
void PeerPlugin<PluginClass>::DeleteContent( void )
{
		this->m_kGamePlugIn.close();
}

//-----------------------------------------------------------------------------
template < class PluginClass >
void PeerPlugin<PluginClass>::StartNetworkSession( void )
{
	this->m_pNetInterface = RakNetworkFactory::GetRakPeerInterface();

	SocketDescriptor sd;
	sd.port=SERVER_PORT;
	while (SocketLayer::IsPortInUse(sd.port)==true)
			sd.port++;

	printf("Starting peer at port: %d.\n", sd.port);	

	this->m_pNetInterface->Startup(32,100,&sd,1);
	this->m_pNetInterface->SetMaximumIncomingConnections(32);
}

//-----------------------------------------------------------------------------
template < class PluginClass >
void PeerPlugin<PluginClass>::StopNetworkSession( void )
{
	this->m_pNetInterface->Shutdown(100,0);
	RakNetworkFactory::DestroyRakPeerInterface(this->m_pNetInterface);
	printf("Destroyed peer.\n");
}
