#pragma once
#include "networkplugin.h"

#include "FullyConnectedMesh2.h"
#include "ConnectionGraph2.h"

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

	virtual bool IsConnected() const;
	virtual void StartNetworkSession( void );
private:

	FullyConnectedMesh2 m_kfullyConnectedMeshPlugin;
	ConnectionGraph2 m_kconnectionGraphPlugin;
	
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

	this->m_pNetInterface->AttachPlugin(&this->m_kfullyConnectedMeshPlugin);
	this->m_pNetInterface->AttachPlugin(&this->m_kconnectionGraphPlugin);

	SocketDescriptor sd;
	sd.port=SERVER_PORT;
	bool bStarted(false);
	while( false == bStarted )
	{
		while (SocketLayer::IsPortInUse(sd.port)==true)
			sd.port++;
		if( true == this->m_pNetInterface->Startup(32,100,&sd,1) )
		{
			this->m_pNetInterface->SetMaximumIncomingConnections(32);
			bStarted = true;
		}
	}
	printf("Starting peer at port: %d.\n", sd.port);	
}

//-----------------------------------------------------------------------------
template < class PluginClass >
bool PeerPlugin<PluginClass>::IsConnected() const
{
	return 0 < this->m_pNetInterface->NumberOfConnections();
}