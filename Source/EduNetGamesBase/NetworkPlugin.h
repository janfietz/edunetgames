#pragma once


#include "OpenSteer/Vec3.h"
#include "OpenSteer/Clock.h"
#include "OpenSteer/PlugIn.h"
#include "OpenSteer/Camera.h"
#include "OpenSteer/Utilities.h"

#include "RakNetworkFactory.h"
#include "RakPeerInterface.h"
//#include <RakNetTypes.h>

#define _WINSOCK2API_
#define _WINSOCKAPI_   /* Prevent inclusion of winsock.h in windows.h */
#include "SocketLayer.h"

#define SERVER_PORT 12345
#if 0

template 
class NetWorkPlugin< class PluginClass = PlugIn >  : public PlugIn
{
	PluginClass m_kGamePlugin; // content plugin


	// generic PlugIn actions: open, update, redraw, close and reset
	virtual void open (void) = 0;
	virtual void update (const float currentTime, const float elapsedTime) = 0;
	virtual void redraw (const float currentTime, const float elapsedTime) = 0;
	virtual void close (void) = 0;
	virtual void reset (void) = 0;


	virtual void open()
	{
	}


	virtual void close()
	{
	}

	void update (const float currentTime, const float elapsedTime)
	{
		if( !IsConnected() )
		{
			Connect();
		}
	}

	virtual void CreateContent()
	{

	}

	virtual void CreateContent() = 0;
	virtual bool IsConnected() const = 0;
	virtual bool Connect() = 0;
	virtual void Disconnect() = 0;



	//	virtual bool IsCreatorAuthority() const = 0;
};


class ClientPlugin : public NetWorkPlugin
{
	virtual void CreateContent();
};

void ClientPlugin::CreateContent()
{
	// nothing
}



class PeerPlugin: public NetWorkPlugin
{
};

void PeerPlugin::CreateContent()
{
	// smth
	// create boids ...
	m_kGamePlugin.open();

	// iterate over content and create replicas
}



class ClientServerPlugin  : public PlugIn
{
	ClientPlugin m_CP;
	PeerPlugin m_SP;
}
#endif

//-----------------------------------------------------------------------------
template < class PluginClass = OpenSteer::PlugIn  >
class NetworkPlugIn :
	public OpenSteer::PlugIn
{
public:
	NetworkPlugIn(void) {};
	virtual ~NetworkPlugIn(void) {};

	virtual void open();
	virtual void close();
	virtual void reset (void);
	void update (const float currentTime, const float elapsedTime);

	const OpenSteer::AVGroup& allVehicles (void) {return m_kGamePlugIn.allVehicles();}


	virtual void CreateContent( void );
	virtual void DeleteContent( void );
	virtual bool IsConnected() const{  return false; };// = 0;
	virtual bool Connect(){ return false; };// = 0;
	virtual void Disconnect(){};// = 0;

	virtual void StartNetworkSession( void ){};
	virtual void StopNetworkSession( void ){};


	PluginClass m_kGamePlugIn;

	RakPeerInterface* m_pNetInterface;
};

//-----------------------------------------------------------------------------
template < class PluginClass >
void NetworkPlugIn< PluginClass >::open()
{
 	this->StartNetworkSession();
// 
 	this->CreateContent();

}

//-----------------------------------------------------------------------------
template < class PluginClass >
void NetworkPlugIn< PluginClass >::close()
{
	this->DeleteContent();
// 
 	this->StopNetworkSession();
	

}

//-----------------------------------------------------------------------------
template < class PluginClass >
void NetworkPlugIn< PluginClass >::reset()
{
	m_kGamePlugIn.reset();
}



//-----------------------------------------------------------------------------
template < class PluginClass >
void NetworkPlugIn< PluginClass >::update (const float currentTime, const float elapsedTime)
{
	if( !IsConnected() )
	{
		Connect();
	}

	m_kGamePlugIn.update(currentTime, elapsedTime);
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
