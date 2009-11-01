#ifndef __EMPTYRPCPLUGIN_H__
#define __EMPTYRPCPLUGIN_H__

#include "Tutorial_01/plugins/EmptyPlugin.h"
#include "Tutorial_02/plugins/ClientPlugin.h"
#include "Tutorial_02/plugins/PeerPlugin.h"
#include "EduNetPluginHost.h"
#include "../EduNetPluginSelector.h"

#include "OpenSteerExtras/PlugInArray.h"

// ----------------------------------------------------------------------------
class EmptyServerRpcPlugin : public PeerPlugin<OpenSteer::PlugInArray>,
	public EduNetGames::PluginHost
{
	ET_DECLARE_BASE(PeerPlugin<OpenSteer::PlugInArray>);
public:
	EmptyServerRpcPlugin(bool bAddToRegistry = true);
	virtual ~EmptyServerRpcPlugin(){};

	virtual const char* name (void) const;

	 bool requestInitialSelection (void) const {return true;}
	 virtual float selectionOrderSortKey (void) const { return 1.0f ;}
	 
	virtual bool needRedraw ( void ) const { return false; }
	
	void handleFunctionKeys (int keyNumber);
	
	virtual void open(void);

	virtual bool DoAutoConnect( void ) const
	{
		return false;
	}

	virtual void StartNetworkSession( void );
	virtual void CreateContent( void );
	virtual void DeleteContent( void );

	const char* GetCurrentPluginName( void ) const;
	virtual void SelectPluginByName( const char* pszPluginName ){ };

private:

	void InitializeRpcSystem( void );

	virtual void InitializeGamePlugin( void );

	virtual void InitializeServerPortAndPongCount( void )
	{
		this->m_uiStartPort = 33456;
		this->m_uiPortPongCount = 1;
	}

	PluginSelector m_kPluginSelector;
	PluginSelectorReplicaManager m_kReplicaManager;
	RakNet::RPC3 m_kRpc3Inst;
	
};


// ----------------------------------------------------------------------------
class EmptyClientRpcPlugin : public ClientPlugin<OpenSteer::PlugInArray>,
	public EduNetGames::PluginHost
{
	ET_DECLARE_BASE(ClientPlugin<OpenSteer::PlugInArray>);
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

	virtual OpenSteer::AbstractPlugIn* CreatePluginByName(
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