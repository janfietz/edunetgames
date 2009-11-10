#include "NetPedestrianReplicaConnection.h"
#include "NetPedestrian.h"
#include "NetPedestrianReplica.h"
#include "NetPedestrianPlugin.h"

#include "OpenSteerExtras/PluginArray.h"
#include "Tutorial_01/EmptyPlugin.h"
#include "Tutorial_02/ClientPlugin.h"
#include "Tutorial_02/PeerPlugin.h"

//-----------------------------------------------------------------------------
class NetPedestrian* NetPedestrianFactory::CreateNetPedestrian( osProximityDatabase& pd )
{
	return new NetPedestrian( pd );
};

//-----------------------------------------------------------------------------
void NetPedestrianFactory::DestroyNetPedestrian( const class NetPedestrian* boid )
{
	delete boid;
};

//-----------------------------------------------------------------------------
RakNet::Replica3* NetPedestrianReplicaConnection::AllocReplica(
	RakNet::BitStream *allocationId,
	RakNet::ReplicaManager3 *replicaManager3)
{
	static NetPedestrianReplica kReplica;
	RakNet::RakString typeName;
	allocationId->Read(typeName);
	if ( typeName== kReplica.GetName() ){
		NetPedestrianReplica* pkNewReplica = new NetPedestrianReplica( this->m_pNetPedestrianPlugin  );
		// 		OpenSteer::AbstractVehicleGroup kVG( this->m_pNetPedestrianPlugin->allVehicles() );
		// 		kVG.addVehicle( pkNewReplica->AccessVehicle() );
		this->m_pNetPedestrianPlugin->addPedestrianToCrowd( pkNewReplica->AccessVehicle() );
		return pkNewReplica; 
	}
	return 0;
}

//-----------------------------------------------------------------------------
NetPedestrian* NetPedestrianReplicaFactory::CreateNetPedestrian( 
	OpenSteer::ProximityDatabase& pd )	
{	
	NetPedestrianReplica* pkNewReplica = new NetPedestrianReplica( pd );		
	this->m_pkReplicaManager->Reference(pkNewReplica);

	this->m_uidMap.Set(pkNewReplica->AccessVehicle()->getEntityId(), pkNewReplica);
	return pkNewReplica->AccessVehicle();
}

//-----------------------------------------------------------------------------
void NetPedestrianReplicaFactory::DestroyNetPedestrian( const NetPedestrian* pkNetPedestrian)
{	
	size_t uiNetPedestrianAddress = pkNetPedestrian->getEntityId();	
	if(true == this->m_uidMap.Has(uiNetPedestrianAddress))
	{
		RakNet::Replica3* pReplicaObject = this->m_uidMap.Get( uiNetPedestrianAddress);		
		this->m_pkReplicaManager->BroadcastDestruction( pReplicaObject, UNASSIGNED_SYSTEM_ADDRESS);
		this->m_uidMap.Set(uiNetPedestrianAddress, NULL);
		delete pReplicaObject;
	}

}





//-----------------------------------------------------------------------------
class PedestrianClientServerPlugin : public OpenSteer::PluginArray
{
	ET_DECLARE_BASE(OpenSteer::PluginArray);
public:

	PedestrianClientServerPlugin();
	virtual ~PedestrianClientServerPlugin();

	//---------------------------------------------------------------------
	// interface AbstractPlugin
	virtual const char *name(void) const; 

	virtual void initGui( void* pkUserdata );
};



typedef PeerPlugin<NetPedestrianPlugin> TPedestrianPeerPlugin;
typedef ClientPlugin<NetPedestrianPlugin> TPedestrianClientPlugin;

//-----------------------------------------------------------------------------
class PedestrianPeerPlugin : public TPedestrianPeerPlugin
{
	ET_DECLARE_BASE(TPedestrianPeerPlugin)
public:
	PedestrianPeerPlugin( bool bAddToRegistry = true ):
	BaseClass( bAddToRegistry )
	{
		this->m_kReplicaManager.SetAutoSerializeInterval(
			this->m_kReplicationSettings.interval);

		this->m_kReplicaManager.SetPlugin(&this->m_kGamePlugin);

		this->m_pkNetPedestrianFactory = new NetPedestrianReplicaFactory( &this->m_kReplicaManager );	
		this->m_kGamePlugin.setNetPedestrianFactory( this->m_pkNetPedestrianFactory );
	}
	virtual const char* name() const { return "PedestrianPeerPlugin"; };

	//-----------------------------------------------------------------------------
	void StartNetworkSession( void )
	{
		BaseClass::StartNetworkSession();
		this->m_pNetInterface->AttachPlugin(&this->m_kReplicaManager);
	}

	//-----------------------------------------------------------------------------
	void CreateContent( void )
	{
		BaseClass::CreateContent();

	}

	//-----------------------------------------------------------------------------
	void handleFunctionKeys (int keyNumber)
	{
		switch (keyNumber)
		{
		case 101:  ChangeReplicationInterval(5);         break; //GLUT_KEY_UP
		case 103:  ChangeReplicationInterval(-5);    break; //GLUT_KEY_DOWN  
		default: BaseClass::handleFunctionKeys(keyNumber);
		}	
	}

	//-----------------------------------------------------------------------------
	void ChangeReplicationInterval( RakNetTime additionalTime )
	{
		m_kReplicationSettings.interval += additionalTime;
		//clamp interval
		if( 5 > m_kReplicationSettings.interval )
		{
			m_kReplicationSettings.interval = 5;
		}
		printf("Changed replication interval to: %d ms\n",
			m_kReplicationSettings.interval);

		this->m_kReplicaManager.SetAutoSerializeInterval(
			this->m_kReplicationSettings.interval);
	}

	//-----------------------------------------------------------------------------
	void DeleteContent( void )
	{	
		BaseClass::DeleteContent();
	}

private:
	struct ReplicationParams
	{
		ReplicationParams():
	interval(30){}		
	RakNetTime interval;
	};
//	void ChangeReplicationInterval( RakNetTime additionalTime );

	NetPedestrianReplicaFactory* m_pkNetPedestrianFactory;
	NetPedestrianReplicaManager m_kReplicaManager;

	ReplicationParams m_kReplicationSettings;
};

//-----------------------------------------------------------------------------
class PedestrianClientPlugin : public TPedestrianClientPlugin
{
	ET_DECLARE_BASE(TPedestrianClientPlugin)
public:
	PedestrianClientPlugin( bool bAddToRegistry = true ):
	BaseClass( bAddToRegistry )
	{
		this->m_kReplicaManager.SetPlugin(&this->m_kGamePlugin);

		this->m_pkBoidFactory = new NetPedestrianDummyFactory(&this->m_kReplicaManager);	
		this->m_kGamePlugin.setNetPedestrianFactory( this->m_pkBoidFactory );
	}

	virtual const char* name() const { return "PedestrianClientPlugin"; };

	//-----------------------------------------------------------------------------
	void StartNetworkSession( void )
	{
		BaseClass::StartNetworkSession();
		this->m_pNetInterface->AttachPlugin(&this->m_kReplicaManager);
	}

	//-----------------------------------------------------------------------------
	void CreateContent( void )
	{
		BaseClass::CreateContent();
	}

	//-----------------------------------------------------------------------------
	void DeleteContent( void )
	{	
		BaseClass::DeleteContent();
	}
private:

	NetPedestrianReplicaFactory* m_pkBoidFactory;
	NetPedestrianReplicaManager m_kReplicaManager;
};

//-----------------------------------------------------------------------------
PedestrianClientServerPlugin::PedestrianClientServerPlugin()
{
	OpenSteer::Plugin::addToRegistry(this);

	this->addPlugin( new PedestrianPeerPlugin( false ) );
	this->addPlugin( new PedestrianClientPlugin( false ) );
}

//-----------------------------------------------------------------------------
PedestrianClientServerPlugin::~PedestrianClientServerPlugin()
{

}

//-----------------------------------------------------------------------------
const char* PedestrianClientServerPlugin::name(void) const
{
	return "PedestrianClientServerPlugin";
}

//-----------------------------------------------------------------------------
void PedestrianClientServerPlugin::initGui( void* pkUserdata ) 
{
	BaseClass::initGui( pkUserdata );
	GLUI* glui = ::getRootGLUI();
	GLUI_Panel* pluginPanel = static_cast<GLUI_Panel*>( pkUserdata );

	// 	glui->add_button_to_panel( pluginPanel, "Connect Client" );
	// 	glui->add_button_to_panel( pluginPanel, "Connect Server" );
};


PedestrianPeerPlugin gPedestrianPeerPlugin( true );
PedestrianClientPlugin gPedestrianClientPlugin( true );

PedestrianClientServerPlugin gClientServerPlugin;

EduNetGames::EmptyPlugin gEmptyPlugin;