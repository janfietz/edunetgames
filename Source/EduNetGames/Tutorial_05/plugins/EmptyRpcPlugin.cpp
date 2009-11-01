#include "EmptyRpcPlugin.h"
#include "EduNet/network/OSReplicaTypes.h"
#include "Tutorial_03/plugins/NetBoidPlugin.h"
#include "OpenSteerExtras/AbstractVehicleGroup.h"

//-----------------------------------------------------------------------------
EmptyServerRpcPlugin::EmptyServerRpcPlugin(bool bAddToRegistry):
	BaseClass( bAddToRegistry )
{
	
}

void EmptyServerRpcPlugin::open(void)
{	
	BaseClass::open();
}

//-----------------------------------------------------------------------------
void EmptyServerRpcPlugin::StartNetworkSession( void )
{
	BaseClass::StartNetworkSession();
	this->InitializeRpcSystem();

	this->InitializeGamePlugin();
}
//-----------------------------------------------------------------------------
void EmptyServerRpcPlugin::InitializeRpcSystem( void )
{
	this->m_kRpc3Inst.SetNetworkIDManager(&this->m_kNetworkIdManager);	
	this->m_kReplicaManager.Initialize(&this->m_kRpc3Inst, this, false);
	this->m_pNetInterface->AttachPlugin(&this->m_kReplicaManager);
	this->m_pNetInterface->AttachPlugin(&this->m_kRpc3Inst);

	m_kPluginSelector.Initialize(&this->m_kRpc3Inst, this);	
}
//-----------------------------------------------------------------------------
void EmptyServerRpcPlugin::CreateContent ( void )
{
	BaseClass::CreateContent();
	this->m_kReplicaManager.Reference( &this->m_kPluginSelector );	
}
//-----------------------------------------------------------------------------
void EmptyServerRpcPlugin::DeleteContent( void )
{
	this->m_kGamePlugIn.removeAllPlugIns();
	BaseClass::DeleteContent();
}
//-----------------------------------------------------------------------------
void EmptyServerRpcPlugin::handleFunctionKeys (int keyNumber)
{
	switch (keyNumber)
    {
    //case 101:  ChangeReplicationInterval(5);         break; //GLUT_KEY_UP
    //case 103:  ChangeReplicationInterval(-5);    break; //GLUT_KEY_DOWN  
	default: BaseClass::handleFunctionKeys(keyNumber);
    }
	
}
//-----------------------------------------------------------------------------
void EmptyServerRpcPlugin::InitializeGamePlugin( void )
{
	this->m_kGamePlugIn.addPlugIn(new EduNetGames::EmptyPlugIn() );
}
//-----------------------------------------------------------------------------
const char* EmptyServerRpcPlugin::name (void) const {
	const char* pszCurrentPluginName = this->GetCurrentPluginName();
	if(NULL != pszCurrentPluginName )
	{
		return pszCurrentPluginName ;
	}
	return "EmptyServerRpcPlugin";
}
//-----------------------------------------------------------------------------
const char* EmptyServerRpcPlugin::GetCurrentPluginName( void ) const
{
	AbstractPlugIn* pkCurrentPlugin = this->m_kGamePlugIn.getPlugIn( 0 );
	if( NULL != pkCurrentPlugin)
	{
		return pkCurrentPlugin->name();
	}
	return NULL;
	
}

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
	this->m_kGamePlugIn.removeAllPlugIns();
	BaseClass::DeleteContent();
}
//-----------------------------------------------------------------------------
void EmptyClientRpcPlugin::SelectPluginByName( 
	const char* pszPluginName )
{
	printf("Create plugin: %s \n", pszPluginName);

	OpenSteer::AbstractPlugIn* pkNewPlugin = this->CreatePluginByName(
		pszPluginName);
	if(NULL != pkNewPlugin)
	{
		OpenSteer::AbstractPlugInPtr spCurrentPlugin( 
			this->m_kGamePlugIn.getPlugIn(0) );
		if(NULL != spCurrentPlugin)
		{
			this->m_kGamePlugIn.removePlugIn(spCurrentPlugin.get());
			spCurrentPlugin->close();
		}

		pkNewPlugin->open();
		this->m_kGamePlugIn.addPlugIn(pkNewPlugin);
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
	AbstractPlugIn* pkCurrentPlugin = this->m_kGamePlugIn.getPlugIn( 0 );
	if( NULL != pkCurrentPlugin)
	{
		return pkCurrentPlugin->name();
	}
	return NULL;
	
}