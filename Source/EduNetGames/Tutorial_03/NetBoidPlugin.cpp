#include "NetBoidPlugin.h"

#include "EduNetCommon/EduNetDraw.h"
#include "EduNetConnect/OSReplicaTypes.h"
#include "OpenSteerUT/AbstractVehicleGroup.h"
#include "OpenSteerUT/PluginArray.h"
#include "OpenSteerUT/CameraPlugin.h"

#include "NetBoidConditionReplica.h"

//-----------------------------------------------------------------------------
NetPeerBoidPlugin::NetPeerBoidPlugin(bool bAddToRegistry):
	BaseClass( bAddToRegistry )
{
	this->m_kReplicaManager.SetAutoSerializeInterval(
		this->m_kReplicationSettings.interval);

	this->m_kReplicaManager.setPlugin( &this->m_kGamePlugin );

	this->m_pkBoidFactory = new NetBoidReplicaFactory(&this->m_kReplicaManager);
	OpenSteer::Boid* pkBoid = new OpenSteer::Boid();
	pkBoid->setParentPlugin( &this->m_kGamePlugin );
	this->m_pkBoidFactory->setMasterVehicle( pkBoid );
	this->m_kGamePlugin.setVehicleFactory( this->m_pkBoidFactory );
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
		new NetBoidConditionReplica(&this->m_kGamePlugin);
	m_kReplicaManager.Reference(m_pkConditionReplic);
}

//-----------------------------------------------------------------------------
void NetPeerBoidPlugin::handleFunctionKeys (int keyNumber)
{
	switch (keyNumber)
    {
    case 101:  ChangeReplicationInterval(5);         break; //GLUT_KEY_UP
    case 103:  ChangeReplicationInterval(-5);    break; //GLUT_KEY_DOWN  
	default: BaseClass::handleFunctionKeys(keyNumber);
    }	
}

//-----------------------------------------------------------------------------
void NetPeerBoidPlugin::ChangeReplicationInterval( RakNetTime additionalTime )
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
void NetPeerBoidPlugin::UpdateReplicationValue( void )
{
	this->m_kReplicaManager.SetAutoSerializeInterval(
		this->m_kReplicationSettings.interval);
}

//-----------------------------------------------------------------------------
void changeReplicationDelay(GLUI_Control* pkControl )
{
	NetPeerBoidPlugin* pkPlugin = (NetPeerBoidPlugin*)pkControl->ptr_val;
	pkPlugin->UpdateReplicationValue();
}

//-----------------------------------------------------------------------------
void NetPeerBoidPlugin::initGui( void* pkUserdata ) 
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
	this->m_kReplicaManager.setPlugin( &this->m_kGamePlugin );	
	this->m_kGamePlugin.setVehicleFactory( NULL  );
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
		new NetBoidConditionReplica(&this->m_kGamePlugin);
	m_kReplicaManager.Reference(m_pkConditionReplic);
}

//-----------------------------------------------------------------------------
void NetClientBoidPlugin::DeleteContent( void )
{	
	m_kReplicaManager.Dereference(m_pkConditionReplic);
	//delete m_pkConditionReplic;	
	BaseClass::DeleteContent();
}
//-----------------------------------------------------------------------------
// render client plugin
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
class NetBoidRenderClientPlugin : 
	public OpenSteer::PluginArrayPluginMixin<NetClientBoidPlugin>
{
	ET_DECLARE_BASE( PluginArrayPluginMixin<NetClientBoidPlugin> )
public:
	NetBoidRenderClientPlugin( bool bAddToRegistry = true ):BaseClass( bAddToRegistry ) 
	{
		this->addPlugin( new OpenSteer::CameraPlugin() );
	};
	virtual ~NetBoidRenderClientPlugin() {};

	OS_IMPLEMENT_CLASSNAME( NetBoidRenderClientPlugin )
};

NetBoidRenderClientPlugin gNetBoidClientPlugin( true );

//-----------------------------------------------------------------------------
// render server plugin
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
class NetBoidRenderPeerPlugin : 
	public OpenSteer::PluginArrayPluginMixin<NetPeerBoidPlugin>
{
	ET_DECLARE_BASE( PluginArrayPluginMixin<NetPeerBoidPlugin> )
public:
	NetBoidRenderPeerPlugin( bool bAddToRegistry = true ):
		BaseClass( bAddToRegistry ) 
	{		
		this->addPlugin( new OpenSteer::CameraPlugin() );
	};

	virtual ~NetBoidRenderPeerPlugin() {};

	OS_IMPLEMENT_CLASSNAME( NetBoidRenderPeerPlugin )
};

NetBoidRenderPeerPlugin gNetBoidPeerPlugin( true );

//-----------------------------------------------------------------------------
// client server plugin
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
class NetBoidClientServerPlugin : public OpenSteer::PluginArray
{
	ET_DECLARE_BASE(OpenSteer::PluginArray);
public:

	NetBoidClientServerPlugin(bool bAddToRegistry = true);
	virtual ~NetBoidClientServerPlugin();

	OS_IMPLEMENT_CLASSNAME( NetBoidClientServerPlugin );

	//---------------------------------------------------------------------
	// interface AbstractPlugin
	virtual void initGui( void* pkUserdata );
};

//-----------------------------------------------------------------------------
NetBoidClientServerPlugin::NetBoidClientServerPlugin( bool bAddToRegistry ):
BaseClass( bAddToRegistry )
{
	this->addPlugin( new OpenSteer::CameraPlugin() );
	this->addPlugin( new NetPeerBoidPlugin( false ) );
	this->addPlugin( new NetClientBoidPlugin( false ) );
}

//-----------------------------------------------------------------------------
NetBoidClientServerPlugin::~NetBoidClientServerPlugin()
{

}

//-----------------------------------------------------------------------------
void NetBoidClientServerPlugin::initGui( void* pkUserdata ) 
{
	BaseClass::initGui( pkUserdata );
	GLUI* glui = ::getRootGLUI();
	GLUI_Panel* pluginPanel = static_cast<GLUI_Panel*>( pkUserdata );
};

NetBoidClientServerPlugin gClientServerPlugin;