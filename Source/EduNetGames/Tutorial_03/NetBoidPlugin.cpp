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
	this->setGamePluginReplicaManager( &this->m_kReplicaManager );
	this->m_kReplicaManager.setPlugin( &this->m_kGamePlugin );

	this->m_pkBoidFactory = new NetBoidReplicaFactory(&this->m_kReplicaManager);
	OpenSteer::Boid* pkBoid = new OpenSteer::Boid();
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
		new NetBoidConditionReplica(&this->m_kGamePlugin);
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
class NetBoidRenderOfflinePlugin : 
	public OpenSteer::PluginArrayPluginMixin<OpenSteer::BoidsPlugin>
{
	ET_DECLARE_BASE( OpenSteer::PluginArrayPluginMixin<OpenSteer::BoidsPlugin> )
public:
	NetBoidRenderOfflinePlugin( bool bAddToRegistry = true ):
		BaseClass( bAddToRegistry ) 
	{		
		this->addPlugin( new OpenSteer::CameraPlugin() );
	};

	virtual ~NetBoidRenderOfflinePlugin() {};

	OS_IMPLEMENT_CLASSNAME( NetBoidRenderOfflinePlugin )

	virtual float selectionOrderSortKey (void) const { return 1.0f ;}

};


//-----------------------------------------------------------------------------
// render client plugin
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
class NetBoidRenderClientPlugin : 
	public OpenSteer::PluginArrayPluginMixin<NetClientBoidPlugin>
{
	ET_DECLARE_BASE( OpenSteer::PluginArrayPluginMixin<NetClientBoidPlugin> )
public:
	NetBoidRenderClientPlugin( bool bAddToRegistry = true ):
		BaseClass( bAddToRegistry ) 
	{
		this->addPlugin( new OpenSteer::CameraPlugin() );
	};
	virtual ~NetBoidRenderClientPlugin() {};

	OS_IMPLEMENT_CLASSNAME( NetBoidRenderClientPlugin )
};


//-----------------------------------------------------------------------------
// render server plugin
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
class NetBoidRenderPeerPlugin : 
	public OpenSteer::PluginArrayPluginMixin<NetPeerBoidPlugin>
{
	ET_DECLARE_BASE( OpenSteer::PluginArrayPluginMixin<NetPeerBoidPlugin> )
public:
	NetBoidRenderPeerPlugin( bool bAddToRegistry = true ):
		BaseClass( bAddToRegistry ) 
	{		
		this->addPlugin( new OpenSteer::CameraPlugin() );
	};

	virtual ~NetBoidRenderPeerPlugin() {};

	OS_IMPLEMENT_CLASSNAME( NetBoidRenderPeerPlugin )
};


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

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// namespace
// {
// 	using namespace OpenSteer;
// 	BoidsPlugin gBoidsPlugin(true);
// }


NetBoidRenderOfflinePlugin gNetBoidRenderOfflinePlugin( true );
NetBoidRenderPeerPlugin gNetBoidPeerPlugin( true );
NetBoidRenderClientPlugin gNetBoidRenderClientPlugin( true );
NetBoidClientServerPlugin gClientServerPlugin;
