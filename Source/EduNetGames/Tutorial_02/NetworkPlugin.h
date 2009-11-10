#pragma once

#include "EduNet/common/EduNetCommon.h"

#define CLIENT_PORT  23456
#define SERVER_PORT  12345

//-----------------------------------------------------------------------------
enum ENetworkSessionType
{
	ENetworkSessionType_Undefined,
	ENetworkSessionType_Client,
	ENetworkSessionType_Peer,
	ENetworkSessionType_Count
};


//-----------------------------------------------------------------------------
typedef struct TNetworkStats
{
	TNetworkStats():
	m_uiPacketsReceived(0)
	{
	}

	void reset()
	{
		m_uiPacketsReceived = 0;
	}

	size_t m_uiPacketsReceived;
} NetworkStats;
//-----------------------------------------------------------------------------
typedef struct TNetworkAddress
{
public:
	TNetworkAddress():
		addressString("127.0.0.1"),
		port(SERVER_PORT){}
	RakNet::RakString addressString;
	unsigned short port;
}NetworkAddress;

//-----------------------------------------------------------------------------
class AbstractNetworkPlugin
{
public:
	virtual ~AbstractNetworkPlugin(void){};

	virtual void CreateContent( void ) = 0;
	virtual void DeleteContent( void ) = 0;
	virtual bool IsConnected() const = 0;
	virtual bool DoAutoConnect( void ) const = 0;
	virtual bool Connect() = 0;
	virtual void Disconnect() = 0;


};

//-----------------------------------------------------------------------------
class NetworkPlugin :
	public OpenSteer::Plugin, public AbstractNetworkPlugin
{
	ET_DECLARE_BASE( OpenSteer::Plugin );
public:
	NetworkPlugin(bool bAddToRegistry = true);
	virtual ~NetworkPlugin(void);

	//----------------------------------------------------------------------------
	// OpenSteer::Plugin interface
	virtual void initGui( void* pkUserdata );
	virtual void open(void);
	virtual void close(void);
	virtual void reset (void);
	void update (const float currentTime, const float elapsedTime);
	void redraw (const float currentTime, const float elapsedTime);
	virtual void handleFunctionKeys (int keyNumber);
	virtual void printMiniHelpForFunctionKeys (void) const;

	virtual void CreateContent( void );
	virtual void DeleteContent( void );
	virtual bool IsConnected() const;
	virtual bool DoAutoConnect( void ) const;
	virtual bool Connect();
	virtual void Disconnect();

	virtual void StartNetworkSession( void ){};
	virtual void StopNetworkSession( void );

	void StartClientNetworkSession( void );
	bool StartupNetworkSession( 
		SocketDescriptor& sd, unsigned short maxAllowed, unsigned short maxIncoming );

	void ConnectToAddress( const NetworkAddress& kAddress );

	NetworkAddress& GetCurrentAddress( void )
	{
		return this->m_kAddress;
	};

protected:


	bool PingForOtherPeers( const int iPort );
	void AttachNetworkIdManager( void );

	virtual void OnReceivedPacket( Packet* pPacket );
	virtual OpenSteer::AbstractPlugin* getHostedPlugin( void ) const;

	RakPeerInterface* m_pNetInterface;
	NetworkIDManager m_kNetworkIdManager;

	unsigned int m_uiStartPort;
	unsigned int m_uiPortPongCount;

	int m_iWaitForPongPort;
	SocketDescriptor m_kSocketDescriptor;

	RakNetTime m_kPongEndTime;
	ENetworkSessionType m_eNetworkSessionType;
	NetworkStats m_kStats;

	// settings
	int m_bAutoConnect;
private:

	void CreateNetworkInterface( void );
	void DestroyNetworkInterface( void );
	virtual bool HasIdAuthority( void ) const {return false;}

	void InitializeServerPortSettings( void );
	virtual void InitializeServerPortAndPongCount( void );

	virtual bool AddConnectBox( void ){ return false; }

	void ReceivePackets( void );
	void ReceivedPongPacket( Packet* pPacket );
	void CheckPongTimeout( void );
	void CloseOpenConnections( void );
	bool WaitForPong( void ) const;	

	NetworkAddress m_kAddress;

};

//-----------------------------------------------------------------------------
template < class PluginClass = OpenSteer::Plugin  >
class TNetworkPlugin :
	public NetworkPlugin
{
	ET_DECLARE_BASE( NetworkPlugin );
public:
	TNetworkPlugin(bool bAddToRegistry = true):
	  BaseClass( bAddToRegistry ),
		m_kGamePlugin( false )
	{
		this->m_kGamePlugin.setParentPlugin( this );
	};
	virtual ~TNetworkPlugin(void) {};

	virtual void reset (void);
	virtual void update (const float currentTime, const float elapsedTime);
	virtual void handleFunctionKeys (int keyNumber);
	virtual void printMiniHelpForFunctionKeys (void) const;

	virtual const OpenSteer::AVGroup& allVehicles (void) const 
	{
		return m_kGamePlugin.allVehicles();
	}
	
	virtual OpenSteer::AbstractPlugin* getHostedPlugin( void ) const
	{
		return ((OpenSteer::AbstractPlugin*)&this->m_kGamePlugin);
	}

	PluginClass m_kGamePlugin;
};


//-----------------------------------------------------------------------------
template < class PluginClass >
void TNetworkPlugin< PluginClass >::reset()
{
	//m_kGamePlugin.reset();
}

//-----------------------------------------------------------------------------
template < class PluginClass >
void TNetworkPlugin< PluginClass >::update (const float currentTime, const float elapsedTime)
{
	BaseClass::update( currentTime, elapsedTime );
	m_kGamePlugin.update(currentTime, elapsedTime);
}

//-----------------------------------------------------------------------------
template < class PluginClass >
void TNetworkPlugin<PluginClass>::handleFunctionKeys (int keyNumber)
{
	BaseClass::handleFunctionKeys( keyNumber );
	this->m_kGamePlugin.handleFunctionKeys(keyNumber);
}
//-----------------------------------------------------------------------------
template < class PluginClass >
void TNetworkPlugin<PluginClass>::printMiniHelpForFunctionKeys (void) const
{
	BaseClass::printMiniHelpForFunctionKeys( );
	this->m_kGamePlugin.printMiniHelpForFunctionKeys();
}