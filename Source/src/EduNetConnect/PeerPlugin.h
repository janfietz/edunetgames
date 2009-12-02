#pragma once
#include "NetworkPlugin.h"

template < class PluginClass = OpenSteer::Plugin  >
class PeerPlugin :
	public TNetworkPlugin<PluginClass>
{
	ET_DECLARE_BASE(TNetworkPlugin<PluginClass>);
public:

	PeerPlugin(bool bAddToRegistry = true):
	  BaseClass( bAddToRegistry )
	  {

	  };
	virtual ~PeerPlugin(void){};

	virtual const char* name (void) const {return "PeerPlugin";};	
	virtual void redraw (const float currentTime,
		const float elapsedTime);

	virtual void CreateContent( void );
	virtual void DeleteContent( void );
	
	virtual void StartNetworkSession( void );

private:
	virtual bool HasIdAuthority( void ) const {return true;}

	FullyConnectedMesh2 m_kfullyConnectedMeshPlugin;
	ConnectionGraph2 m_kconnectionGraphPlugin;
	
};

//-----------------------------------------------------------------------------
template < class PluginClass >
void PeerPlugin<PluginClass>::redraw (const float currentTime,
	const float elapsedTime)
{
	if( false == this->isVisible() )
	{
		return;
	}
	this->m_kGamePlugin.redraw( currentTime, elapsedTime);
	BaseClass::redraw( currentTime, elapsedTime );
}

//-----------------------------------------------------------------------------
template < class PluginClass >
void PeerPlugin<PluginClass>::CreateContent( void )
{
	this->m_kGamePlugin.open();
}
//-----------------------------------------------------------------------------
template < class PluginClass >
void PeerPlugin<PluginClass>::DeleteContent( void )
{
	this->m_kGamePlugin.close();
}

//-----------------------------------------------------------------------------
template < class PluginClass >
void PeerPlugin<PluginClass>::StartNetworkSession( void )
{
	this->m_pNetInterface->AttachPlugin(&this->m_kfullyConnectedMeshPlugin);
	this->m_pNetInterface->AttachPlugin(&this->m_kconnectionGraphPlugin);

	SocketDescriptor& sd = this->m_kSocketDescriptor;
	sd.port = this->m_uiStartPort;
	if( true == this->StartupNetworkSession( sd, 32, 32 ) )
	{
		this->m_eNetworkSessionType = ENetworkSessionType_Peer;
	}
	printf("Starting peer at port: %d.\n", sd.port);	
}

