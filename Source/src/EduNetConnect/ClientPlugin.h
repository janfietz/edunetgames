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
	  BaseClass( bAddToRegistry )
	  {
		  this->setIsRemoteObject( true );
		  this->m_kGamePlugin.setIsRemoteObject( true );
	  };
	virtual ~ClientPlugin(void){};

	virtual const char* name (void) const {return "ClientPlugin";};

	virtual void redraw (const float currentTime,
		const float elapsedTime);

	virtual void CreateContent( void );
	virtual void DeleteContent( void );

	virtual void StartNetworkSession( void );
	virtual bool Connect();
private:

	virtual bool addConnectBox( void ){ return true; }

};

//-----------------------------------------------------------------------------
template < class PluginClass >
void ClientPlugin<PluginClass>::redraw (const float currentTime,
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
void ClientPlugin<PluginClass>::CreateContent( void )
{
	// note: in most cases a client plugin will not do anything in the open call
	//       but ! the remote status of the plugin has to be checked by the coder himself
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
	this->ConnectToAddress( this->getCurrentAddress() );
	return true;
}


