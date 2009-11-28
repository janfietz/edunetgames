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
	OpenSteer::Boid* pkBoid = ET_NEW OpenSteer::Boid();
	pkBoid->setParentPlugin( &this->m_kGamePlugin );
	this->m_pkBoidFactory->setMasterVehicle( pkBoid );
	this->m_kGamePlugin.setEntityFactory( this->m_pkBoidFactory );
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

	m_pkConditionReplic = 
		ET_NEW NetBoidConditionReplica(&this->m_kGamePlugin);
	m_kReplicaManager.Reference(m_pkConditionReplic);
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
	m_kReplicaManager.Dereference(m_pkConditionReplic);
	//delete m_pkConditionReplic;	
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

	m_pkConditionReplic = 
		ET_NEW NetBoidConditionReplica(&this->m_kGamePlugin);
	m_kReplicaManager.Reference(m_pkConditionReplic);
}

//-----------------------------------------------------------------------------
void NetClientBoidPlugin::DeleteContent( void )
{	
	m_kReplicaManager.Dereference(m_pkConditionReplic);
	//delete m_pkConditionReplic;	
	BaseClass::DeleteContent();
}

