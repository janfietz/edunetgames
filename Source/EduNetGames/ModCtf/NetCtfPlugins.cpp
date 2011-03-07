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

#include "NetCtfPlugins.h"
#include "NetCtfEntityFactory.h"

#include "OpenSteerUT/PluginArray.h"
#include "OpenSteerUT/GridPlugin.h"
#include "OpenSteerUT/CameraPlugin.h"
#include "OpenSteerUT/LocalPlayer.h"

#include "EduNetConnect/ClientPlugin.h"
#include "EduNetConnect/PeerPlugin.h"
#include "EduNetConnect/AbstractEntityReplica.h"
#include "EduNetConnect/AbstractEntityReplicaConnection.h"

#define ET_TEST_PLAYERREPLICATION 1

//-----------------------------------------------------------------------------
// network plugins
//-----------------------------------------------------------------------------

typedef PeerPlugin<NetCtfPlugin> TCtfPeerPlugin;
typedef ClientPlugin<NetCtfPlugin> TCtfClientPlugin;

//-----------------------------------------------------------------------------
// peer plugin
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
CtfPeerPlugin::CtfPeerPlugin( bool bAddToRegistry ):
BaseClass( bAddToRegistry )
{	
	this->setGamePluginReplicaManager( &this->m_kReplicaManager );
	this->m_kReplicaManager.setPlugin( &this->m_kGamePlugin );

	// remap the entity factory
	this->m_pkPeerEntityFactory = ET_NEW AbstractEntityReplicaFactory( &this->m_kReplicaManager );
	this->m_kGamePlugin.setEntityFactory( this->m_pkPeerEntityFactory );
}

//-----------------------------------------------------------------------------
CtfPeerPlugin::~CtfPeerPlugin()
{
	ET_SAFE_DELETE( this->m_pkPeerEntityFactory );
}

//-----------------------------------------------------------------------------
const char* CtfPeerPlugin::name() const 
{ 
	return this->getClassName(); 
}

//-----------------------------------------------------------------------------
void CtfPeerPlugin::StartNetworkSession( void )
{
	BaseClass::StartNetworkSession();
	this->m_pNetInterface->AttachPlugin(&this->m_kReplicaManager);
}

//-----------------------------------------------------------------------------
void CtfPeerPlugin::CreateContent( void )
{
	BaseClass::CreateContent();
}

//-----------------------------------------------------------------------------
void CtfPeerPlugin::handleFunctionKeys (int keyNumber)
{
}	

//-----------------------------------------------------------------------------
void CtfPeerPlugin::initGui( void* pkUserdata ) 
{
	BaseClass::initGui( pkUserdata );
}

//-----------------------------------------------------------------------------
void CtfPeerPlugin::DeleteContent( void )
{	
	BaseClass::DeleteContent();
}

//-----------------------------------------------------------------------------
// client plugin
//-----------------------------------------------------------------------------
#define ET_TEST_PLAYERREPLICATION 1

//-----------------------------------------------------------------------------
CtfClientPlugin::CtfClientPlugin( bool bAddToRegistry ):
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

//-----------------------------------------------------------------------------
CtfClientPlugin::~CtfClientPlugin()
{
	ET_SAFE_DELETE( this->m_pkClientEntityFactory );
}

//-----------------------------------------------------------------------------
const char* CtfClientPlugin::name() const { return this->getClassName(); };

//-----------------------------------------------------------------------------
void CtfClientPlugin::StartNetworkSession( void )
{
	BaseClass::StartNetworkSession();
	this->m_pNetInterface->AttachPlugin( &this->m_kReplicaManager );
}

//-----------------------------------------------------------------------------
void CtfClientPlugin::CreateContent( void )
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

//-----------------------------------------------------------------------------
void CtfClientPlugin::DeleteContent( void )
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

//-----------------------------------------------------------------------------
void CtfClientPlugin::update (const float currentTime, const float elapsedTime)
{
	if( NULL != this->m_pkNetworkPlayer )
	{
		OpenSteer::CastToAbstractUpdated( m_pkNetworkPlayer )->update( currentTime, elapsedTime );
	}
	BaseClass::update( currentTime,  elapsedTime );
}

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
CtfRenderClientPlugin::CtfRenderClientPlugin( bool bAddToRegistry ):BaseClass( bAddToRegistry ) 
{
}

//-----------------------------------------------------------------------------
CtfRenderClientPlugin::~CtfRenderClientPlugin() 
{
}

//-----------------------------------------------------------------------------
void CtfRenderClientPlugin::initGui(void* pkUserdata)
{
	
	BaseClass::initGui( pkUserdata );
}

//-----------------------------------------------------------------------------
void CtfRenderClientPlugin::open(void)
{
	BaseClass::open();
}

//-----------------------------------------------------------------------------
void CtfRenderClientPlugin::close(void)
{
	BaseClass::close();
}

void CtfRenderClientPlugin::prepareOpen( void )
{
	this->addPlugin( ET_NEW CtfClientPlugin( false ) );
}
//-----------------------------------------------------------------------------
// render server plugin
//-----------------------------------------------------------------------------
CtfRenderPeerPlugin::CtfRenderPeerPlugin( bool bAddToRegistry ):BaseClass( bAddToRegistry ) 
{
}

CtfRenderPeerPlugin::~CtfRenderPeerPlugin() {};

//-----------------------------------------------------------------------------
void CtfRenderPeerPlugin::initGui(void* pkUserdata)
{

	BaseClass::initGui( pkUserdata );
}

//-----------------------------------------------------------------------------
void CtfRenderPeerPlugin::open(void)
{
	BaseClass::open();
}

//-----------------------------------------------------------------------------
void CtfRenderPeerPlugin::close(void)
{
	BaseClass::close();
}

void CtfRenderPeerPlugin::prepareOpen( void )
{
	this->addPlugin( ET_NEW CtfPeerPlugin( false ) );
}
//-----------------------------------------------------------------------------
// client server plugin
//-----------------------------------------------------------------------------

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
void CtfClientServerPlugin::open(void)
{
	BaseClass::open();
}

//-----------------------------------------------------------------------------
void CtfClientServerPlugin::close(void)
{
	BaseClass::close();
}

//-----------------------------------------------------------------------------
void CtfClientServerPlugin::initGui( void* pkUserdata ) 
{
	
	BaseClass::initGui( pkUserdata );
	GLUI* glui = ::getRootGLUI();
	GLUI_Panel* pluginPanel = static_cast<GLUI_Panel*>( pkUserdata );
}
//////////////////////////////////////////////////////////////////////////
void CtfClientServerPlugin::prepareOpen( void )
{
	this->addPlugin( ET_NEW CtfPeerPlugin( false ) );
	this->addPlugin( ET_NEW CtfClientPlugin( false ) );
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
