#include "NetBoidPlugin.h"

#include "EduNetCommon/EduNetDraw.h"
#include "EduNetConnect/OSReplicaTypes.h"
#include "OpenSteerUT/AbstractVehicleGroup.h"

#include "NetBoidConditionReplica.h"

//-----------------------------------------------------------------------------
NetPeerBoidPlugin::NetPeerBoidPlugin(bool bAddToRegistry):
	BaseClass( bAddToRegistry )
{
	
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
	this->m_pNetInterface->AttachPlugin(this->m_pkReplicaManager);
}

//-----------------------------------------------------------------------------
void NetPeerBoidPlugin::CreateContent( void )
{
	BaseClass::CreateContent();

	this->m_pkConditionReplic = 
		ET_NEW NetBoidConditionReplica(&this->m_kGamePlugin);
	this->m_pkReplicaManager->Reference(m_pkConditionReplic);
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
	this->m_pkReplicaManager->Dereference(m_pkConditionReplic);
	// TODO: ref counting needed for this object
	// note: set to NULL
	// this->m_pkConditionReplic = NULL;
	BaseClass::DeleteContent();
}
//-----------------------------------------------------------------------------
void NetPeerBoidPlugin::open( void )
{
	this->m_pkReplicaManager = ET_NEW NetBoidReplicaManager();
	this->setGamePluginReplicaManager( this->m_pkReplicaManager );
	this->m_pkReplicaManager->setPlugin( &this->m_kGamePlugin );

	this->m_pkBoidFactory = ET_NEW NetBoidReplicaFactory(this->m_pkReplicaManager);
	this->m_kGamePlugin.setEntityFactory( this->m_pkBoidFactory );

	BaseClass::open();
}
//-----------------------------------------------------------------------------
void NetPeerBoidPlugin::close( void )
{
	BaseClass::close();

	this->setGamePluginReplicaManager( NULL );
	this->m_pkReplicaManager->setPlugin( NULL);
	this->m_kGamePlugin.setEntityFactory( NULL);
	
	ET_SAFE_DELETE(this->m_pkBoidFactory );
	ET_SAFE_DELETE(this->m_pkReplicaManager )

}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
NetClientBoidPlugin::NetClientBoidPlugin(bool bAddToRegistry):
	BaseClass(bAddToRegistry)
{
}

NetClientBoidPlugin::~NetClientBoidPlugin()
{
	//ET_SAFE_DELETE(this->m_pkConditionReplic);
}

//-----------------------------------------------------------------------------
void NetClientBoidPlugin::StartNetworkSession( void )
{
	BaseClass::StartNetworkSession();
	this->m_pNetInterface->AttachPlugin(this->m_pkReplicaManager);
}

//-----------------------------------------------------------------------------
void NetClientBoidPlugin::CreateContent( void )
{
	BaseClass::CreateContent();

	this->m_pkConditionReplic = 
		ET_NEW NetBoidConditionReplica(&this->m_kGamePlugin);
	this->m_pkReplicaManager->Reference(this->m_pkConditionReplic);
}

//-----------------------------------------------------------------------------
void NetClientBoidPlugin::DeleteContent( void )
{	
	this->m_pkReplicaManager->Dereference( this->m_pkConditionReplic );
	// TODO: ref counting needed for this object
	// note: set to NULL
	this->m_pkConditionReplic = NULL;
	BaseClass::DeleteContent();
}
//-----------------------------------------------------------------------------
void NetClientBoidPlugin::open( void )
{
	this->m_pkReplicaManager = ET_NEW NetBoidReplicaManager();
	this->setGamePluginReplicaManager( this->m_pkReplicaManager );
	this->m_pkReplicaManager->setPlugin( &this->m_kGamePlugin );

	this->m_kGamePlugin.setEntityFactory( NULL);

	BaseClass::open();
}
//-----------------------------------------------------------------------------
void NetClientBoidPlugin::close( void )
{
	BaseClass::close();

	this->setGamePluginReplicaManager( NULL );
	this->m_pkReplicaManager->setPlugin( NULL);
	ET_SAFE_DELETE(this->m_pkReplicaManager )
}