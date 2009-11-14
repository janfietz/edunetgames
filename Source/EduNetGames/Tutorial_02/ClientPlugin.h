#pragma once
#include "NetworkPlugin.h"

//-----------------------------------------------------------------------------
template < class PluginClass = OpenSteer::Plugin  >
class ClientPlugin :
	public TNetworkPlugin<PluginClass>
{
	ET_DECLARE_BASE(TNetworkPlugin<PluginClass>);
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
	virtual bool Connect();
private:

	virtual bool AddConnectBox( void ){ return true; }

};

//-----------------------------------------------------------------------------
template < class PluginClass >
void ClientPlugin<PluginClass>::redraw (const float currentTime,
	const float elapsedTime)
{
	this->m_kGamePlugin.redraw( currentTime, elapsedTime);
	BaseClass::redraw( currentTime, elapsedTime );
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
	this->StartClientNetworkSession();
}

//-----------------------------------------------------------------------------
template < class PluginClass >
bool ClientPlugin<PluginClass>::Connect( void )
{
	this->ConnectToAddress( this->GetCurrentAddress() );
	return true;
}


