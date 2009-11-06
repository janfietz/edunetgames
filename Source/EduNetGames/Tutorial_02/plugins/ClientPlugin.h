#pragma once
#include "NetworkPlugin.h"

//-----------------------------------------------------------------------------
template < class PluginClass = OpenSteer::Plugin  >
class ClientPlugin :
	public NetworkPlugin<PluginClass>
{
	ET_DECLARE_BASE(NetworkPlugin<PluginClass>);
public:
	ClientPlugin(bool bAddToRegistry = true):
	  BaseClass( bAddToRegistry ){};
	virtual ~ClientPlugin(void){};

	virtual const char* name (void) const {return "ClientPlugin";};

	virtual void redraw (const float currentTime,
		const float elapsedTime);

	virtual void CreateContent( void );
	virtual void DeleteContent( void );

	virtual void StartNetworkSession( void );

};

//-----------------------------------------------------------------------------
template < class PluginClass >
void ClientPlugin<PluginClass>::redraw (const float currentTime,
	const float elapsedTime)
{
	this->m_kGamePlugin.redraw( currentTime, elapsedTime);
}

//-----------------------------------------------------------------------------
template < class PluginClass >
void ClientPlugin<PluginClass>::CreateContent( void )
{
	this->m_kGamePlugin.open();
}
//-----------------------------------------------------------------------------
template < class PluginClass >
void ClientPlugin<PluginClass>::DeleteContent( void )
{
		this->m_kGamePlugin.close();
}

//-----------------------------------------------------------------------------
template < class PluginClass >
void ClientPlugin<PluginClass>::StartNetworkSession( void )
{
	SocketDescriptor sd;
	sd.port = CLIENT_PORT;
	bool bStarted(false);
	while( false == bStarted )
	{
		while (SocketLayer::IsPortInUse(sd.port)==true)
			sd.port++;
		if( true == this->m_pNetInterface->Startup(1,100,&sd,1) )
		{
			this->m_pNetInterface->SetMaximumIncomingConnections(0);
			bStarted = true;
		}
	}
	printf("Starting client at port: %d.\n", sd.port);	
}