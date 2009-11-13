#include "NetBoidPlugin.h"
#include "EduNetConnect/OSReplicaTypes.h"
#include "OpenSteerUT/AbstractVehicleGroup.h"

#include "NetBoidFactory.h"
#include "NetBoidConditionReplica.h"

#include "glui/GL/glui.h"


NetPeerBoidPlugin gNetPeerBoidPlugin;
NetClientBoidPlugin gNetClientBoidPlugin;

//-----------------------------------------------------------------------------
NetPeerBoidPlugin::NetPeerBoidPlugin(bool bAddToRegistry):BaseClass( bAddToRegistry )
{
	this->m_kReplicaManager.SetAutoSerializeInterval(
		this->m_kReplicationSettings.interval);

	this->m_kReplicaManager.SetPlugin(&this->m_kGamePlugin);

	this->m_pkBoidFactory = new NetBoidReplicaFactory(&this->m_kReplicaManager);	
	this->m_kGamePlugin.SetBoidFactory( this->m_pkBoidFactory );
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
	delete m_pkConditionReplic;	
	BaseClass::DeleteContent();
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
NetClientBoidPlugin::NetClientBoidPlugin()
{
	this->m_kReplicaManager.SetPlugin(&this->m_kGamePlugin);

	this->m_pkBoidFactory = new BoidDummyFactory(&this->m_kReplicaManager);	
	this->m_kGamePlugin.SetBoidFactory( this->m_pkBoidFactory );
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
	delete m_pkConditionReplic;	
	BaseClass::DeleteContent();
}