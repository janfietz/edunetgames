#include "PluginSelectionPlugin.h"

//-----------------------------------------------------------------------------
void EduNetConnect::queryConnectionsSettings( ConnectSettings& kSettings )
{
	kSettings.uiClientStartPort = CLIENT_PORT;
	kSettings.uiServerStartPort = 33456;
	kSettings.sessionPassword = "Tutorial5";
	kSettings.uiPortPongCount = 10;
}


//-----------------------------------------------------------------------------
void PluginServerPlugin::StartNetworkSession( void )
{
	BaseClass::StartNetworkSession();
	this->InitializeRpcSystem();
}

//-----------------------------------------------------------------------------
void PluginServerPlugin::InitializeRpcSystem( void )
{
	this->m_kRpc3Inst.SetNetworkIDManager( this->m_pkNetworkIdManager );	
	this->m_kReplicaManager.Initialize(&this->m_kRpc3Inst, this, false);
	this->m_pNetInterface->AttachPlugin(&this->m_kReplicaManager);
	this->m_pNetInterface->AttachPlugin(&this->m_kRpc3Inst);


}
//-----------------------------------------------------------------------------
void PluginServerPlugin::CreateContent ( void )
{
	BaseClass::CreateContent();	
}

//-----------------------------------------------------------------------------
const char* PluginServerPlugin::name (void) const {
	const char* pszCurrentPluginName = this->getCurrentPluginName();
	if(NULL != pszCurrentPluginName )
	{
		return pszCurrentPluginName ;
	}
	return "PluginServerPlugin";
}
//-----------------------------------------------------------------------------
const char* PluginServerPlugin::getCurrentPluginName( void ) const
{	
	AbstractPlugin* pkPlugin = this->m_kGamePlugin.getPlugin( 0 );
	if (NULL != pkPlugin)
	{
		return pkPlugin->pluginName();	
	}
	return "NoName";
}


void PluginServerPlugin::addPlugin( OpenSteer::AbstractPlugin* pkPlugin )
{
	this->m_kGamePlugin.addPlugin( pkPlugin );
}

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
	PluginSelector* pkNewSelector = ET_NEW PluginSelector();
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
		// temporary fix
		OpenSteer::SimpleVehicle::selectedVehicle = NULL;
		OpenSteer::SimpleVehicle::nearestMouseVehicle = NULL;

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