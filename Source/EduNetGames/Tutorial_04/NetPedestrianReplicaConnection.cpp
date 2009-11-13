#include "NetPedestrianReplicaConnection.h"
#include "NetPedestrian.h"
#include "NetPedestrianReplica.h"
#include "NetPedestrianPlugin.h"

#include "OpenSteerUT/PluginArray.h"
//#include "OpenSteerUT/EmptyPlugin.h"
#include "OpenSteerUT/GridPlugin.h"
#include "EduNetConnect/ClientPlugin.h"
#include "EduNetConnect/PeerPlugin.h"

#include "EduNetCommon/EduNetDraw.h"

//-----------------------------------------------------------------------------
class OpenSteer::AbstractVehicle* NetPedestrianFactory::CreateNetPedestrian( osProximityDatabase& pd )
{
	return new NetPedestrian( pd );
};

//-----------------------------------------------------------------------------
void NetPedestrianFactory::DestroyNetPedestrian( const class OpenSteer::AbstractVehicle* boid )
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
OpenSteer::AbstractVehicle* NetPedestrianReplicaFactory::CreateNetPedestrian( 
	OpenSteer::ProximityDatabase& pd )	
{	
	NetPedestrianReplica* pkNewReplica = new NetPedestrianReplica( pd );		
	this->m_pkReplicaManager->Reference(pkNewReplica);

	OpenSteer::AbstractVehicle* pkVehicle = pkNewReplica->AccessVehicle();
	this->m_uidMap.Set(pkVehicle->getEntityId(), pkNewReplica);
	return pkNewReplica->AccessVehicle();
}

//-----------------------------------------------------------------------------
void NetPedestrianReplicaFactory::DestroyNetPedestrian( const OpenSteer::AbstractVehicle* pkNetPedestrian)
{	
	const OpenSteer::InstanceTracker::Id uiEntityId = pkNetPedestrian->getEntityId();	
	if(true == this->m_uidMap.Has( uiEntityId ))
	{
		RakNet::Replica3* pReplicaObject = this->m_uidMap.Get( uiEntityId);		
		this->m_pkReplicaManager->BroadcastDestruction( pReplicaObject, UNASSIGNED_SYSTEM_ADDRESS);
		this->m_uidMap.Set( uiEntityId, NULL );
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

	OS_IMPLEMENT_CLASSNAME( PedestrianClientServerPlugin )
	//---------------------------------------------------------------------
	// interface AbstractPlugin
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
	}
	OS_IMPLEMENT_CLASSNAME( PedestrianPeerPlugin )
	virtual const char* name() const { return this->getClassName(); };

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

		this->UpdateReplicationValue();
	}

	//-----------------------------------------------------------------------------
	static void changeReplicationDelay(GLUI_Control* pkControl )
	{
		PedestrianPeerPlugin* pkPlugin = (PedestrianPeerPlugin*)pkControl->ptr_val;
		pkPlugin->UpdateReplicationValue();
	}

	//-----------------------------------------------------------------------------
	void UpdateReplicationValue( void )
	{
		this->m_kReplicaManager.SetAutoSerializeInterval(
			this->m_kReplicationSettings.interval);
	}

	//-----------------------------------------------------------------------------
	virtual void initGui( void* pkUserdata ) 
	{
		BaseClass::initGui( pkUserdata );
		GLUI* glui = ::getRootGLUI();
		GLUI_Panel* pluginPanel = static_cast<GLUI_Panel*>( pkUserdata );

		GLUI_Spinner* repSpinner =
			glui->add_spinner_to_panel(pluginPanel, "ReplicationDelay", GLUI_SPINNER_INT, &m_kReplicationSettings.interval, -1, changeReplicationDelay);
		repSpinner->set_int_limits(5, 1000000);
		repSpinner->set_ptr_val( this );
	};

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
	}

	OS_IMPLEMENT_CLASSNAME( PedestrianClientPlugin )
	virtual const char* name() const { return this->getClassName(); };

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

	this->addPlugin( new OpenSteer::GridPlugin() );
	this->addPlugin( new PedestrianPeerPlugin( false ) );
	this->addPlugin( new PedestrianClientPlugin( false ) );
}

//-----------------------------------------------------------------------------
PedestrianClientServerPlugin::~PedestrianClientServerPlugin()
{

}

//-----------------------------------------------------------------------------
void PedestrianClientServerPlugin::initGui( void* pkUserdata ) 
{
	BaseClass::initGui( pkUserdata );
	GLUI* glui = ::getRootGLUI();
	GLUI_Panel* pluginPanel = static_cast<GLUI_Panel*>( pkUserdata );
};

//-----------------------------------------------------------------------------
class OfflinePedestrianPlugin : public OpenSteer::PluginArray
{
	ET_DECLARE_BASE(OpenSteer::PluginArray)
public:
	OfflinePedestrianPlugin( bool bAddToRegistry = true ):
	BaseClass( bAddToRegistry )
	{
		this->addPlugin( new OpenSteer::GridPlugin() );
		this->addPlugin( new NetPedestrianPlugin( false ) );
	}

	OS_IMPLEMENT_CLASSNAME( OfflinePedestrianPlugin )
};

OfflinePedestrianPlugin gPedestrianPlugin;

//-----------------------------------------------------------------------------
class PedestrianRenderClientPlugin : public OpenSteer::PluginArrayPluginMixin<PedestrianClientPlugin>
{
	ET_DECLARE_BASE( PluginArrayPluginMixin<PedestrianClientPlugin> )
public:
	PedestrianRenderClientPlugin( bool bAddToRegistry = true ):BaseClass( bAddToRegistry ) 
	{
		this->addPlugin( new OpenSteer::GridPlugin() );
	};
	virtual ~PedestrianRenderClientPlugin() {};

	OS_IMPLEMENT_CLASSNAME( PedestrianRenderClientPlugin )
};

//-----------------------------------------------------------------------------
class PedestrianRenderPeerPlugin : public OpenSteer::PluginArrayPluginMixin<PedestrianPeerPlugin>
{
	ET_DECLARE_BASE( PluginArrayPluginMixin<PedestrianPeerPlugin> )
public:
	PedestrianRenderPeerPlugin( bool bAddToRegistry = true ):BaseClass( bAddToRegistry ) 
	{
		this->addPlugin( new OpenSteer::GridPlugin() );
	};
	virtual ~PedestrianRenderPeerPlugin() {};

	OS_IMPLEMENT_CLASSNAME( PedestrianRenderPeerPlugin )
};

PedestrianRenderPeerPlugin gPedestrianPeerPlugin( true );
PedestrianRenderClientPlugin gPedestrianClientPlugin( true );

PedestrianClientServerPlugin gClientServerPlugin;

