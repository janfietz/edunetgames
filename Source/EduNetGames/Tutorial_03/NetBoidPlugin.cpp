#include "NetBoidPlugin.h"

#include "EduNetCommon/EduNetDraw.h"
#include "EduNetConnect/OSReplicaTypes.h"
#include "OpenSteerUT/AbstractVehicleGroup.h"

#include "NetBoidConditionReplica.h"

//-----------------------------------------------------------------------------
NetPeerBoidPlugin::NetPeerBoidPlugin(bool bAddToRegistry):
	BaseClass( bAddToRegistry )
{
	this->setGamePluginReplicaManager( &this->m_kReplicaManager );
	this->m_kReplicaManager.setPlugin( &this->m_kGamePlugin );

	this->m_pkBoidFactory = ET_NEW NetBoidReplicaFactory(&this->m_kReplicaManager);
	this->m_kGamePlugin.setEntityFactory( this->m_pkBoidFactory );
}

NetPeerBoidPlugin::~NetPeerBoidPlugin()
{
	ET_SAFE_DELETE( this->m_pkBoidFactory );
//	ET_SAFE_DELETE( this->m_pkConditionReplic );
}

//-----------------------------------------------------------------------------
void NetPeerBoidPlugin::StartNetworkSession( void )
{
	BaseClass::StartNetworkSession();
	this->m_pNetInterface->AttachPlugin(&this->m_kReplicaManager);
}

//-----------------------------------------------------------------------------
void NetPeerBoidPlugin::CreateContent( void )
{
	BaseClass::CreateContent();

	this->m_pkConditionReplic = 
		ET_NEW NetBoidConditionReplica(&this->m_kGamePlugin);
	this->m_kReplicaManager.Reference(m_pkConditionReplic);
}

//-----------------------------------------------------------------------------
void NetPeerBoidPlugin::handleFunctionKeys (int keyNumber)
{
}

//-----------------------------------------------------------------------------
void NetPeerBoidPlugin::initGui( void* pkUserdata ) 
{
	BaseClass::initGui( pkUserdata );
};


//-----------------------------------------------------------------------------
void NetPeerBoidPlugin::DeleteContent( void )
{	
	this->m_kReplicaManager.Dereference(m_pkConditionReplic);
	// TODO: ref counting needed for this object
	// note: set to NULL
	// this->m_pkConditionReplic = NULL;
	BaseClass::DeleteContent();
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
NetClientBoidPlugin::NetClientBoidPlugin(bool bAddToRegistry):
	BaseClass(bAddToRegistry)
{
	this->setGamePluginReplicaManager( &this->m_kReplicaManager );
	this->m_kReplicaManager.setPlugin( &this->m_kGamePlugin );	
	this->m_kGamePlugin.setEntityFactory( NULL  );
}

NetClientBoidPlugin::~NetClientBoidPlugin()
{
	//ET_SAFE_DELETE(this->m_pkConditionReplic);
}

//-----------------------------------------------------------------------------
void NetClientBoidPlugin::StartNetworkSession( void )
{
	BaseClass::StartNetworkSession();
	this->m_pNetInterface->AttachPlugin(&this->m_kReplicaManager);
}

//-----------------------------------------------------------------------------
void NetClientBoidPlugin::CreateContent( void )
{
	BaseClass::CreateContent();

	this->m_pkConditionReplic = 
		ET_NEW NetBoidConditionReplica(&this->m_kGamePlugin);
	this->m_kReplicaManager.Reference(this->m_pkConditionReplic);
}

//-----------------------------------------------------------------------------
void NetClientBoidPlugin::DeleteContent( void )
{	
	this->m_kReplicaManager.Dereference( this->m_pkConditionReplic );
	// TODO: ref counting needed for this object
	// note: set to NULL
	this->m_pkConditionReplic = NULL;
	BaseClass::DeleteContent();
}

