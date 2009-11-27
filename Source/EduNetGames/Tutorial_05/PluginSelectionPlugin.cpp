#include "PluginSelectionPlugin.h"

//-----------------------------------------------------------------------------
PluginClientPlugin::PluginClientPlugin(bool bAddToRegistry):
	BaseClass( bAddToRegistry )
{
	
}
//-----------------------------------------------------------------------------
void PluginClientPlugin::StartNetworkSession( void )
{
	BaseClass::StartNetworkSession();
	this->InitializeRpcSystem();	
}

//-----------------------------------------------------------------------------
void PluginClientPlugin::InitializeRpcSystem( void )
{
	this->m_kRpc3Inst.SetNetworkIDManager( this->m_pkNetworkIdManager );	
	this->m_kReplicaManager.Initialize(&this->m_kRpc3Inst, this, true);	

	this->m_pNetInterface->AttachPlugin(&this->m_kReplicaManager);
	this->m_pNetInterface->AttachPlugin(&this->m_kRpc3Inst);

	
	
}
//-----------------------------------------------------------------------------
void PluginClientPlugin::CreateContent( void )
{
	BaseClass::CreateContent();
	PluginSelector* pkNewSelector = new PluginSelector();
	pkNewSelector->Initialize(&this->m_kRpc3Inst, this);	
	this->m_kReplicaManager.Reference( pkNewSelector );	
}
//-----------------------------------------------------------------------------
void PluginClientPlugin::DeleteContent( void )
{
	this->m_kGamePlugin.removeAllPlugins();
	BaseClass::DeleteContent();
}
//-----------------------------------------------------------------------------
void PluginClientPlugin::SelectPluginByName( 
	const char* pszPluginName )
{
	printf("Create plugin: %s \n", pszPluginName);

	OpenSteer::AbstractPlugin* pkNewPlugin = this->CreatePluginByName(
		pszPluginName);
	if(NULL != pkNewPlugin)
	{
		OpenSteer::AbstractPluginPtr spCurrentPlugin( 
			this->m_kGamePlugin.getPlugin(0) );
		if(NULL != spCurrentPlugin)
		{
			this->m_kGamePlugin.removePlugin(spCurrentPlugin.get());
			spCurrentPlugin->close();
		}

		pkNewPlugin->open();
		this->m_kGamePlugin.addPlugin(pkNewPlugin);
	}

}
//-----------------------------------------------------------------------------
const char* PluginClientPlugin::name (void) const
{
	const char* pszCurrentPluginName = this->getCurrentPluginName();
	if(NULL != pszCurrentPluginName )
	{
		return pszCurrentPluginName ;
	}
	return "ClientRpcPlugin";
}
//-----------------------------------------------------------------------------
const char* PluginClientPlugin::getCurrentPluginName( void ) const
{
	AbstractPlugin* pkCurrentPlugin = this->m_kGamePlugin.getPlugin( 0 );
	if( NULL != pkCurrentPlugin)
	{
		return pkCurrentPlugin->pluginName();
	}
	return NULL;
	
}