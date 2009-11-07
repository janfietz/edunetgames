#ifndef __EMPTYRPCPLUGIN_H__
#define __EMPTYRPCPLUGIN_H__

#include "Tutorial_01/plugins/EmptyPlugin.h"
#include "Tutorial_02/plugins/ClientPlugin.h"
#include "Tutorial_02/plugins/PeerPlugin.h"
#include "EduNetPluginHost.h"
#include "../EduNetPluginSelector.h"

#include "OpenSteerExtras/PluginArray.h"

// ----------------------------------------------------------------------------
template < class PluginClass = OpenSteer::Plugin  >
class EmptyServerRpcPlugin : public PeerPlugin<PluginClass>,
	public EduNetGames::PluginHost
{
	ET_DECLARE_BASE(PeerPlugin<PluginClass>);
public:
	EmptyServerRpcPlugin(bool bAddToRegistry = true):
	  BaseClass( bAddToRegistry )
	{
		m_bAutoConnect = 0;
	};
	virtual ~EmptyServerRpcPlugin(){};

	virtual const char* name (void) const;

	 bool requestInitialSelection (void) const {return true;}
	 virtual float selectionOrderSortKey (void) const { return 1.0f ;}
	 
	virtual bool needRedraw ( void ) const { return false; }
	
// 	virtual bool DoAutoConnect( void ) const
// 	{
// 		return false;
// 	}

	virtual void StartNetworkSession( void );
	virtual void CreateContent( void );

	const char* GetCurrentPluginName( void ) const;
	virtual void SelectPluginByName( const char* pszPluginName ){ };

private:

	void InitializeRpcSystem( void );

	virtual void InitializeServerPortAndPongCount( void )
	{
		this->m_uiStartPort = 33456;
		this->m_uiPortPongCount = 1;
	}

	PluginSelector m_kPluginSelector;
	PluginSelectorReplicaManager m_kReplicaManager;
	RakNet::RPC3 m_kRpc3Inst;
	
};

//-----------------------------------------------------------------------------
template < class PluginClass>
void EmptyServerRpcPlugin<PluginClass>::StartNetworkSession( void )
{
	BaseClass::StartNetworkSession();
	this->InitializeRpcSystem();
}

//-----------------------------------------------------------------------------
template < class PluginClass>
void EmptyServerRpcPlugin<PluginClass>::InitializeRpcSystem( void )
{
	this->m_kRpc3Inst.SetNetworkIDManager(&this->m_kNetworkIdManager);	
	this->m_kReplicaManager.Initialize(&this->m_kRpc3Inst, this, false);
	this->m_pNetInterface->AttachPlugin(&this->m_kReplicaManager);
	this->m_pNetInterface->AttachPlugin(&this->m_kRpc3Inst);

	m_kPluginSelector.Initialize(&this->m_kRpc3Inst, this);	
}
//-----------------------------------------------------------------------------
template < class PluginClass>
void EmptyServerRpcPlugin<PluginClass>::CreateContent ( void )
{
	BaseClass::CreateContent();
	this->m_kReplicaManager.Reference( &this->m_kPluginSelector );	
}

//-----------------------------------------------------------------------------
template < class PluginClass>
const char* EmptyServerRpcPlugin<PluginClass>::name (void) const {
	const char* pszCurrentPluginName = this->GetCurrentPluginName();
	if(NULL != pszCurrentPluginName )
	{
		return pszCurrentPluginName ;
	}
	return "EmptyServerRpcPlugin";
}
//-----------------------------------------------------------------------------
template < class PluginClass>
const char* EmptyServerRpcPlugin<PluginClass>::GetCurrentPluginName( void ) const
{	
	return this->m_kGamePlugin.name();	
}

// ----------------------------------------------------------------------------
class EmptyClientRpcPlugin : public ClientPlugin<OpenSteer::PluginArray>,
	public EduNetGames::PluginHost
{
	ET_DECLARE_BASE(ClientPlugin<OpenSteer::PluginArray>);
public:
	EmptyClientRpcPlugin(bool bAddToRegistry = true);
	virtual ~EmptyClientRpcPlugin(){};

	virtual const char* name (void) const;

	 bool requestInitialSelection (void) const {return true;}
	 virtual float selectionOrderSortKey (void) const { return 2.0f ;}
	 
	virtual bool needRedraw ( void ) const { return false; }
	
	virtual void StartNetworkSession( void );
	virtual void DeleteContent( void );

	const char* GetCurrentPluginName( void ) const;
	void SelectPluginByName( const char* pszPluginName );

private:

	virtual void InitializeRpcSystem( void );

	virtual OpenSteer::AbstractPlugin* CreatePluginByName(
		const char* pszPluginName )
	{
		return NULL;
	};

	virtual void InitializeServerPortAndPongCount( void )
	{
		this->m_uiStartPort = 33456;
		this->m_uiPortPongCount = 1;
	}

	PluginSelectorReplicaManager m_kReplicaManager;
	RakNet::RPC3 m_kRpc3Inst;	
	
};
#endif // __EMPTYRPCPLUGIN_H__