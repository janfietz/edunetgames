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

#include "NetCtfPlugin.h"
#include "NetCtfVehicleFactory.h"

#include "OpenSteerUT/PluginArray.h"
#include "OpenSteerUT/GridPlugin.h"
#include "OpenSteerUT/CameraPlugin.h"

#include "EduNetConnect/ClientPlugin.h"
#include "EduNetConnect/PeerPlugin.h"
#include "EduNetConnect/AbstractVehicleReplica.h"
#include "EduNetConnect/AbstractVehicleReplicaConnection.h"

#include "EduNetCommon/EduNetDraw.h"

//-----------------------------------------------------------------------------
// now the basic network plugins
//-----------------------------------------------------------------------------
// now 1 global vehicle factory
NetCtfVehicleFactory gOnlineNetCtfVehicleFactory;

typedef PeerPlugin<NetCtfPlugin> TCtfPeerPlugin;
typedef ClientPlugin<NetCtfPlugin> TCtfClientPlugin;

//-----------------------------------------------------------------------------
class CtfPeerPlugin : public TCtfPeerPlugin
{
	ET_DECLARE_BASE(TCtfPeerPlugin)
public:
	CtfPeerPlugin( bool bAddToRegistry = true ):
	BaseClass( bAddToRegistry )
	{	

		this->m_kReplicaManager.setPlugin( &this->m_kGamePlugin );

		this->setLocalReplicaParamsFromManager( &this->m_kReplicaManager);

		// attach vehicle factory
		this->m_pkNetCtfFactory = new AbstractVehicleReplicaFactory( &this->m_kReplicaManager );
		AbstractVehicleReplica::setAbstractVehicleFactory( &gOnlineNetCtfVehicleFactory );
		this->m_kGamePlugin.setVehicleFactory( this->m_pkNetCtfFactory );
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
		switch (keyNumber)
		{
		case 101:  setReplicationInterval(5);         break; //GLUT_KEY_UP
		case 103:  setReplicationInterval(-5);    break; //GLUT_KEY_DOWN  
		default: BaseClass::handleFunctionKeys(keyNumber);
		}	
	}	

	//-------------------------------------------------------------------------
	virtual void initGui( void* pkUserdata ) 
	{
		BaseClass::initGui( pkUserdata );
		GLUI* glui = ::getRootGLUI();
		GLUI_Panel* pluginPanel = static_cast<GLUI_Panel*>( pkUserdata );

		this->addReplicaGuiWithManager( pkUserdata );
	};

	//-------------------------------------------------------------------------
	void DeleteContent( void )
	{	
		BaseClass::DeleteContent();
	}

	virtual void onChangedReplicationParams(
		const ReplicationParams& kParams )
	{
		this->m_kReplicaManager.SetAutoSerializeInterval(
			kParams.interval);
		this->m_kReplicaManager.SetDefaultPacketReliability(
			kParams.sendParameter.reliability);
		this->m_kReplicaManager.SetDefaultPacketPriority(
			kParams.sendParameter.priority);
	};

private:
	struct ReplicationParams
	{
		ReplicationParams():
	interval(30){}		
	RakNetTime interval;
	};
	AbstractVehicleReplicaFactory* m_pkNetCtfFactory;
	AbstractVehicleReplicaManager m_kReplicaManager;

	ReplicationParams m_kReplicationSettings;
};


//-----------------------------------------------------------------------------
class CtfClientPlugin : public TCtfClientPlugin
{
	ET_DECLARE_BASE(TCtfClientPlugin)
public:
	CtfClientPlugin( bool bAddToRegistry = true ):
	BaseClass( bAddToRegistry )
	{
		this->m_kReplicaManager.setPlugin( &this->m_kGamePlugin );
		this->m_kGamePlugin.setVehicleFactory( NULL );
		AbstractVehicleReplica::setAbstractVehicleFactory( &gOnlineNetCtfVehicleFactory );
	}

	OS_IMPLEMENT_CLASSNAME( CtfClientPlugin )
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
	void DeleteContent( void )
	{	
		BaseClass::DeleteContent();
	}
private:

	AbstractVehicleReplicaFactory* m_pkBoidFactory;
	AbstractVehicleReplicaManager m_kReplicaManager;
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
		this->addPlugin( new OpenSteer::GridPlugin() );
		this->addPlugin( new NetCtfPlugin( false ) );
		this->addPlugin( new OpenSteer::CameraPlugin() );
	}

	OS_IMPLEMENT_CLASSNAME( OfflineCtfPlugin )
};

OfflineCtfPlugin gCtfPlugin;

//-----------------------------------------------------------------------------
// render client plugin
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
class CtfRenderClientPlugin : public OpenSteer::PluginArrayPluginMixin<CtfClientPlugin>
{
	ET_DECLARE_BASE( OpenSteer::PluginArrayPluginMixin<CtfClientPlugin> )
public:
	CtfRenderClientPlugin( bool bAddToRegistry = true ):BaseClass( bAddToRegistry ) 
	{
		this->addPlugin( new OpenSteer::GridPlugin() );
		this->addPlugin( new OpenSteer::CameraPlugin() );
	};
	virtual ~CtfRenderClientPlugin() {};

	OS_IMPLEMENT_CLASSNAME( CtfRenderClientPlugin )
};

CtfRenderClientPlugin gCtfClientPlugin( true );

//-----------------------------------------------------------------------------
// render server plugin
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
class CtfRenderPeerPlugin : public OpenSteer::PluginArrayPluginMixin<CtfPeerPlugin>
{
	ET_DECLARE_BASE( OpenSteer::PluginArrayPluginMixin<CtfPeerPlugin> )
public:
	CtfRenderPeerPlugin( bool bAddToRegistry = true ):BaseClass( bAddToRegistry ) 
	{
		this->addPlugin( new OpenSteer::GridPlugin() );
		this->addPlugin( new OpenSteer::CameraPlugin() );
	};
	virtual ~CtfRenderPeerPlugin() {};

	OS_IMPLEMENT_CLASSNAME( CtfRenderPeerPlugin )
};

CtfRenderPeerPlugin gCtfPeerPlugin( true );

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

	//---------------------------------------------------------------------
	// interface AbstractPlugin
	virtual void initGui( void* pkUserdata );
};

//-----------------------------------------------------------------------------
CtfClientServerPlugin::CtfClientServerPlugin( bool bAddToRegistry ):
BaseClass( bAddToRegistry )
{
	this->addPlugin( new OpenSteer::GridPlugin() );
	this->addPlugin( new CtfPeerPlugin( false ) );
	this->addPlugin( new CtfClientPlugin( false ) );
	this->addPlugin( new OpenSteer::CameraPlugin() );
}

//-----------------------------------------------------------------------------
CtfClientServerPlugin::~CtfClientServerPlugin()
{

}

//-----------------------------------------------------------------------------
void CtfClientServerPlugin::initGui( void* pkUserdata ) 
{
	BaseClass::initGui( pkUserdata );
	GLUI* glui = ::getRootGLUI();
	GLUI_Panel* pluginPanel = static_cast<GLUI_Panel*>( pkUserdata );
};

CtfClientServerPlugin gClientServerPlugin;
