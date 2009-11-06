#include "EmptyRpcPlugin.h"
#include "EduNet/network/OSReplicaTypes.h"
#include "Tutorial_03/plugins/NetBoidPlugin.h"
#include "OpenSteerExtras/AbstractVehicleGroup.h"




//-----------------------------------------------------------------------------
EmptyClientRpcPlugin::EmptyClientRpcPlugin(bool bAddToRegistry):
	BaseClass( bAddToRegistry )
{
	
}
//-----------------------------------------------------------------------------
void EmptyClientRpcPlugin::StartNetworkSession( void )
{
	BaseClass::StartNetworkSession();
	this->InitializeRpcSystem();	
}

//-----------------------------------------------------------------------------
void EmptyClientRpcPlugin::InitializeRpcSystem( void )
{
	this->m_kRpc3Inst.SetNetworkIDManager(&this->m_kNetworkIdManager);	
	this->m_kReplicaManager.Initialize(&this->m_kRpc3Inst, this, true);	

	this->m_pNetInterface->AttachPlugin(&this->m_kReplicaManager);
	this->m_pNetInterface->AttachPlugin(&this->m_kRpc3Inst);
	
}
//-----------------------------------------------------------------------------
void EmptyClientRpcPlugin::DeleteContent( void )
{
	this->m_kGamePlugin.removeAllPlugins();
	BaseClass::DeleteContent();
}
//-----------------------------------------------------------------------------
void EmptyClientRpcPlugin::SelectPluginByName( 
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
const char* EmptyClientRpcPlugin::name (void) const
{
	const char* pszCurrentPluginName = this->GetCurrentPluginName();
	if(NULL != pszCurrentPluginName )
	{
		return pszCurrentPluginName ;
	}
	return "ClientRpcPlugin";
}
//-----------------------------------------------------------------------------
const char* EmptyClientRpcPlugin::GetCurrentPluginName( void ) const
{
	AbstractPlugin* pkCurrentPlugin = this->m_kGamePlugin.getPlugin( 0 );
	if( NULL != pkCurrentPlugin)
	{
		return pkCurrentPlugin->name();
	}
	return NULL;
	
}