//-----------------------------------------------------------------------------
// Copyright (c) 2011, Jan Fietz, Cyrus Preuss
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

#include "NetCtfPlugin.h"
#include "NetCtfEntityFactory.h"

#include "OpenSteerUT/PluginArray.h"
#include "OpenSteerUT/GridPlugin.h"
#include "OpenSteerUT/CameraPlugin.h"
#include "OpenSteerUT/LocalPlayer.h"

#include "EduNetConnect/ClientPlugin.h"
#include "EduNetConnect/PeerPlugin.h"
#include "EduNetConnect/AbstractEntityReplica.h"
#include "EduNetConnect/AbstractEntityReplicaConnection.h"

#include "EduNetCommon/EduNetDraw.h"

//-----------------------------------------------------------------------------
void EduNetConnect::queryConnectionsSettings( ConnectSettings& kSettings )
{
	kSettings.uiClientStartPort = CLIENT_PORT;
	kSettings.uiServerStartPort = SERVER_PORT;
	kSettings.sessionPassword = "Tutorial7";
	kSettings.uiPortPongCount = 10;
}

//-----------------------------------------------------------------------------
// network plugins
//-----------------------------------------------------------------------------

typedef PeerPlugin<NetCtfPlugin> TCtfPeerPlugin;
typedef ClientPlugin<NetCtfPlugin> TCtfClientPlugin;

//-----------------------------------------------------------------------------
// peer plugin
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
class CtfPeerPlugin : public TCtfPeerPlugin
{
	ET_DECLARE_BASE(TCtfPeerPlugin)
public:
	CtfPeerPlugin( bool bAddToRegistry = true ):
	BaseClass( bAddToRegistry )
	{	
		this->setGamePluginReplicaManager( &this->m_kReplicaManager );
		this->m_kReplicaManager.setPlugin( &this->m_kGamePlugin );

		// remap the entity factory
		this->m_pkPeerEntityFactory = ET_NEW AbstractEntityReplicaFactory( &this->m_kReplicaManager );
		this->m_kGamePlugin.setEntityFactory( this->m_pkPeerEntityFactory );
	}

	virtual ~CtfPeerPlugin()
	{
		ET_SAFE_DELETE( this->m_pkPeerEntityFactory );
	}

	OS_IMPLEMENT_CLASSNAME( CtfPeerPlugin )
	virtual const char* name() const { return this->getClassName(); };

	//-------------------------------------------------------------------------
	void StartNetworkSession( void )
	{
		BaseClass::StartNetworkSession();
		this->m_pNetInterface->AttachPlugin(&this->m_kReplicaManager);
	}

	//-------------------------------------------------------------------------
	void CreateContent( void )
	{
		BaseClass::CreateContent();

	}

	//-------------------------------------------------------------------------
	void handleFunctionKeys (int keyNumber)
	{
	}	

	//-------------------------------------------------------------------------
	virtual void initGui( void* pkUserdata ) 
	{
		BaseClass::initGui( pkUserdata );
	};

	//-------------------------------------------------------------------------
	void DeleteContent( void )
	{	
		BaseClass::DeleteContent();
	}

private:
	AbstractEntityReplicaFactory* m_pkPeerEntityFactory;
	AbstractEntityReplicaManager m_kReplicaManager;
};

//-----------------------------------------------------------------------------
// client plugin
//-----------------------------------------------------------------------------
#define ET_TEST_PLAYERREPLICATION 1

//-----------------------------------------------------------------------------
class CtfClientPlugin : public TCtfClientPlugin
{
	ET_DECLARE_BASE(TCtfClientPlugin)
public:
	CtfClientPlugin( bool bAddToRegistry = true ):
	BaseClass( bAddToRegistry ),
		m_pkNetworkPlayer( NULL )
	{
		this->setGamePluginReplicaManager( &this->m_kReplicaManager );

#if ET_TEST_PLAYERREPLICATION
		this->m_pkClientEntityFactory = ET_NEW AbstractEntityReplicaFactory( &this->m_kReplicaManager );
#else
		this->m_pkClientEntityFactory = NULL;
#endif
		this->m_kReplicaManager.setPlugin( &this->m_kGamePlugin );
		this->m_kGamePlugin.setEntityFactory( NULL );
	}

	virtual ~CtfClientPlugin()
	{
		ET_SAFE_DELETE( this->m_pkClientEntityFactory );
	}

	OS_IMPLEMENT_CLASSNAME( CtfClientPlugin )
	virtual const char* name() const { return this->getClassName(); };

	//-------------------------------------------------------------------------
	void StartNetworkSession( void )
	{
		BaseClass::StartNetworkSession();
		this->m_pNetInterface->AttachPlugin( &this->m_kReplicaManager );
	}
	//-------------------------------------------------------------------------
	void CreateContent( void )
	{
		BaseClass::CreateContent();
#if ET_TEST_PLAYERREPLICATION
		this->m_kGamePlugin.setEntityFactory( this->m_pkClientEntityFactory );
		this->m_pkNetworkPlayer = this->m_pkClientEntityFactory->createEntity( OS_CID_PLAYER );
		osAbstractController* pkController = OpenSteer::CastToAbstractPlayer( this->m_pkNetworkPlayer )->accessController();
		if( NULL != pkController )
		{
			pkController->setCustomUpdated( OpenSteer::LocalPlayerController::accessLocalPlayerController() );
		}

		this->m_kGamePlugin.setEntityFactory( NULL );
		this->m_kGamePlugin.addPlayer( OpenSteer::CastToAbstractPlayer( this->m_pkNetworkPlayer ) );
#endif
	}

	//-------------------------------------------------------------------------
	void DeleteContent( void )
	{	
#if ET_TEST_PLAYERREPLICATION
		this->m_kGamePlugin.setEntityFactory( this->m_pkClientEntityFactory );
		this->m_kGamePlugin.removePlayer(  OpenSteer::CastToAbstractPlayer( this->m_pkNetworkPlayer ) );
		this->m_kGamePlugin.setEntityFactory( NULL );
		this->m_pkClientEntityFactory->destroyEntity( this->m_pkNetworkPlayer );
		this->m_pkNetworkPlayer = NULL;
#endif
		BaseClass::DeleteContent();
	}

	void update (const float currentTime, const float elapsedTime)
	{
		if( NULL != this->m_pkNetworkPlayer )
		{
			OpenSteer::CastToAbstractUpdated( m_pkNetworkPlayer )->update( currentTime, elapsedTime );
		}
		BaseClass::update( currentTime,  elapsedTime );
	};

private:
	AbstractEntityReplicaFactory* m_pkClientEntityFactory;
	AbstractEntityReplicaManager m_kReplicaManager;
	AbstractEntity* m_pkNetworkPlayer;
};

//-----------------------------------------------------------------------------
// offline client
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
class OfflineCtfPlugin : public OpenSteer::PluginArray
{
	ET_DECLARE_BASE(OpenSteer::PluginArray)
public:
	OfflineCtfPlugin( bool bAddToRegistry = true ):
	BaseClass( bAddToRegistry )
	{
	}

	OS_IMPLEMENT_CLASSNAME( OfflineCtfPlugin )

	virtual void initGui(void* pkUserdata)
	{
		this->addPlugin( ET_NEW OpenSteer::CameraPlugin() );
		this->addPlugin( ET_NEW OpenSteer::GridPlugin() );
		this->addPlugin( ET_NEW NetCtfPlugin( false ) );
		BaseClass::initGui( pkUserdata );
	}

	virtual void open(void)
	{
		BaseClass::open();
	}
	virtual void close(void)
	{
		BaseClass::close();
	}

};


//-----------------------------------------------------------------------------
// render client plugin
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
class CtfRenderClientPlugin : public OpenSteer::PluginArray
{
	ET_DECLARE_BASE(OpenSteer::PluginArray);
public:
	CtfRenderClientPlugin( bool bAddToRegistry = true ):BaseClass( bAddToRegistry ) 
	{
	};
	virtual ~CtfRenderClientPlugin() {};

	OS_IMPLEMENT_CLASSNAME( CtfRenderClientPlugin )

	virtual void initGui(void* pkUserdata)
	{
		this->addPlugin( ET_NEW OpenSteer::CameraPlugin() );
		this->addPlugin( ET_NEW OpenSteer::GridPlugin() );
		this->addPlugin( ET_NEW CtfClientPlugin( false ) );
		BaseClass::initGui( pkUserdata );
	}

	virtual void open(void)
	{
		BaseClass::open();
	}
	virtual void close(void)
	{
		BaseClass::close();
	}

};


//-----------------------------------------------------------------------------
// render server plugin
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
class CtfRenderPeerPlugin : public OpenSteer::PluginArray
{
	ET_DECLARE_BASE(OpenSteer::PluginArray);
public:
	CtfRenderPeerPlugin( bool bAddToRegistry = true ):BaseClass( bAddToRegistry ) 
	{
	};
	virtual ~CtfRenderPeerPlugin() {};

	OS_IMPLEMENT_CLASSNAME( CtfRenderPeerPlugin )

	virtual void initGui(void* pkUserdata)
	{
		this->addPlugin( ET_NEW OpenSteer::CameraPlugin() );
		this->addPlugin( ET_NEW OpenSteer::GridPlugin() );
		this->addPlugin( ET_NEW CtfPeerPlugin( false ) );
		BaseClass::initGui( pkUserdata );
	}

	virtual void open(void)
	{
		BaseClass::open();
	}
	virtual void close(void)
	{
		BaseClass::close();
	}

};


//-----------------------------------------------------------------------------
// client server plugin
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
class CtfClientServerPlugin : public OpenSteer::PluginArray
{
	ET_DECLARE_BASE(OpenSteer::PluginArray);
public:

	CtfClientServerPlugin(bool bAddToRegistry = true);
	virtual ~CtfClientServerPlugin();

	OS_IMPLEMENT_CLASSNAME( CtfClientServerPlugin );

	virtual void open(void)
	{
		BaseClass::open();
	}
	virtual void close(void)
	{
		BaseClass::close();
	}

	//---------------------------------------------------------------------
	// interface AbstractPlugin
	virtual void initGui( void* pkUserdata );
};

//-----------------------------------------------------------------------------
CtfClientServerPlugin::CtfClientServerPlugin( bool bAddToRegistry ):
BaseClass( bAddToRegistry )
{
}

//-----------------------------------------------------------------------------
CtfClientServerPlugin::~CtfClientServerPlugin()
{

}

//-----------------------------------------------------------------------------
void CtfClientServerPlugin::initGui( void* pkUserdata ) 
{
	this->addPlugin( ET_NEW OpenSteer::CameraPlugin() );
	this->addPlugin( ET_NEW OpenSteer::GridPlugin() );
	this->addPlugin( ET_NEW CtfPeerPlugin( false ) );
	this->addPlugin( ET_NEW CtfClientPlugin( false ) );
	BaseClass::initGui( pkUserdata );
	GLUI* glui = ::getRootGLUI();
	GLUI_Panel* pluginPanel = static_cast<GLUI_Panel*>( pkUserdata );
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------


OfflineCtfPlugin* ctfPlugin = NULL;
CtfRenderClientPlugin* ctfClientPlugin = NULL;
CtfRenderPeerPlugin* ctfPeerPlugin = NULL;
CtfClientServerPlugin* clientServerPlugin = NULL;

//-----------------------------------------------------------------------------
namespace EduNet
{
	void initializeDynamicPlugins( )
	{
		NetCtfPlugin::allocateEntityFactory();
		ctfPlugin = ET_NEW OfflineCtfPlugin();
		ctfClientPlugin = ET_NEW CtfRenderClientPlugin( true );
		ctfPeerPlugin = ET_NEW CtfRenderPeerPlugin( true );
		clientServerPlugin = ET_NEW CtfClientServerPlugin();
	}

	void shutdownDynamicPlugins( )
	{
		ET_SAFE_DELETE( ctfPlugin );
		ET_SAFE_DELETE( ctfClientPlugin );
		ET_SAFE_DELETE( ctfPeerPlugin );
		ET_SAFE_DELETE( clientServerPlugin );
		NetCtfPlugin::destroyEntityFactory();
	}
}
