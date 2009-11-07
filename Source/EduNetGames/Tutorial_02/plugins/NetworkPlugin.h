#pragma once

#include "EduNet/common/EduNetCommon.h"



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
	virtual void handleFunctionKeys (int keyNumber);
	virtual void printMiniHelpForFunctionKeys (void) const;

	virtual void CreateContent( void );
	virtual void DeleteContent( void );
	virtual bool IsConnected() const;
	virtual bool DoAutoConnect( void ) const;
	virtual bool Connect();
	virtual void Disconnect(){};

	virtual void StartNetworkSession( void ){};
	virtual void StopNetworkSession( void );

	void StartClientNetworkSession( void );

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

	void InitializeServerPortSettings( void );
	virtual void InitializeServerPortAndPongCount( void );



	void ReceivePackets( void );
	void ReceivedPongPacket( Packet* pPacket );
	void CheckPongTimeout( void );
	void CloseOpenConnections( void );
	bool WaitForPong( void ) const;

	int m_iWaitForPongPort;

	RakNetTime m_kPongEndTime;
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
		
	};
	virtual ~TNetworkPlugin(void) {};

	virtual void reset (void);
	void update (const float currentTime, const float elapsedTime);
	virtual void handleFunctionKeys (int keyNumber);
	virtual void printMiniHelpForFunctionKeys (void) const;

	const OpenSteer::AVGroup& allVehicles (void) const 
	{
		return m_kGamePlugin.allVehicles();
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