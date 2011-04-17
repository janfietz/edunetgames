#include "NetworkPlugin.h"

#include "EduNetConnect/ServerVehicleUpdate.h"
#include "OpenSteerUT/AbstractVehicleGroup.h"
#include "OpenSteerUT/AbstractPluginUtilities.h"
#include "OpenSteer/GlobalData.h"
#include "EduNetProfile/EduNetProfile.h"
#include "EduNetConnectGuiReplicationSettings.h"

static const int SERVER_PONG_COUNT = 32;
#define PONG_WAIT_TIMEOUT 1000 // 5s

using namespace OpenSteer;

#include "EduNetConnectGui.h"
//-----------------------------------------------------------------------------
OpenSteer::AbstractVehicleMotionStatePlot NetworkPlugin::ms_kMotionStateProfile;

class NetworkPluginPanel : public NetworkPluginGui
{
public: 
	
	NetworkPluginPanel( NetworkPlugin* netPlugin, wxWindow* parent, wxWindowID id = wxID_ANY,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxTAB_TRAVERSAL): NetworkPluginGui( parent, id, pos, size, style),
		m_plugin(netPlugin)
	{
		NetworkAddress& kAddress = m_plugin->accessCurrentAddress();
		*m_PortTxtCtrl << kAddress.port;
		*m_AddressTxtCtrl << kAddress.addressString.C_String();

		m_AutoConnectCheckBox->SetValue( m_plugin->DoAutoConnect() );

	};


	//Handlers
	virtual void OnCheckedMotionPlot( wxCommandEvent& event );
	virtual void OnCheckedNetworkPlot( wxCommandEvent& event );
	virtual void OnAddressChanged( wxCommandEvent& event );
	virtual void OnAutoConnectChecked( wxCommandEvent& event );
	virtual void OnSimEnabledChecked( wxCommandEvent& event );
	virtual void OnPacketLossChanged( wxScrollEvent& event );
	virtual void OnSimDelayChanged( wxSpinEvent& event );
	virtual void OnSimVarianceChanged( wxSpinEvent& event );
protected:
	virtual ~NetworkPluginPanel(){};

	NetworkPlugin* m_plugin;
};

void NetworkPluginPanel::OnCheckedMotionPlot( wxCommandEvent& event )
{
	OpenSteer::GlobalData::getInstance()->m_bShowMotionStatePlot = event.IsChecked();
}

void NetworkPluginPanel::OnCheckedNetworkPlot( wxCommandEvent& event )
{
	m_plugin->DrawNetworkPlot(event.IsChecked());
}

void NetworkPluginPanel::OnAddressChanged( wxCommandEvent& event )
{
	NetworkAddress& kAddress = m_plugin->accessCurrentAddress();
	if (event.GetId() == etNet_Address)
	{
		wxString adressStr = event.GetString();
		if (adressStr.IsEmpty() == false)
		{
			kAddress.addressString = adressStr.ToAscii();
		}

	}
	else if (event.GetId() == etNet_Port)
	{
		wxString portStr = event.GetString();
		unsigned long ulPort;
		if( portStr.ToCULong(&ulPort ) == true )
		{
			if( ulPort < USHRT_MAX )
			{
				kAddress.port = static_cast<unsigned short>(ulPort);
			}
		}
	}
}

void NetworkPluginPanel::OnAutoConnectChecked( wxCommandEvent& event )
{
	m_plugin->AutoConnect(event.IsChecked());
}

void NetworkPluginPanel::OnSimEnabledChecked( wxCommandEvent& event )
{
	NetworkSimulatorData& simData = m_plugin->GetNetworkSimulatorSettings();
	simData.enabled = event.IsChecked();

	m_plugin->UpdateNetworkSimulatorSettings();
}

void NetworkPluginPanel::OnPacketLossChanged( wxScrollEvent& event )
{
	NetworkSimulatorData& simData = m_plugin->GetNetworkSimulatorSettings();

	int pos = event.GetPosition();
	simData.packetloss = static_cast<float>(pos) / 100.0f;

	m_plugin->UpdateNetworkSimulatorSettings();
}

void NetworkPluginPanel::OnSimDelayChanged( wxSpinEvent& event )
{
	NetworkSimulatorData& simData = m_plugin->GetNetworkSimulatorSettings();

	int pos = event.GetPosition();
	simData.minExtraPing = pos;

	m_plugin->UpdateNetworkSimulatorSettings();
}

void NetworkPluginPanel::OnSimVarianceChanged( wxSpinEvent& event )
{
	NetworkSimulatorData& simData = m_plugin->GetNetworkSimulatorSettings();

	int pos = event.GetPosition();
	simData.extraPingVariance = pos;
}
//-----------------------------------------------------------------------------
NetworkPlugin::NetworkPlugin(bool bAddToRegistry):
	BaseClass( bAddToRegistry ),
		m_pNetInterface( NULL ),
		m_pkNetworkIdManager( NULL ),
		m_pkGamePluginReplicaManager( NULL ),
		m_pkAddress( NULL ),
		m_eNetworkSessionType( ENetworkSessionType_Undefined ),
		m_bAutoConnect(1),
		m_bDrawNetworkPlot(0),
		m_bWaitForConnection(false),
		m_pReplicaManagerGui(NULL)
{
}

//-----------------------------------------------------------------------------
NetworkPlugin::~NetworkPlugin(void)
{

}

//-----------------------------------------------------------------------------
wxWindow* NetworkPlugin::prepareGui( wxWindow* parent )
{
	 wxWindow* parentWindow = BaseClass::prepareGui( parent );

	 wxWindow* myPanel = new NetworkPluginPanel( this, parentWindow );
	 parentWindow->GetSizer()->Add(myPanel);

	 if (m_pReplicaManagerGui == NULL)
	 {
		 m_pReplicaManagerGui = new EduNetConnectGuiReplicationSettings( parentWindow );
		 parentWindow->GetSizer()->Add(m_pReplicaManagerGui);
	 }
	 return parentWindow;
}


//-----------------------------------------------------------------------------
void NetworkPlugin::UpdateNetworkSimulatorSettings( void )
{
	if (NULL != this->m_pNetInterface)
	{
		if (1 == this->m_kSimulatorData.enabled)
		{
			this->m_pNetInterface->ApplyNetworkSimulator(
				this->m_kSimulatorData.packetloss,
				(unsigned short)this->m_kSimulatorData.minExtraPing,
				(unsigned short)this->m_kSimulatorData.extraPingVariance);
		}else
		{
			this->m_pNetInterface->ApplyNetworkSimulator(0.0, 0, 0);
		}
	}
}

//-----------------------------------------------------------------------------
void NetworkPlugin::incrementReplicationInterval( int additionalTime )
{
	//TODO: forward to replicamanager
}

//-----------------------------------------------------------------------------
void NetworkPlugin::open( void )
{	
	EduNetConnect::queryConnectionsSettings(this->m_ConnectionSettings);

	this->CreateNetworkInterface();
	this->StartNetworkSession();
	this->CreateContent();
    this->acceptConnections();
}

//-----------------------------------------------------------------------------
void NetworkPlugin::close( void )
{
	this->StopNetworkSession();
	this->DeleteContent();
	this->DestroyNetworkInterface();
}

//-----------------------------------------------------------------------------
void NetworkPlugin::reset (void)
{

}

//-----------------------------------------------------------------------------
void NetworkPlugin::update (const float currentTime, const float elapsedTime)
{
	if( false == this->isEnabled() )
	{
		return;
	}
	if( NULL == this->m_pNetInterface )
	{
		return;
	}

	EduNet::IProfile* pkProfile = OpenSteer::GlobalData::accessProfile();
	EduNet::IProfileNodePtr spNode = pkProfile->allocNode( "a" );
	ET_PROFILE(updateNetworkPlugin);

	if( !IsConnected()&& DoAutoConnect() )
	{
		this->Connect();
	}

	if( true == this->WaitForPong() )
	{
		this->CheckPongTimeout();
	}

	this->ReceivePackets();

	this->recordNetworkStatistics( currentTime, elapsedTime );
}

//-----------------------------------------------------------------------------
void NetworkPlugin::recordNetUpdate(
	osAbstractVehicle* pkVehicle, const float currentTime, const float elapsedTime )
{
	if( 0 == OpenSteer::GlobalData::getInstance()->m_bShowMotionStatePlot )
	{
		return;
	}
	NetworkPlugin::ms_kMotionStateProfile.recordNetUpdate( pkVehicle, currentTime, elapsedTime );
}

//-----------------------------------------------------------------------------
void NetworkPlugin::updateMotionStateProfile( const float currentTime, const float elapsedTime )
{
	if( OpenSteer::SimpleVehicle::getSelectedVehicle() != NULL )
	{
		// find the selected vehicle by id
		AbstractPlugin* pkHostedPlugin = this->getHostedPlugin();
		AbstractEntity* pkPluginEntity = dynamic_cast<AbstractEntity*>( pkHostedPlugin );
		assert( NULL != pkPluginEntity );

		bool bTrySelectServerVehicle = false;
		if( true == pkPluginEntity->isRemoteObject() )
		{
			bool bTest = true;
			bTest = false;
		}
		else
		{
			AbstractVehicle* vehicle = OpenSteer::SimpleVehicle::getSelectedVehicle();
			const bool bIsRemoteVehicle = vehicle->isRemoteObject();
			if( true == bIsRemoteVehicle )
			{
				bTrySelectServerVehicle = true;
			}
		}

		if( ( 0 != OpenSteer::GlobalData::getInstance()->m_bShowMotionStatePlot ) || ( true == bTrySelectServerVehicle ) )
		{
			AbstractVehicleGroup kAV( pkHostedPlugin->allVehicles() );
			NetworkId networkId = OpenSteer::SimpleVehicle::getSelectedVehicle()->getNetworkId();
			AVGroup::iterator kFound = kAV.findNetworkVehicle( networkId );
			if( kFound != kAV.end() )
			{
				// TODO: move to own service function
				AbstractVehicle* pkVehicle = *kFound;
				if( pkVehicle != OpenSteer::SimpleVehicle::getSelectedVehicle() )
				{
					if( false == pkVehicle->isRemoteObject() )
					{
						// switch to the server object
						// as this will not jitter
						OpenSteer::SimpleVehicle::setSelectedVehicle( pkVehicle );
					}
				}

				if( ( 0 != OpenSteer::GlobalData::getInstance()->m_bShowMotionStatePlot ) )
				{
					// update motion state plot
					NetworkPlugin::ms_kMotionStateProfile.recordUpdate( *kFound, currentTime, elapsedTime );
				}
			}
		}
	}
}

//-----------------------------------------------------------------------------
void NetworkPlugin::redraw (OpenSteer::AbstractRenderer* pRenderer,
	const float currentTime,
	const float elapsedTime)
{
	if( false == this->isVisible() )
	{
		return;
	}

	if( false == this->IsConnected() )
	{

	}
	else if( 0 != OpenSteer::GlobalData::getInstance()->m_bDebugNetStats )
	{
		// display status
		const bool bIsClient = this->isRemoteObject();

		Color kColorNetStats = gGray80;
		std::ostringstream status;
		status << std::setprecision (2);
		status << std::setiosflags (std::ios::fixed);

		switch( this->m_eNetworkSessionType )
		{
		case(ENetworkSessionType_Client):
			{
				status << "Client";
				kColorNetStats = gOrange;
				break;
			}
		case(ENetworkSessionType_Peer):
			{
				status << "Server";
				kColorNetStats = gGreen;
				break;
			}
		}

		SocketDescriptor& sd = this->m_kSocketDescriptor;
		status << " Port: ";
		status << sd.port << std::endl;;
		status << "Bytes Received: ";
		status << this->m_kStats.m_uiBytesReceived;
		status << " Sent: ";
		status << this->m_kStats.m_uiBytesSend;

		status << std::endl;
		const float h = pRenderer->drawGetWindowHeight();
		osVector3 screenLocation( 10, h, 0);
		float fOffset = 100.0f;
		fOffset += bIsClient ? 50.0f : 100.0f;
		screenLocation.y -= fOffset;
		pRenderer->draw2dTextAt2dLocation(
			status, screenLocation, kColorNetStats,
			pRenderer->drawGetWindowWidth(), 
			pRenderer->drawGetWindowHeight() );

	}

	if( 0 != OpenSteer::GlobalData::getInstance()->m_bShowMotionStatePlot )
	{
		// draw motion state plot
		if( NULL != SimpleVehicle::getSelectedVehicle() )
		{
			NetworkPlugin::ms_kMotionStateProfile.draw( pRenderer, currentTime );
		}
	}

	if( 0 != this->m_bDrawNetworkPlot )
	{
		this->drawNetworkPlot(pRenderer, currentTime, elapsedTime);
	}
}

//-----------------------------------------------------------------------------
void NetworkPlugin::handleFunctionKeys (int keyNumber)
{
	//// TODO: client server difference ?
	//switch (keyNumber)
	//{
	//case GLUT_KEY_UP:
	//	incrementReplicationInterval(5);
	//	break; //GLUT_KEY_UP
	//case GLUT_KEY_DOWN:
	//	incrementReplicationInterval(-5);
	//	break; //GLUT_KEY_DOWN
	//default:
	//	BaseClass::handleFunctionKeys(keyNumber);
	//}
}

//-----------------------------------------------------------------------------
void NetworkPlugin::printMiniHelpForFunctionKeys (void) const
{
}

//-----------------------------------------------------------------------------
OpenSteer::AbstractPlugin* NetworkPlugin::getHostedPlugin( void ) const
{
	return NULL;
}

//-----------------------------------------------------------------------------
bool NetworkPlugin::DoAutoConnect( void ) const
{
	return ( this->m_bAutoConnect != 0 );
}

//-----------------------------------------------------------------------------
void NetworkPlugin::CheckPongTimeout( void )
{
	RakNetTime kCurrent = RakNet::GetTime();
	if( kCurrent > this->m_kPongEndTime )
	{
		this->m_iWaitForPongPort += 1;

		const unsigned int uiPortCount = this->m_ConnectionSettings.uiPortPongCount;		
		if(uiPortCount <= this->m_iWaitForPongPort - this->m_uiStartPort)
		{
			this->m_iWaitForPongPort -= uiPortCount;
		}
		this->m_iWaitForPongPort *= -1;
	}
}

//-----------------------------------------------------------------------------
void NetworkPlugin::CreateContent( void )
{
	printf("NetworkPlugin::CreateContent()\n");
}

//-----------------------------------------------------------------------------
void NetworkPlugin::DeleteContent( void )
{
	printf("NetworkPlugin::DeleteContent()\n");
}

//-----------------------------------------------------------------------------
void NetworkPlugin::InitializeServerPortSettings( void )
{	
	this->m_uiStartPort = this->m_ConnectionSettings.uiServerStartPort;	

	this->accessCurrentAddress().port = this->m_uiStartPort;
	this->m_iWaitForPongPort = -1 * this->m_uiStartPort;
}

//-----------------------------------------------------------------------------
void NetworkPlugin::CreateNetworkInterface( void )
{
	this->m_pNetInterface = RakNetworkFactory::GetRakPeerInterface();
	this->AttachNetworkIdManager();
	this->InitializeServerPortSettings();
}

//-----------------------------------------------------------------------------
void NetworkPlugin::DestroyNetworkInterface( void )
{
	if( NULL != this->m_pNetInterface )
	{
		this->m_pNetInterface->SetNetworkIDManager( NULL );
	}
	ET_SAFE_DELETE( this->m_pkNetworkIdManager );
	if( NULL != this->m_pNetInterface )
	{
		RakNetworkFactory::DestroyRakPeerInterface( this->m_pNetInterface );
		this->m_pNetInterface = NULL;
		printf("Destroyed peer.\n");
	}
	ET_SAFE_DELETE( this->m_pkAddress );
}

//-----------------------------------------------------------------------------
void NetworkPlugin::StartClientNetworkSession( void )
{	
	this->m_uiStartPort = this->m_ConnectionSettings.uiClientStartPort;
	SocketDescriptor& sd = this->m_kSocketDescriptor;
	sd.port = this->m_uiStartPort;
	if( true == this->StartupNetworkSession( sd, 1, 0 ) )
	{
		this->m_eNetworkSessionType = ENetworkSessionType_Client;
	}
	printf("Starting client at port: %d.\n", sd.port);
}

//-----------------------------------------------------------------------------
bool NetworkPlugin::StartupNetworkSession( SocketDescriptor& sd, unsigned short maxAllowed, unsigned short maxIncoming )
{
	// 	virtual bool Startup( unsigned short maxConnections, int _threadSleepTimer, SocketDescriptor *socketDescriptors, unsigned socketDescriptorCount )=0;
	const int threadSleepTimer = 33;
	bool bStarted(false);
	if( NULL != this->m_pNetInterface )
	{
		while( false == bStarted )
		{
			while( SocketLayer::IsPortInUse(sd.port) == true )
				++sd.port;
			if( true == this->m_pNetInterface->Startup( maxAllowed, threadSleepTimer, &sd, 1 ) )
			{
				//give a password
				this->setIncommingPassword();

				this->m_pNetInterface->SetMaximumIncomingConnections( maxIncoming );
				bStarted = true;
			}
		}
	}
	return bStarted;
}
//-----------------------------------------------------------------------------
void NetworkPlugin::setIncommingPassword( void )
{
	const char* pszIncommingPassword = this->m_ConnectionSettings.sessionPassword.C_String();
	int	incommingPasswordLength =  this->m_ConnectionSettings.sessionPassword.GetLength();
	this->m_pNetInterface->SetIncomingPassword( pszIncommingPassword, incommingPasswordLength );
}

//-----------------------------------------------------------------------------
void NetworkPlugin::StopNetworkSession( void )
{
	if( NULL != this->m_pNetInterface )
	{
		// do not allow any further connections
		this->m_pNetInterface->SetMaximumIncomingConnections( 0 );
	}
	this->CloseOpenConnections();

	if( NULL != this->m_pNetInterface )
	{
		this->m_pNetInterface->Shutdown( 100,0 );
	}
	this->m_eNetworkSessionType = ENetworkSessionType_Undefined;
	this->m_kStats.reset();
}
//-----------------------------------------------------------------------------
void NetworkPlugin::acceptConnections( void )
{
  this->m_pNetInterface->SetMaximumIncomingConnections( this->getMaxIncomingConnections() );
}
//-----------------------------------------------------------------------------
unsigned short NetworkPlugin::getMaxIncomingConnections( void ) const
{
  return 0;
}
//-----------------------------------------------------------------------------
void NetworkPlugin::CloseOpenConnections( void )
{
	DataStructures::List<SystemAddress> kAddresses;
	DataStructures::List<RakNetGUID> kGuids;
	if( NULL != this->m_pNetInterface )
	{

		this->m_pNetInterface->GetSystemList( kAddresses, kGuids );
		unsigned int usCount = kAddresses.Size();
		if( 0 < usCount )
		{
		for(unsigned int us = 0; us < usCount; ++us)
		{
			this->m_pNetInterface->CloseConnection(
				kAddresses[us], true );
		}

		while(IsConnected())
		{
			EduNet::sleep( 100 );
		}
		}
	}
}

//-----------------------------------------------------------------------------
void NetworkPlugin::ReceivePackets( void )
{
	ET_PROFILE(ReceivePackets);
	while(true)
	{
		Packet* pPacket = this->m_pNetInterface->Receive();
		if( 0 != pPacket )
		{
			this->OnReceivedPacket( pPacket );
			this->m_pNetInterface->DeallocatePacket(pPacket);
		}
		else
		{
			return;
		}
	}
}

//-----------------------------------------------------------------------------
void NetworkPlugin::OnReceivedPacket( Packet* pPacket )
{
	switch( pPacket->data[0] )
	{
	case ID_CONNECTION_ATTEMPT_FAILED:
		{
			printf("ID_CONNECTION_ATTEMPT_FAILED %s\n", pPacket->systemAddress.ToString());
			m_bWaitForConnection = false;
		}
		break;
	case ID_NO_FREE_INCOMING_CONNECTIONS:
		{
			m_bWaitForConnection = false;
			printf("ID_NO_FREE_INCOMING_CONNECTIONS\n");
		}
		break;
	case ID_CONNECTION_REQUEST_ACCEPTED:
		{
			m_bWaitForConnection = false;
			printf("ID_CONNECTION_REQUEST_ACCEPTED\n");

		}break;
	case ID_NEW_INCOMING_CONNECTION:
		printf("ID_NEW_INCOMING_CONNECTION from %s\n", pPacket->systemAddress.ToString());
		break;
	case ID_DISCONNECTION_NOTIFICATION:
		{
			m_bWaitForConnection = false;
			printf("ID_DISCONNECTION_NOTIFICATION %s\n", pPacket->systemAddress.ToString());

		}break;
	case ID_CONNECTION_LOST:
		{
			m_bWaitForConnection = false;
			printf("ID_CONNECTION_LOST %s\n", pPacket->systemAddress.ToString());

		}break;
	case ID_INVALID_PASSWORD:
		{
			m_bWaitForConnection = false;
			printf("ID_INVALID_PASSWORD (%s) %s\n",
				this->m_ConnectionSettings.sessionPassword.C_String(),
				pPacket->systemAddress.ToString() );

		}break;
	case ID_MODIFIED_PACKET:
		printf("ID_MODIFIED_PACKET %s\n", pPacket->systemAddress.ToString());
		break;

	case ID_PONG:
		{
			//printf("ID_PONG\n");
			this->ReceivedPongPacket( pPacket );
		}
		break;
		/*case ID_ADVERTISE_SYSTEM:
		rakPeer->Connect(packet->systemAddress.ToString(false), packet->systemAddress.port,0,0);
		break;*/
	}
}

//-----------------------------------------------------------------------------
void NetworkPlugin::ReceivedPongPacket( Packet* pPacket )
{
	if( true == this->WaitForPong() )
	{
		if( pPacket->systemAddress != this->m_pNetInterface->GetInternalID() )
		{
			printf("Got pong from %s(%s) with time %i\n",
				pPacket->systemAddress.ToString(),
				pPacket->guid.ToString(),
				this->m_kPongEndTime - RakNet::GetTime() );

			NetworkAddress kAddress;
			kAddress.addressString = pPacket->systemAddress.ToString(false);
			kAddress.port = pPacket->systemAddress.port;

			this->ConnectToAddress( kAddress );
			this->m_iWaitForPongPort *= -1;
			this->m_bWaitForConnection = true;
		}
	}
}

//-----------------------------------------------------------------------------
void NetworkPlugin::ConnectToAddress( const NetworkAddress& kAddress )
{
	this->CloseOpenConnections();

	RakNetTime uiTimeout = 0;
#ifdef ET_DEBUG
	uiTimeout = 3600000;
#endif

	const char* pszPassword = this->m_ConnectionSettings.sessionPassword.C_String();
	size_t passwordLength = this->m_ConnectionSettings.sessionPassword.GetLength();
	this->m_pNetInterface->Connect(
		kAddress.addressString.C_String(),
		kAddress.port,
		pszPassword,passwordLength,
		0, 12, 500, uiTimeout);
}

//-----------------------------------------------------------------------------
bool NetworkPlugin::WaitForPong( void ) const
{
	return ( 0 <= this->m_iWaitForPongPort );
}

//-----------------------------------------------------------------------------
bool NetworkPlugin::Connect()
{
	return this->PingForOtherPeers( -1* this->m_iWaitForPongPort);
}

//-----------------------------------------------------------------------------
void NetworkPlugin::Disconnect()
{
	this->CloseOpenConnections();
}

//-----------------------------------------------------------------------------
bool NetworkPlugin::PingForOtherPeers( const int iPort )
{
	if (true == m_bWaitForConnection)
	{
		return false;
	}

	if( false == this->WaitForPong() )
	{
		unsigned short usMyPort =
			this->m_pNetInterface->GetInternalID( UNASSIGNED_SYSTEM_ADDRESS,0 ).port;
		if (usMyPort != iPort)
		{
			if( true == this->m_pNetInterface->Ping("255.255.255.255", iPort, true) )
			{
				this->m_kPongEndTime = RakNet::GetTime() +  PONG_WAIT_TIMEOUT;
			}
		}
		this->m_iWaitForPongPort = iPort;
	}
	return this->WaitForPong();
}

//-----------------------------------------------------------------------------
void NetworkPlugin::AttachNetworkIdManager( void )
{
	ET_SAFE_DELETE( this->m_pkNetworkIdManager );
	this->m_pkNetworkIdManager = ET_NEW NetworkIDManager();
	this->m_pNetInterface->SetNetworkIDManager( this->m_pkNetworkIdManager );
	this->m_pkNetworkIdManager->SetIsNetworkIDAuthority( this->HasIdAuthority() );
}

//-----------------------------------------------------------------------------
bool NetworkPlugin::IsConnected() const
{
	return 0 < this->m_pNetInterface->NumberOfConnections();
}

//-----------------------------------------------------------------------------
void NetworkPlugin::recordNetworkStatistics(const float currentTime,
	const float elapsedTime)
{
	RakNetStatistics kStats;
	this->gatherNetworkStatistics( kStats );
	this->m_kStats.m_uiBytesReceived = kStats.runningTotal[ACTUAL_BYTES_RECEIVED];
	this->m_kStats.m_uiBytesSend = kStats.runningTotal[ACTUAL_BYTES_RECEIVED];
	if( 0 != this->m_bDrawNetworkPlot )
	{
		this->m_kNetworkPlot.recordUpdate( kStats, currentTime, elapsedTime);
	}
}

//-----------------------------------------------------------------------------
void NetworkPlugin::gatherNetworkStatistics( RakNetStatistics& kStats )
{

	DataStructures::List<SystemAddress> kAddresses;
	DataStructures::List<RakNetGUID> kGuids;
	if( NULL != this->m_pNetInterface )
	{
		// collect statistics of all connected and active systems
		this->m_pNetInterface->GetStatistics(UNASSIGNED_SYSTEM_ADDRESS, &kStats);

		/*this->m_pNetInterface->GetSystemList( kAddresses, kGuids );
		unsigned int usCount = kAddresses.Size();
		for(unsigned int us = 0; us < usCount; ++us)
		{
			RakNetStatistics kSystemStats;
			RakNetStatistics* pkResult =
				this->m_pNetInterface->GetStatistics(kAddresses[us], &kSystemStats);
			if (NULL != pkResult)
			{
				kStats += kSystemStats;
			}
		}	*/
	}
}

//-----------------------------------------------------------------------------
void NetworkPlugin::drawNetworkPlot(OpenSteer::AbstractRenderer* pRenderer,
	const float currentTime,
	const float elapsedTime)
{
	this->m_kNetworkPlot.draw( pRenderer, this );
}

//-----------------------------------------------------------------------------
AbstractEntityReplica* NetworkPlugin::allocEntityReplica(
	OpenSteer::AbstractPlugin* pPlugin,
	OpenSteer::EntityClassId classId,
	bool bIsRemoteObject,  bool bClientReplica ) const
{
	AbstractEntityReplica* pkNewReplica = ET_NEW AbstractEntityReplica( pPlugin, classId, bIsRemoteObject, bClientReplica  );
	return pkNewReplica;
}
//-----------------------------------------------------------------------------
AbstractEntityReplica* NetworkPlugin::createLocalEntityReplica( 
	OpenSteer::AbstractPlugin* pPlugin, 
	OpenSteer::EntityClassId classId, 
	bool bIsRemoteObject, 
	bool bClientReplica ) const
{
	return ET_NEW AbstractEntityReplica( 
		pPlugin, classId, bIsRemoteObject, bClientReplica );
}

//-----------------------------------------------------------------------------
void NetworkPlugin::setGamePluginReplicaManager( 
	RakNet::ReplicaManager3* pkReplicaManager )
{
	m_pkGamePluginReplicaManager = pkReplicaManager;
	if (m_pReplicaManagerGui != NULL)
	{
		m_pReplicaManagerGui->useReplicaManager( pkReplicaManager );
	}
}
