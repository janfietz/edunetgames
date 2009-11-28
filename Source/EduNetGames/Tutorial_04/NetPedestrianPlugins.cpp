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

#include "NetPedestrianReplicaConnection.h"
#include "NetPedestrianPlugin.h"

#include "OpenSteerUT/PluginArray.h"
#include "OpenSteerUT/GridPlugin.h"
#include "OpenSteerUT/CameraPlugin.h"

#include "EduNetConnect/ClientPlugin.h"
#include "EduNetConnect/PeerPlugin.h"

#include "EduNetCommon/EduNetDraw.h"

//-----------------------------------------------------------------------------
// now the basic network plugins
//-----------------------------------------------------------------------------

typedef PeerPlugin<NetPedestrianPlugin> TPedestrianPeerPlugin;
typedef ClientPlugin<NetPedestrianPlugin> TPedestrianClientPlugin;

//-----------------------------------------------------------------------------
class PedestrianPeerPlugin : public TPedestrianPeerPlugin
{
	ET_DECLARE_BASE(TPedestrianPeerPlugin)
public:
	PedestrianPeerPlugin( bool bAddToRegistry = true ):
	BaseClass( bAddToRegistry )
	{
		this->setGamePluginReplicaManager( &this->m_kReplicaManager );
		this->m_kReplicaManager.setPlugin( &this->m_kGamePlugin );

		// attach vehicle factory
		this->m_pkNetPedestrianFactory = new NetPedestrianReplicaFactory( &this->m_kReplicaManager );	
		this->m_kGamePlugin.setEntityFactory( this->m_pkNetPedestrianFactory );
	}
	OS_IMPLEMENT_CLASSNAME( PedestrianPeerPlugin )
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
	NetPedestrianReplicaFactory* m_pkNetPedestrianFactory;
	NetPedestrianReplicaManager m_kReplicaManager;

};

//-----------------------------------------------------------------------------
class PedestrianClientPlugin : public TPedestrianClientPlugin
{
	ET_DECLARE_BASE(TPedestrianClientPlugin)
public:
	PedestrianClientPlugin( bool bAddToRegistry = true ):
	BaseClass( bAddToRegistry )
	{
		this->setGamePluginReplicaManager( &this->m_kReplicaManager );
		this->m_kReplicaManager.setPlugin( &this->m_kGamePlugin );
		this->m_kGamePlugin.setEntityFactory( NULL );
	}

	OS_IMPLEMENT_CLASSNAME( PedestrianClientPlugin )
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

	NetPedestrianReplicaFactory* m_pkBoidFactory;
	NetPedestrianReplicaManager m_kReplicaManager;
};

//-----------------------------------------------------------------------------
// offline client
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
class OfflinePedestrianPlugin : public OpenSteer::PluginArray
{
	ET_DECLARE_BASE(OpenSteer::PluginArray)
public:
	OfflinePedestrianPlugin( bool bAddToRegistry = true ):
	BaseClass( bAddToRegistry )
	{
	}

	OS_IMPLEMENT_CLASSNAME( OfflinePedestrianPlugin )

	virtual void initGui(void* pkUserdata)
	{
		this->addPlugin( new OpenSteer::CameraPlugin() );
		this->addPlugin( new OpenSteer::GridPlugin() );
		this->addPlugin( new NetPedestrianPlugin( false ) );
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

OfflinePedestrianPlugin gPedestrianPlugin;

//-----------------------------------------------------------------------------
// render client plugin
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//class PedestrianRenderClientPlugin : public OpenSteer::PluginArrayPluginMixin<PedestrianClientPlugin>
class PedestrianRenderClientPlugin : public OpenSteer::PluginArray
{
	ET_DECLARE_BASE( OpenSteer::PluginArray )
public:
	PedestrianRenderClientPlugin( bool bAddToRegistry = true ):BaseClass( bAddToRegistry ) 
	{
	};
	virtual ~PedestrianRenderClientPlugin() {};

	OS_IMPLEMENT_CLASSNAME( PedestrianRenderClientPlugin )

	virtual void initGui(void* pkUserdata)
	{
		this->addPlugin( new OpenSteer::CameraPlugin() );
		this->addPlugin( new OpenSteer::GridPlugin() );
		this->addPlugin( new PedestrianClientPlugin( false ) );
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

PedestrianRenderClientPlugin gPedestrianClientPlugin( true );

//-----------------------------------------------------------------------------
// render server plugin
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
class PedestrianRenderPeerPlugin : public OpenSteer::PluginArray
{
	ET_DECLARE_BASE( OpenSteer::PluginArray )
public:
	PedestrianRenderPeerPlugin( bool bAddToRegistry = true ):BaseClass( bAddToRegistry ) 
	{
	};
	virtual ~PedestrianRenderPeerPlugin() {};

	OS_IMPLEMENT_CLASSNAME( PedestrianRenderPeerPlugin )

	virtual void initGui(void* pkUserdata)
	{
		this->addPlugin( new OpenSteer::CameraPlugin() );
		this->addPlugin( new OpenSteer::GridPlugin() );
		this->addPlugin( new PedestrianPeerPlugin( false ) );
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

PedestrianRenderPeerPlugin gPedestrianPeerPlugin( true );

//-----------------------------------------------------------------------------
// client server plugin
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
class PedestrianClientServerPlugin : public OpenSteer::PluginArray
{
	ET_DECLARE_BASE(OpenSteer::PluginArray);
public:

	PedestrianClientServerPlugin(bool bAddToRegistry = true);
	virtual ~PedestrianClientServerPlugin();

	OS_IMPLEMENT_CLASSNAME( PedestrianClientServerPlugin );

	//---------------------------------------------------------------------
	// interface AbstractPlugin
	virtual void initGui( void* pkUserdata );

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
PedestrianClientServerPlugin::PedestrianClientServerPlugin( bool bAddToRegistry ):
BaseClass( bAddToRegistry )
{
}

//-----------------------------------------------------------------------------
PedestrianClientServerPlugin::~PedestrianClientServerPlugin()
{

}

//-----------------------------------------------------------------------------
void PedestrianClientServerPlugin::initGui( void* pkUserdata ) 
{
	this->addPlugin( new OpenSteer::CameraPlugin() );
	this->addPlugin( new OpenSteer::GridPlugin() );
	this->addPlugin( new PedestrianPeerPlugin( false ) );
	this->addPlugin( new PedestrianClientPlugin( false ) );
	BaseClass::initGui( pkUserdata );
	GLUI* glui = ::getRootGLUI();
	GLUI_Panel* pluginPanel = static_cast<GLUI_Panel*>( pkUserdata );
};

PedestrianClientServerPlugin gClientServerPlugin;
