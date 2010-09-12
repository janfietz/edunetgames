#ifndef __EMPTYRPCPLUGIN_H__
#define __EMPTYRPCPLUGIN_H__

#include "EduNetConnect/ClientPlugin.h"
#include "EduNetConnect/PeerPlugin.h"
#include "EduNetPluginSelector.h"

#include "OpenSteerUT/PluginArray.h"

//-----------------------------------------------------------------------------
class PluginServerPlugin : public PeerPlugin<OpenSteer::PluginArray>,
	public OpenSteer::VirtualPluginSelector
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
	virtual void selectPluginByName( const char* pszPluginName ){ };

	void addPlugin( OpenSteer::AbstractPlugin* pkPlugin );

private:

	void InitializeRpcSystem( void );

	
	PluginSelectorReplicaManager m_kReplicaManager;
	RakNet::RPC3 m_kRpc3Inst;
	
};

//-----------------------------------------------------------------------------
class ClientPluginSelectionPlugin : public ClientPlugin<OpenSteer::PluginArray>,
	public OpenSteer::VirtualPluginSelector
{
	ET_DECLARE_BASE(ClientPlugin<OpenSteer::PluginArray>);
public:
	ClientPluginSelectionPlugin(bool bAddToRegistry = true);
	virtual ~ClientPluginSelectionPlugin(){};

	virtual const char* name (void) const;

	bool requestInitialSelection (void) const {return true;}
	virtual float selectionOrderSortKey (void) const { return 2.0f ;}
	 
	virtual bool needRedraw ( void ) const { return false; }
	
	virtual void StartNetworkSession( void );
	virtual void CreateContent( void );
	virtual void DeleteContent( void );

	const char* getCurrentPluginName( void ) const;
	void selectPluginByName( const char* pszPluginName );

protected:
	virtual OpenSteer::AbstractPlugin* createPluginByName(
		const char* pszPluginName )
	{
		return NULL;
	};

private:

	virtual void InitializeRpcSystem( void );

	PluginSelectorReplicaManager m_kReplicaManager;
	RakNet::RPC3 m_kRpc3Inst;	
	
};
#endif // __EMPTYRPCPLUGIN_H__