#include "NetworkPlugin.h"

#include "glui/GL/glui.h"


static const int CLIENT_PORT = 23456;
static const int SERVER_PORT = 12345;
static const int SERVER_PONG_COUNT = 32;
#define PONG_WAIT_TIMEOUT 1000 // 5s

//-----------------------------------------------------------------------------
NetworkPlugin::NetworkPlugin(bool bAddToRegistry):
	BaseClass( bAddToRegistry )
{

}

//-----------------------------------------------------------------------------
NetworkPlugin::~NetworkPlugin(void) 
{

}

//-----------------------------------------------------------------------------
void NetworkPlugin::initGui( void* pkUserdata )
{
	GLUI* glui = ::getRootGLUI();
	GLUI_Panel* pluginPanel = static_cast<GLUI_Panel*>( pkUserdata );
	glui->add_button_to_panel( pluginPanel, "Connect" );
}

//-----------------------------------------------------------------------------
void NetworkPlugin::InitializeServerPortSettings( void )
{
	this->InitializeServerPortAndPongCount();
	this->m_iWaitForPongPort = -1*this->m_uiStartPort;
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
	printf("Destroyed peer.\n");
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
void NetworkPlugin::StartClientNetworkSession( void )
{
	SocketDescriptor sd;
	sd.port = CLIENT_PORT;
	bool bStarted(false);
	while( false == bStarted )
	{
		while (SocketLayer::IsPortInUse(sd.port)==true)
			sd.port++;
		if( true == this->m_pNetInterface->Startup(1,100,&sd,1) )
		{
			this->m_pNetInterface->SetMaximumIncomingConnections(0);
			bStarted = true;
		}
	}
	printf("Starting client at port: %d.\n", sd.port);	
}

//-----------------------------------------------------------------------------
void NetworkPlugin::StopNetworkSession( void )
{	
	this->CloseOpenConnections();
	this->m_pNetInterface->Shutdown(100,0);	
}

//-----------------------------------------------------------------------------
void NetworkPlugin::CloseOpenConnections( void )
{
	DataStructures::List<SystemAddress> kAddresses;
	DataStructures::List<RakNetGUID> kGuids;
	unsigned short usCount = this->m_pNetInterface->NumberOfConnections();
	if( 0 < usCount )
	{
		this->m_pNetInterface->GetSystemList(kAddresses, kGuids );
		for(unsigned short us = 0; us < usCount; ++us)
		{
			this->m_pNetInterface->CloseConnection(
				kAddresses[us], true );
		}

		Sleep(1000);
	}
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
bool NetworkPlugin::DoAutoConnect( void ) const
{
	return true;
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
void NetworkPlugin::ReceivePackets( void )
{
	while(true)
	{
		Packet* pPacket = this->m_pNetInterface->Receive();
		if(0 !=  pPacket)
		{
			this->OnReceivedPacket( pPacket );
			this->m_pNetInterface->DeallocatePacket(pPacket);
		} else
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
		if(pPacket->systemAddress != this->m_pNetInterface->GetInternalID())
		{
			printf("Got pong from %s(%s) with time %i\n", 
				pPacket->systemAddress.ToString(),
				pPacket->guid.ToString(),
				this->m_kPongEndTime - RakNet::GetTime() );
			this->m_pNetInterface->Connect(
				pPacket->systemAddress.ToString(false),
				pPacket->systemAddress.port,0,0);
			this->m_iWaitForPongPort *= -1;
		}

	}
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

//-----------------------------------------------------------------------------
void NetworkPlugin::handleFunctionKeys (int keyNumber)
{
}

//-----------------------------------------------------------------------------
void NetworkPlugin::printMiniHelpForFunctionKeys (void) const
{
}