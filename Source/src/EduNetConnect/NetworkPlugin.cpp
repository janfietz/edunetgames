#include "NetworkPlugin.h"

#include "EduNetCommon/EduNetDraw.h"



static const int SERVER_PONG_COUNT = 32;
#define PONG_WAIT_TIMEOUT 1000 // 5s

using namespace OpenSteer;

//-----------------------------------------------------------------------------
NetworkPlugin::NetworkPlugin(bool bAddToRegistry):
	BaseClass( bAddToRegistry ),
		m_pNetInterface( NULL ),
		m_eNetworkSessionType( ENetworkSessionType_Undefined ),
		m_bAutoConnect(1)

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
	NetworkAddress& kAddress = pkPlugin->GetCurrentAddress();
	kAddress.port = pkTextBox->get_int_val();
}
//-----------------------------------------------------------------------------
void setAddress(GLUI_Control* pkControl )
{
	GLUI_EditText* pkTextBox = (GLUI_EditText*)pkControl;
	
	NetworkPlugin* pkPlugin = (NetworkPlugin*)pkControl->ptr_val;
	NetworkAddress& kAddress = pkPlugin->GetCurrentAddress();
	kAddress.addressString = pkTextBox->get_text();	
}
//-----------------------------------------------------------------------------
void connectToAddress(GLUI_Control* pkControl )
{
	NetworkPlugin* pkPlugin = (NetworkPlugin*)pkControl->ptr_val;
	const NetworkAddress& kAddress = pkPlugin->GetCurrentAddress();
	pkPlugin->ConnectToAddress(kAddress);
}
//-----------------------------------------------------------------------------
void NetworkPlugin::initGui( void* pkUserdata )
{
	GLUI* glui = ::getRootGLUI();
	GLUI_Panel* pluginPanel = static_cast<GLUI_Panel*>( pkUserdata );


	OpenSteer::AbstractPlugin* pkHostedPlugin = this->getHostedPlugin();
	if( NULL != pkHostedPlugin )
	{
		GLUI_Panel* subPluginPanel = glui->add_panel_to_panel( pluginPanel, pkHostedPlugin ? pkHostedPlugin->name() : "Plugin" );
		pkHostedPlugin->initGui( subPluginPanel );
	}

	glui->add_checkbox_to_panel( pluginPanel, "AutoConnect", &this->m_bAutoConnect);
	if(true == this->AddConnectBox())
	{
		
		GLUI_EditText* pkTextControl;
		pkTextControl = glui->add_edittext_to_panel( pluginPanel, "Address", GLUI_EDITTEXT_TEXT,
			NULL, -1, setAddress );
		pkTextControl->set_text(this->m_kAddress.addressString.C_String());
		pkTextControl->set_ptr_val( this );

		pkTextControl = glui->add_edittext_to_panel( pluginPanel, "Port", GLUI_EDITTEXT_INT,
			NULL, -1, setPort );
		pkTextControl->set_int_val(this->m_kAddress.port);
		pkTextControl->set_int_limits(0, (unsigned short)-1 );
		pkTextControl->set_ptr_val( this );
		
		GLUI_Control* pkControl;
		pkControl = glui->add_button_to_panel( pluginPanel, "Connect", -1 , connectToAddress );
		pkControl->set_ptr_val( this );
	}

}

//-----------------------------------------------------------------------------
void NetworkPlugin::open( void )
{	
	this->CreateNetworkInterface();
	this->StartNetworkSession(); 
	this->CreateContent();
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
	if( !IsConnected()&& DoAutoConnect() )
	{
		this->Connect();
	}

	if(true == this->WaitForPong() )
	{
		this->CheckPongTimeout();
	}

	this->ReceivePackets();
}

//-----------------------------------------------------------------------------
void NetworkPlugin::redraw (const float currentTime, const float elapsedTime)
{

	// display status 

	bool bIsClient = ( ENetworkSessionType_Client == this->m_eNetworkSessionType );

	std::ostringstream status;
	status << std::setprecision (2);
	status << std::setiosflags (std::ios::fixed);
	if( false == this->IsConnected() )
	{

	}
	else
	{
		switch( this->m_eNetworkSessionType )
		{
		case(ENetworkSessionType_Client):
			{
				status << "Client";
				break;
			}
		case(ENetworkSessionType_Peer):
			{
				status << "Server";
				break;
			}
		}
		SocketDescriptor& sd = this->m_kSocketDescriptor;
		status << " Port: ";
		status << sd.port << std::endl;;
		status << "PacketsReceived: ";
		status << this->m_kStats.m_uiPacketsReceived << std::endl;;
		
	}
	const float h = drawGetWindowHeight();
	osVector3 screenLocation( 10, h, 0);
	float fOffset = 50.0f;
	fOffset += bIsClient ? 50.0f : 100.0f;
	screenLocation.y -= fOffset;
	draw2dTextAt2dLocation(
		status, screenLocation, gGray80, drawGetWindowWidth(), drawGetWindowHeight() );
}

//-----------------------------------------------------------------------------
void NetworkPlugin::handleFunctionKeys (int keyNumber)
{
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
		if(this->m_uiPortPongCount <= this->m_iWaitForPongPort - this->m_uiStartPort)
		{
			this->m_iWaitForPongPort -= this->m_uiPortPongCount;
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
	this->InitializeServerPortAndPongCount();
	this->m_iWaitForPongPort = -1 * this->m_uiStartPort;
}

//-----------------------------------------------------------------------------
void NetworkPlugin::InitializeServerPortAndPongCount( void )
{
	this->m_uiStartPort = SERVER_PORT;
	this->m_uiPortPongCount = SERVER_PONG_COUNT;
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
	RakNetworkFactory::DestroyRakPeerInterface(this->m_pNetInterface);
	this->m_pNetInterface = NULL;
	printf("Destroyed peer.\n");
}

//-----------------------------------------------------------------------------
void NetworkPlugin::StartClientNetworkSession( void )
{
	SocketDescriptor& sd = this->m_kSocketDescriptor;
	this->m_uiStartPort = CLIENT_PORT;
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
				this->m_pNetInterface->SetMaximumIncomingConnections( maxIncoming );
				bStarted = true;
			}
		}
	}
	return bStarted;
}


//-----------------------------------------------------------------------------
void NetworkPlugin::StopNetworkSession( void )
{	
	this->CloseOpenConnections();	

	if( NULL != this->m_pNetInterface )
	{
		this->m_pNetInterface->Shutdown( 100,0 );
	}
	this->m_eNetworkSessionType = ENetworkSessionType_Undefined;
	this->m_kStats.reset();
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
		for(unsigned int us = 0; us < usCount; ++us)
		{
			this->m_pNetInterface->CloseConnection(
				kAddresses[us], true );
		}

		while(IsConnected())
	{
	#ifdef WIN32
		Sleep(1);
	#else
	  sleep(1);
	#endif
	}
	}
}

//-----------------------------------------------------------------------------
void NetworkPlugin::ReceivePackets( void )
{
	while(true)
	{
		Packet* pPacket = this->m_pNetInterface->Receive();
		if(0 != pPacket)
		{
			++this->m_kStats.m_uiPacketsReceived;
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
	switch (pPacket->data[0])
	{
	case ID_CONNECTION_ATTEMPT_FAILED:
		printf("ID_CONNECTION_ATTEMPT_FAILED\n");		
		break;
	case ID_NO_FREE_INCOMING_CONNECTIONS:
		printf("ID_NO_FREE_INCOMING_CONNECTIONS\n");		
		break;
	case ID_CONNECTION_REQUEST_ACCEPTED:
		printf("ID_CONNECTION_REQUEST_ACCEPTED\n");
		break;
	case ID_NEW_INCOMING_CONNECTION:
		printf("ID_NEW_INCOMING_CONNECTION from %s\n", pPacket->systemAddress.ToString());
		break;
	case ID_DISCONNECTION_NOTIFICATION:
		printf("ID_DISCONNECTION_NOTIFICATION\n");
		break;
	case ID_CONNECTION_LOST:
		printf("ID_CONNECTION_LOST\n");
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
		}
	}
}

//-----------------------------------------------------------------------------
void NetworkPlugin::ConnectToAddress( const NetworkAddress& kAddress )
{
	this->CloseOpenConnections();
	

	this->m_pNetInterface->Connect(
		kAddress.addressString.C_String(),
		kAddress.port,0,0);
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

void NetworkPlugin::Disconnect()
{
	this->CloseOpenConnections();
}

//-----------------------------------------------------------------------------
bool NetworkPlugin::PingForOtherPeers( const int iPort )
{
	if( false == this->WaitForPong() )
	{	
		unsigned short usMyPort = 
			this->m_pNetInterface->GetInternalID(UNASSIGNED_SYSTEM_ADDRESS,0).port;
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
	this->m_pNetInterface->SetNetworkIDManager(&this->m_kNetworkIdManager);
	this->m_kNetworkIdManager.SetIsNetworkIDAuthority( this->HasIdAuthority() );
}

//-----------------------------------------------------------------------------
bool NetworkPlugin::IsConnected() const
{
	return 0 < this->m_pNetInterface->NumberOfConnections();
}

