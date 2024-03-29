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
#include <mgf/wx/WxWidgets.h>

#define EDUNET_NO_OPENSTEER_INCLUDES 0 // include opensteer
#include "EduNetConnect/EduNetConnect.h"
#include "EduNetConnect/ReplicationTypes.h"

#include "EduNetConnect/NetworkPlot.h"
#include "EduNetConnect/SimpleNetworkVehicle.h"
#include "EduNetConnect/AbstractEntityReplica.h"

#include "OpenSteer/Plugin.h"
#include "OpenSteerUT/AbstractVehicleMotionStatePlot.h"


namespace EduNetConnect
{
	extern void queryConnectionsSettings( ConnectSettings& kSettings );
}


//-----------------------------------------------------------------------------
class AbstractNetworkPlugin
{
public:
	virtual ~AbstractNetworkPlugin(void){};

	virtual void CreateContent( void ) ET_ABSTRACT;
	virtual void DeleteContent( void ) ET_ABSTRACT;
	virtual bool IsConnected() const ET_ABSTRACT;
	virtual bool DoAutoConnect( void ) const ET_ABSTRACT;
	virtual bool Connect() ET_ABSTRACT;
	virtual void Disconnect() ET_ABSTRACT;

	virtual OpenSteer::AbstractEntityFactory* getGamePluginEntityFactory( void ) const ET_ABSTRACT;
	virtual OpenSteer::AbstractPlugin* getHostedPlugin( void ) const ET_ABSTRACT;

	virtual AbstractEntityReplica* allocEntityReplica(
		OpenSteer::AbstractPlugin* pPlugin,
		OpenSteer::EntityClassId classId,
		bool bIsRemoteObject,  bool bClientReplica ) const ET_ABSTRACT;

	virtual AbstractEntityReplica* createLocalEntityReplica(
		OpenSteer::AbstractPlugin* pPlugin,
		OpenSteer::EntityClassId classId,
		bool bIsRemoteObject,  bool bClientReplica ) const ET_ABSTRACT;

};

//-----------------------------------------------------------------------------
class NetworkPlugin :
	public OpenSteer::Plugin, public AbstractNetworkPlugin
{
	ET_DECLARE_BASE( OpenSteer::Plugin );
public:

	enum GuiIds
	{
		id_MotionState = 1,
		id_PlotNetwork,
		id_AutoConnect,
		id_NetAddress,
		id_NetPort,
		id_NetConnect,
		id_SimEnable,
		id_SimPacketLoss,
		id_SimDelay,
		id_SimVar,
	};

	NetworkPlugin( bool bAddToRegistry = true );
	virtual ~NetworkPlugin(void);

	//----------------------------------------------------------------------------
	// OpenSteer::Plugin interface
	virtual wxWindow* prepareGui ( wxWindow* parent ) OS_OVERRIDE;
	virtual void open(void);
	virtual void close(void);
	virtual void reset (void);
	void update (const float currentTime, const float elapsedTime);
	virtual void redraw ( OpenSteer::AbstractRenderer* pRenderer, const float currentTime,
		const float elapsedTime) OS_OVERRIDE;
	virtual void handleFunctionKeys (int keyNumber);
	virtual void printMiniHelpForFunctionKeys (void) const;




	virtual void CreateContent( void );
	virtual void DeleteContent( void );
	virtual bool IsConnected() const;
	virtual bool DoAutoConnect( void ) const;
	virtual bool Connect();
	virtual void Disconnect();

	virtual AbstractEntityReplica* allocEntityReplica(
		OpenSteer::AbstractPlugin* pPlugin,
		OpenSteer::EntityClassId classId,
		bool bIsRemoteObject,  bool bClientReplica ) const;

	virtual AbstractEntityReplica* createLocalEntityReplica(
		OpenSteer::AbstractPlugin* pPlugin,
		OpenSteer::EntityClassId classId,
		bool bIsRemoteObject,  bool bClientReplica ) const;


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


	void incrementReplicationInterval( int additionalTime );


	void updateMotionStateProfile( const float currentTime, const float elapsedTime );
	static void recordNetUpdate(
		osAbstractVehicle* pkVehicle, const float currentTime, const float elapsedTime );

	void DrawNetworkPlot(bool val) { m_bDrawNetworkPlot = val; }
	void AutoConnect(bool val) { m_bAutoConnect = val; }

protected:
	bool PingForOtherPeers( const int iPort );
	void AttachNetworkIdManager( void );

	void gatherNetworkStatistics( RakNetStatistics& kStats );

	virtual void OnReceivedPacket( Packet* pPacket );
	virtual OpenSteer::AbstractPlugin* getHostedPlugin( void ) const;
        virtual unsigned short getMaxIncomingConnections( void ) const;

	void setGamePluginReplicaManager( RakNet::ReplicaManager3* pkReplicaManager );

	static OpenSteer::AbstractVehicleMotionStatePlot ms_kMotionStateProfile;

	RakPeerInterface* m_pNetInterface;
	NetworkIDManager* m_pkNetworkIdManager;
	
	ConnectSettings m_ConnectionSettings;
	unsigned int m_uiStartPort;

	int m_iWaitForPongPort;
	SocketDescriptor m_kSocketDescriptor;

	RakNetTime m_kPongEndTime;
	ENetworkSessionType m_eNetworkSessionType;
	NetworkStats m_kStats;

	// settings
	bool m_bAutoConnect;
	
private:

	void CreateNetworkInterface( void );
	void DestroyNetworkInterface( void );
	virtual bool HasIdAuthority( void ) const {return false;}

	void InitializeServerPortSettings( void );	

	virtual bool addConnectBox( void ){ return false; }

	void ReceivePackets( void );
	void ReceivedPongPacket( Packet* pPacket );
	void CheckPongTimeout( void );
	void CloseOpenConnections( void );
	bool WaitForPong( void ) const;

	void recordNetworkStatistics(const float currentTime,
		const float elapsedTime);
	void drawNetworkPlot(OpenSteer::AbstractRenderer* pRenderer,
		const float currentTime,
		const float elapsedTime);

        void acceptConnections( void );

	void setIncommingPassword( void );

	RakNet::ReplicaManager3* m_pkGamePluginReplicaManager;
	class EduNetConnectGuiReplicationSettings* m_pReplicaManagerGui;

	NetworkAddress* m_pkAddress;
	NetworkSimulatorData m_kSimulatorData;

	bool m_bDrawNetworkPlot;
	
	NetworkPlot m_kNetworkPlot;
	bool m_bWaitForConnection;

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
		this->m_pkGamePluginEntityFactory = this->m_kGamePlugin.getEntityFactory();
		this->m_kGamePlugin.setParentPlugin( this );
	};
	virtual ~TNetworkPlugin(void)
	{

	};

	virtual wxWindow* prepareGui ( wxWindow* parent ) OS_OVERRIDE;

	virtual void reset (void);
	virtual void update (const float currentTime, const float elapsedTime);
	virtual void handleFunctionKeys (int keyNumber);
	virtual void printMiniHelpForFunctionKeys (void) const;

	virtual const OpenSteer::AVGroup& allVehicles( void ) const
	{
		return m_kGamePlugin.allVehicles();
	}

	virtual OpenSteer::AVGroup& allVehicles( void )
	{
		return m_kGamePlugin.allVehicles();
	}

	virtual OpenSteer::AbstractPlayerGroup& allPlayers( void )
	{
		return m_kGamePlugin.allPlayers();
	}
	virtual const OpenSteer::AbstractPlayerGroup& allPlayers( void ) const
	{
		return m_kGamePlugin.allPlayers();
	}

	virtual const OpenSteer::ObstacleGroup& allObstacles( void ) const
	{
		return m_kGamePlugin.allObstacles();
	}

	virtual OpenSteer::ObstacleGroup& allObstacles( void )
	{
		return m_kGamePlugin.allObstacles();
	}

	virtual OpenSteer::AbstractPlugin* getHostedPlugin( void ) const
	{
		return ((OpenSteer::AbstractPlugin*)&this->m_kGamePlugin);
	}

	//----------------------------------------------------------------------------
	// NetworkPlugin interface
	virtual OpenSteer::AbstractEntityFactory* getGamePluginEntityFactory( void ) const
	{
		return this->m_pkGamePluginEntityFactory;
	}
protected:
	PluginClass m_kGamePlugin;
	OpenSteer::AbstractEntityFactory* m_pkGamePluginEntityFactory;
};

//-----------------------------------------------------------------------------
template < class PluginClass >
wxWindow* TNetworkPlugin< PluginClass >::prepareGui ( wxWindow* parent )
{
	wxWindow* parentWindow = BaseClass::prepareGui( parent );
	wxWindow* gameWindow = m_kGamePlugin.prepareGui(parentWindow);
	parentWindow->GetSizer()->Add(gameWindow);
	return parentWindow;
}
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
	if( false == this->isEnabled() )
	{
		return;
	}
	const bool bIsRemoteObject = m_kGamePlugin.isRemoteObject();
	if( true == bIsRemoteObject )
	{
		// client side
		// read net first
		BaseClass::update( currentTime, elapsedTime );
		m_kGamePlugin.update(currentTime, elapsedTime);
	}
	else
	{
		// peer side
		// update first and write updated net data
		m_kGamePlugin.update(currentTime, elapsedTime);
		BaseClass::update( currentTime, elapsedTime );
	}

	this->updateMotionStateProfile( currentTime, elapsedTime );
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