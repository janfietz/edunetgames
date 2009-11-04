#pragma once

#include "EduNet/common/EduNetCommon.h"

static const int CLIENT_PORT = 23456;
static const int SERVER_PORT = 12345;
static const int SERVER_PONG_COUNT = 32;
#define PONG_WAIT_TIMEOUT 1000 // 5s

//-----------------------------------------------------------------------------
template < class PluginClass = OpenSteer::Plugin  >
class NetworkPlugIn :
	public OpenSteer::Plugin
{
	ET_DECLARE_BASE(OpenSteer::Plugin);
public:
	NetworkPlugIn(bool bAddToRegistry = true):
	  BaseClass( bAddToRegistry ),
		m_kGamePlugIn( false )
	{
		
	};
	virtual ~NetworkPlugIn(void) {};

	virtual void open(void);
	virtual void close(void);
	virtual void reset (void);
	void update (const float currentTime, const float elapsedTime);
	virtual void handleFunctionKeys (int keyNumber);
	virtual void printMiniHelpForFunctionKeys (void) const;


	const OpenSteer::AVGroup& allVehicles (void) const {return m_kGamePlugIn.allVehicles();}


	virtual void CreateContent( void );
	virtual void DeleteContent( void );
	virtual bool IsConnected() const;// = 0;
	virtual bool DoAutoConnect( void ) const;
	virtual bool Connect();
	virtual void Disconnect(){};// = 0;


	virtual void StartNetworkSession( void ){};
	virtual void StopNetworkSession( void );
	
	PluginClass m_kGamePlugIn;
protected:
	
	bool PingForOtherPeers( const int iPort );
	void AttachNetworkIdManager( void );

	virtual void OnReceivedPacket( Packet* pPacket );
	
	RakPeerInterface* m_pNetInterface;
	NetworkIDManager m_kNetworkIdManager;

	unsigned int m_uiStartPort;
	unsigned int m_uiPortPongCount;
private:
	
	void CreateNetworkInterface( void );
	void DestroyNetworkInterface( void );
	virtual bool HasIdAuthority( void ) const {return false;}

	void InitializeServerPortSettings( void )
	{
		this->InitializeServerPortAndPongCount();
		this->m_iWaitForPongPort = -1*this->m_uiStartPort;
	}
	virtual void InitializeServerPortAndPongCount( void )
	{
		this->m_uiStartPort = SERVER_PORT;
		this->m_uiPortPongCount = SERVER_PONG_COUNT;
	}



	void ReceivePackets( void );
	void ReceivedPongPacket( Packet* pPacket );
	void CheckPongTimeout( void );
	void CloseOpenConnections( void );
	bool WaitForPong( void ) const;

	int m_iWaitForPongPort;
	
	RakNetTime m_kPongEndTime;
};

//-----------------------------------------------------------------------------
template < class PluginClass >
void NetworkPlugIn< PluginClass >::open( void )
{	
	this->CreateNetworkInterface();
 	this->StartNetworkSession(); 
 	this->CreateContent();
}
//-----------------------------------------------------------------------------
template < class PluginClass >
void NetworkPlugIn< PluginClass >::CreateNetworkInterface( void )
{
	this->m_pNetInterface = RakNetworkFactory::GetRakPeerInterface();
	this->AttachNetworkIdManager();
	this->InitializeServerPortSettings();
}
//-----------------------------------------------------------------------------
template < class PluginClass >
void NetworkPlugIn< PluginClass >::close( void )
{
	this->StopNetworkSession();
	this->DeleteContent(); 	
	this->DestroyNetworkInterface();
}
//-----------------------------------------------------------------------------
template < class PluginClass >
void NetworkPlugIn< PluginClass >::StopNetworkSession( void )
{	
	this->CloseOpenConnections();
	this->m_pNetInterface->Shutdown(100,0);	
}

//-----------------------------------------------------------------------------
template < class PluginClass >
void NetworkPlugIn< PluginClass >::DestroyNetworkInterface( void )
{
	RakNetworkFactory::DestroyRakPeerInterface(this->m_pNetInterface);
	printf("Destroyed peer.\n");
}

//-----------------------------------------------------------------------------
template < class PluginClass >
void NetworkPlugIn< PluginClass >::CloseOpenConnections( void )
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
template < class PluginClass >
void NetworkPlugIn< PluginClass >::reset()
{
	//m_kGamePlugIn.reset();
}



//-----------------------------------------------------------------------------
template < class PluginClass >
void NetworkPlugIn< PluginClass >::update (const float currentTime, const float elapsedTime)
{
	if( !IsConnected()&& DoAutoConnect() )
	{
		Connect();
	}

	if(true == this->WaitForPong() )
	{
		this->CheckPongTimeout();
	}

	this->ReceivePackets();

	m_kGamePlugIn.update(currentTime, elapsedTime);
}
//-----------------------------------------------------------------------------
template < class PluginClass >
bool NetworkPlugIn< PluginClass >::DoAutoConnect( void ) const
{
	return true;
}
//-----------------------------------------------------------------------------
template < class PluginClass >
void NetworkPlugIn< PluginClass >::CheckPongTimeout( void )
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
template < class PluginClass >
void NetworkPlugIn< PluginClass >::CreateContent( void )
{

}


//-----------------------------------------------------------------------------
template < class PluginClass >
void NetworkPlugIn< PluginClass >::DeleteContent( void )
{

}
//-----------------------------------------------------------------------------
template < class PluginClass >
void NetworkPlugIn< PluginClass >::ReceivePackets( void )
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
template < class PluginClass >
void NetworkPlugIn< PluginClass >::OnReceivedPacket( Packet* pPacket )
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
template < class PluginClass >
void NetworkPlugIn< PluginClass >::ReceivedPongPacket( Packet* pPacket )
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
template < class PluginClass >
bool NetworkPlugIn< PluginClass >::WaitForPong( void ) const
{
	return ( 0 <= this->m_iWaitForPongPort );
}
//-----------------------------------------------------------------------------
template < class PluginClass >
bool NetworkPlugIn< PluginClass >::Connect()
{	
	return this->PingForOtherPeers( -1* this->m_iWaitForPongPort);	
}

//-----------------------------------------------------------------------------
template < class PluginClass >
bool NetworkPlugIn< PluginClass >::PingForOtherPeers( const int iPort )
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
template < class PluginClass >
void NetworkPlugIn< PluginClass >::AttachNetworkIdManager( void )
{
	this->m_pNetInterface->SetNetworkIDManager(&this->m_kNetworkIdManager);
	this->m_kNetworkIdManager.SetIsNetworkIDAuthority( this->HasIdAuthority() );
}

//-----------------------------------------------------------------------------
template < class PluginClass >
bool NetworkPlugIn<PluginClass>::IsConnected() const
{
	return 0 < this->m_pNetInterface->NumberOfConnections();
}

//-----------------------------------------------------------------------------
template < class PluginClass >
void NetworkPlugIn<PluginClass>::handleFunctionKeys (int keyNumber)
{
	this->m_kGamePlugIn.handleFunctionKeys(keyNumber);
}
//-----------------------------------------------------------------------------
template < class PluginClass >
void NetworkPlugIn<PluginClass>::printMiniHelpForFunctionKeys (void) const
{
	this->m_kGamePlugIn.printMiniHelpForFunctionKeys();
}