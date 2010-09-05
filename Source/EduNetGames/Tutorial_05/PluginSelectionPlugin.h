#ifndef __EMPTYRPCPLUGIN_H__
#define __EMPTYRPCPLUGIN_H__

#include "EduNetConnect/ClientPlugin.h"
#include "EduNetConnect/PeerPlugin.h"
#include "EduNetPluginHost.h"
#include "EduNetPluginSelector.h"

#include "OpenSteerUT/PluginArray.h"

#include "EduNetApplication/EduNetPluginLoadPlugin.h"

//-----------------------------------------------------------------------------
class PluginServerPlugin : public PeerPlugin<OpenSteer::PluginArray>,
	public EduNet::PluginHost
{
	ET_DECLARE_BASE(PeerPlugin<OpenSteer::PluginArray>);
public:
	PluginServerPlugin(bool bAddToRegistry = false):
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

	const char* getCurrentPluginName( void ) const;
	virtual void SelectPluginByName( const char* pszPluginName ){ };

	void addPlugin( OpenSteer::AbstractPlugin* pkPlugin );

private:

	void InitializeRpcSystem( void );

	
	PluginSelectorReplicaManager m_kReplicaManager;
	RakNet::RPC3 m_kRpc3Inst;
	
};

//-----------------------------------------------------------------------------
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

	const char* getCurrentPluginName( void ) const;
	void SelectPluginByName( const char* pszPluginName );

private:

	virtual void InitializeRpcSystem( void );

	virtual OpenSteer::AbstractPlugin* CreatePluginByName(
		const char* pszPluginName )
	{
		return NULL;
	};

	PluginSelectorReplicaManager m_kReplicaManager;
	RakNet::RPC3 m_kRpc3Inst;	
	
};
#endif // __EMPTYRPCPLUGIN_H__