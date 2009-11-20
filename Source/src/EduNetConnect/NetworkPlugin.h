#ifndef __NETWORKPLUGIN_H__
#define __NETWORKPLUGIN_H__
//-----------------------------------------------------------------------------
// Copyright (c) 2009, Jan Fietz, Cyrus Preuss
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without modification, 
// are permitted provided that the following conditions are met:
// 
// * Redistributions of source code must retain the above copyright notice, 
//   this list of conditions and the following disclaimer.
// * Redistributions in binary form must reproduce the above copyright notice, 
//   this list of conditions and the following disclaimer in the documentation 
//   and/or other materials provided with the distribution.
// * Neither the name of EduNetGames nor the names of its contributors
//   may be used to endorse or promote products derived from this software
//   without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
// IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES 
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON 
// ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, 
// EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//-----------------------------------------------------------------------------

#define EDUNET_NO_OPENSTEER_INCLUDES 0 // include opensteer
#include "EduNetConnect/EduNetConnect.h"
#include "EduNetConnect/NetworkPlot.h"
#include "EduNetConnect/SimpleNetworkVehicle.h"

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
typedef struct TNetworkSimulatorData
{
public:
	TNetworkSimulatorData():
		enabled(0),
		packetloss(0.0f),
		minExtraPing(0),
		extraPingVariance(0){}
	int enabled;
	float packetloss;
	int minExtraPing;
	int extraPingVariance;
}NetworkSimulatorData;

//-----------------------------------------------------------------------------
typedef struct TReplicationParams
{
public:
	TReplicationParams():
		interval(30){}		
	RakNetTime interval;
	RakNet::PRO sendParameter;
}ReplicationParams;

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
	NetworkPlugin( bool bAddToRegistry = true );
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

	const NetworkAddress& getCurrentAddress( void ) const
	{
		return ((NetworkPlugin*)this)->accessCurrentAddress();
	};

	NetworkAddress& accessCurrentAddress( void )
	{
		if( NULL == this->m_pkAddress )
		{
			this->m_pkAddress = ET_NEW NetworkAddress();
		}
		return *this->m_pkAddress;
	};

	ENetworkSessionType getNetworkSessionType( void ) const
	{
		return this->m_eNetworkSessionType;
	}

	void UpdateNetworkSimulatorSettings( void );
	NetworkSimulatorData& GetNetworkSimulatorSettings( void )
	{
		return this->m_kSimulatorData;
	}


	void setReplicationInterval( RakNetTime additionalTime );
	ReplicationParams& getReplicationParams( void )
	{
		return this->m_kReplicationParams;
	}
	virtual void onChangedReplicationParams(
		const ReplicationParams& kParams ){};

	void getNetworkStatistics(RakNetStatistics& kStats);


protected:
	bool PingForOtherPeers( const int iPort );
	void AttachNetworkIdManager( void );

	void setLocalReplicaParamsFromManager( 
		class RakNet::ReplicaManager3* pkReplicaManager );

	void addReplicaGuiWithManager( void* pkUserdata );

	virtual void OnReceivedPacket( Packet* pPacket );
	virtual OpenSteer::AbstractPlugin* getHostedPlugin( void ) const;

	OpenSteer::AbstractVehicleMotionStatePlot m_kMotionStateProfile;

	RakPeerInterface* m_pNetInterface;
	NetworkIDManager* m_pkNetworkIdManager;

	unsigned int m_uiStartPort;
	unsigned int m_uiPortPongCount;

	int m_iWaitForPongPort;
	int m_bShowMotionStatePlot;
	SocketDescriptor m_kSocketDescriptor;

	RakNetTime m_kPongEndTime;
	ENetworkSessionType m_eNetworkSessionType;
	NetworkStats m_kStats;
	ReplicationParams m_kReplicationParams;

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
	
	void recordNetworkStatistics(const float currentTime,
		const float elapsedTime);
	void drawNetworkPlot(const float currentTime,
		const float elapsedTime);

	void AddNetworkSimulator( void* pkUserdata );

	NetworkAddress* m_pkAddress;
	NetworkSimulatorData m_kSimulatorData;
	
	int m_bDrawNetworkPlot;
	NetworkPlot m_kNetworkPlot;

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

	if( OpenSteer::SimpleVehicle::selectedVehicle != NULL )
	{
		// update motion state plot
		this->m_kMotionStateProfile.recordUpdate( OpenSteer::SimpleVehicle::selectedVehicle, currentTime, elapsedTime );
	}
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

#endif // __NETWORKPLUGIN_H__