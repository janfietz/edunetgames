#include "NetworkPlugin.h"

#include "EduNetCommon/EduNetDraw.h"
#include "EduNetConnect/ServerVehicleUpdate.h"
#include "OpenSteerUT/AbstractVehicleGroup.h"
#include "OpenSteerUT/AbstractPluginUtilities.h"
#include "OpenSteer/GlobalData.h"
#include "EduNetProfile/EduNetProfile.h"

static const int SERVER_PONG_COUNT = 32;
#define PONG_WAIT_TIMEOUT 1000 // 5s

using namespace OpenSteer;

//-----------------------------------------------------------------------------
OpenSteer::AbstractVehicleMotionStatePlot NetworkPlugin::ms_kMotionStateProfile;

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
		m_bWaitForConnection(false)
{
}

//-----------------------------------------------------------------------------
NetworkPlugin::~NetworkPlugin(void)
{

}

//-----------------------------------------------------------------------------
void setPort(GLUI_Control* pkControl )
{
	GLUI_EditText* pkTextBox = (GLUI_EditText*)pkControl;

	NetworkPlugin* pkPlugin = (NetworkPlugin*)pkControl->ptr_val;
	NetworkAddress& kAddress = pkPlugin->accessCurrentAddress();
	kAddress.port = pkTextBox->get_int_val();
}
//-----------------------------------------------------------------------------
void setAddress(GLUI_Control* pkControl )
{
	GLUI_EditText* pkTextBox = (GLUI_EditText*)pkControl;

	NetworkPlugin* pkPlugin = (NetworkPlugin*)pkControl->ptr_val;
	NetworkAddress& kAddress = pkPlugin->accessCurrentAddress();
	kAddress.addressString = pkTextBox->get_text();
}
//-----------------------------------------------------------------------------
void connectToAddress(GLUI_Control* pkControl )
{
	NetworkPlugin* pkPlugin = (NetworkPlugin*)pkControl->ptr_val;
	const NetworkAddress& kAddress = pkPlugin->getCurrentAddress();
	pkPlugin->ConnectToAddress( kAddress );
}
//-----------------------------------------------------------------------------
void NetworkPlugin::initGui( void* pkUserdata )
{
	GLUI* glui = ::getRootGLUI();
	GLUI_Panel* pluginPanel = static_cast<GLUI_Panel*>( pkUserdata );


	OpenSteer::AbstractPlugin* pkPlugin = this->getHostedPlugin();
	AbstractEntity* pkPluginEntity = dynamic_cast<AbstractEntity*>( pkPlugin );
	assert( NULL != pkPluginEntity );
	if( NULL != pkPlugin )
	{
		GLUI_Rollout* pluginRollout = glui->add_rollout_to_panel( pluginPanel, pkPlugin ? pkPlugin->pluginName() : "Plugin", false );
		GLUI_Panel* subPluginPanel = pluginRollout;
		pkPlugin->initGui( subPluginPanel );
	}

	// general network plugin gui
	{
		GLUI_Rollout* profileRollout = glui->add_rollout_to_panel( pluginPanel, "Network Profile", true );
		GLUI_Panel* subPanel = profileRollout;
		glui->add_checkbox_to_panel( subPanel, "Show Motionstate", &OpenSteer::GlobalData::getInstance()->m_bShowMotionStatePlot);
		glui->add_checkbox_to_panel( subPanel, "Plot Network", &this->m_bDrawNetworkPlot);
		NetworkVehicle::initGui( profileRollout, pkPluginEntity->isRemoteObject() );
	}

	glui->add_checkbox_to_panel( pluginPanel, "AutoConnect", &this->m_bAutoConnect);

	if( true == this->addConnectBox() )
	{
		const NetworkAddress& kAddress = this->getCurrentAddress();

		GLUI_EditText* pkTextControl;
		pkTextControl = glui->add_edittext_to_panel( pluginPanel, "Address", GLUI_EDITTEXT_TEXT,
			NULL, -1, setAddress );
		pkTextControl->set_text( kAddress.addressString.C_String() );
		pkTextControl->set_ptr_val( this );

		pkTextControl = glui->add_edittext_to_panel( pluginPanel, "Port", GLUI_EDITTEXT_INT,
			NULL, -1, setPort );
		pkTextControl->set_int_val( kAddress.port );
		pkTextControl->set_int_limits(0, (unsigned short)-1 );
		pkTextControl->set_ptr_val( this );

		GLUI_Control* pkControl;
		pkControl = glui->add_button_to_panel( pluginPanel, "Connect", -1 , connectToAddress );
		pkControl->set_ptr_val( this );
	}

	this->addNetworkSimulatorGui( pluginPanel );

	this->addReplicaManagerGui( pluginPanel );
}

//-----------------------------------------------------------------------------
wxWindow* NetworkPlugin::prepareGui( wxWindow* parent )
{
	 wxWindow* parentWindow = BaseClass::prepareGui( parent );

	 wxBoxSizer* sizer = new wxBoxSizer( wxVERTICAL );
	 parentWindow->GetSizer()->Add( sizer );

	wxCheckBox* cbMotionState = new wxCheckBox( parentWindow, id_MotionState, "Motion state");
	cbMotionState->Bind( wxEVT_COMMAND_CHECKBOX_CLICKED, &NetworkPlugin::onSettingChanged, this );
	sizer->Add(cbMotionState);

	wxCheckBox* cbPlotNetwork = new wxCheckBox( parentWindow, id_PlotNetwork, "Plot network");
	cbPlotNetwork->Bind( wxEVT_COMMAND_CHECKBOX_CLICKED , &NetworkPlugin::onSettingChanged, this );
	sizer->Add(cbPlotNetwork);

	 //wxButton* boidAddButton = new wxButton(window,id_AddBoid, "+");
	 //boidAddButton->Bind( wxEVT_COMMAND_BUTTON_CLICKED, &NetBoidsPlugin::onAddBoid, this );
	 //buttonSizer->Add(boidAddButton);

	 return parentWindow;
}
//-----------------------------------------------------------------------------
void NetworkPlugin::onSettingChanged(wxCommandEvent& event)
{

}
//-----------------------------------------------------------------------------
void changeNetworkSimulatorSettings(GLUI_Control* pkControl )
{
	NetworkPlugin* pkPlugin = (NetworkPlugin*)pkControl->ptr_val;
	if(NULL == pkPlugin)
	{
		return;
	}

	NetworkSimulatorData& kData = pkPlugin->GetNetworkSimulatorSettings();
	switch( pkControl->get_id() )
	{
	case 1:
		{
			GLUI_Checkbox* pkCheckBox = (GLUI_Checkbox*)pkControl;
			kData.enabled = pkCheckBox->get_int_val();
		}break;
	case 2:
		{
			kData.packetloss = pkControl->get_float_val();
		}break;
	case 3:
		{
			kData.minExtraPing = pkControl->get_int_val();
		}break;
	case 4:
		{
			kData.extraPingVariance = pkControl->get_int_val();
		}break; break;
	}

	pkPlugin->UpdateNetworkSimulatorSettings();

}

//-----------------------------------------------------------------------------
void NetworkPlugin::addNetworkSimulatorGui( void* pkUserdata )
{
	GLUI* glui = ::getRootGLUI();
	GLUI_Panel* pluginPanel = static_cast<GLUI_Panel*>( pkUserdata );
	GLUI_Panel* simulatorPanel = glui->add_rollout_to_panel( pluginPanel, "Network Simulator", false );

	GLUI_Checkbox* pkControl =
		glui->add_checkbox_to_panel( simulatorPanel, "Enable Simulator", NULL, 1, changeNetworkSimulatorSettings );
	pkControl->set_ptr_val( this );

	GLUI_Spinner* repSpinner =
		glui->add_spinner_to_panel(simulatorPanel, "Packetloss",
		GLUI_SPINNER_FLOAT, NULL, 2, changeNetworkSimulatorSettings);
	repSpinner->set_float_limits(0.0f, 1.0f);
	repSpinner->set_ptr_val( this );

	GLUI_EditText* pkTextControl = glui->add_edittext_to_panel( simulatorPanel,
		"MinPing", GLUI_EDITTEXT_INT, NULL, 3, changeNetworkSimulatorSettings );
	pkTextControl->set_int_limits(0, (unsigned short)-1 );
	pkTextControl->set_int_val( m_kSimulatorData.minExtraPing );
	pkTextControl->set_ptr_val( this );

	pkTextControl = glui->add_edittext_to_panel( simulatorPanel,
		"PingVariance", GLUI_EDITTEXT_INT, NULL, 4, changeNetworkSimulatorSettings );
	pkTextControl->set_int_limits(0, (unsigned short)-1 );
	pkTextControl->set_int_val( m_kSimulatorData.extraPingVariance );
	pkTextControl->set_ptr_val( this );

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
	this->m_kReplicaManagerGui.incrementReplicationInterval( additionalTime );
}

//-----------------------------------------------------------------------------
void NetworkPlugin::addReplicaManagerGui( void* pkUserdata  )
{
	this->m_kReplicaManagerGui.initGui( pkUserdata );
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
	// TODO: client server difference ?
	switch (keyNumber)
	{
	case GLUT_KEY_UP:
		incrementReplicationInterval(5);
		break; //GLUT_KEY_UP
	case GLUT_KEY_DOWN:
		incrementReplicationInterval(-5);
		break; //GLUT_KEY_DOWN
	default:
		BaseClass::handleFunctionKeys(keyNumber);
	}
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

