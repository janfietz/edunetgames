#ifndef __EMPTYRPCPLUGIN_H__
#define __EMPTYRPCPLUGIN_H__

#include "EduNetConnect/ClientPlugin.h"
#include "EduNetConnect/PeerPlugin.h"
#include "EduNetPluginHost.h"
#include "EduNetPluginSelector.h"

#include "OpenSteerUT/PluginArray.h"

// ----------------------------------------------------------------------------
template < class PluginClass = OpenSteer::Plugin  >
class PluginServerPlugin : public PeerPlugin<PluginClass>,
	public EduNet::PluginHost
{
	ET_DECLARE_BASE(PeerPlugin<PluginClass>);
public:
	PluginServerPlugin(bool bAddToRegistry = true):
	  BaseClass( bAddToRegistry )
	{
		m_bAutoConnect = 0;
	};
	virtual ~PluginServerPlugin(){};

	virtual const char* name (void) const;

	 bool requestInitialSelection (void) const {return true;}
	 virtual float selectionOrderSortKey (void) const { return 1.0f ;}
	 
	virtual bool needRedraw ( void ) const { return false; }	


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

	
	PluginSelectorReplicaManager m_kReplicaManager;
	RakNet::RPC3 m_kRpc3Inst;
	
};

//-----------------------------------------------------------------------------
template < class PluginClass>
void PluginServerPlugin<PluginClass>::StartNetworkSession( void )
{
	BaseClass::StartNetworkSession();
	this->InitializeRpcSystem();
}

//-----------------------------------------------------------------------------
template < class PluginClass>
void PluginServerPlugin<PluginClass>::InitializeRpcSystem( void )
{
	this->m_kRpc3Inst.SetNetworkIDManager(&this->m_kNetworkIdManager);	
	this->m_kReplicaManager.Initialize(&this->m_kRpc3Inst, this, false);
	this->m_pNetInterface->AttachPlugin(&this->m_kReplicaManager);
	this->m_pNetInterface->AttachPlugin(&this->m_kRpc3Inst);

	
}
//-----------------------------------------------------------------------------
template < class PluginClass>
void PluginServerPlugin<PluginClass>::CreateContent ( void )
{
	BaseClass::CreateContent();	
}

//-----------------------------------------------------------------------------
template < class PluginClass>
const char* PluginServerPlugin<PluginClass>::name (void) const {
	const char* pszCurrentPluginName = this->GetCurrentPluginName();
	if(NULL != pszCurrentPluginName )
	{
		return pszCurrentPluginName ;
	}
	return "PluginServerPlugin";
}
//-----------------------------------------------------------------------------
template < class PluginClass>
const char* PluginServerPlugin<PluginClass>::GetCurrentPluginName( void ) const
{	
	return this->m_kGamePlugin.name();	
}

// ----------------------------------------------------------------------------
class PluginClientPlugin : public ClientPlugin<OpenSteer::PluginArray>,
	public EduNet::PluginHost
{
	ET_DECLARE_BASE(ClientPlugin<OpenSteer::PluginArray>);
public:
	PluginClientPlugin(bool bAddToRegistry = true);
	virtual ~PluginClientPlugin(){};

	virtual const char* name (void) const;

	 bool requestInitialSelection (void) const {return true;}
	 virtual float selectionOrderSortKey (void) const { return 2.0f ;}
	 
	virtual bool needRedraw ( void ) const { return false; }
	
	virtual void StartNetworkSession( void );
	virtual void CreateContent( void );
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