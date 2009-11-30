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
#include "NetSoccerPlugin.h"
#include "NetSoccerClientPlugin.h"


#include "EduNetCommon/EduNetDraw.h"
#include "OpenSteerUT/AbstractVehicleGroup.h"
#include "OpenSteerUT/PluginArray.h"
#include "OpenSteerUT/CameraPlugin.h"

#include "EduNetConnect/PeerPlugin.h"

#include "EduNetConnect/AbstractEntityReplica.h"
#include "EduNetConnect/AbstractEntityReplicaConnection.h"

//-----------------------------------------------------------------------------
// network plugins
//-----------------------------------------------------------------------------
typedef PeerPlugin<NetSoccerPlugin> TSoccerPeerPlugin;

//-----------------------------------------------------------------------------
// soccer peer plugin
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
class SoccerPeerPlugin : public TSoccerPeerPlugin
{
	ET_DECLARE_BASE(TSoccerPeerPlugin)
public:
	SoccerPeerPlugin( bool bAddToRegistry = true ):
	BaseClass( bAddToRegistry )
	{	
		this->setGamePluginReplicaManager( &this->m_kReplicaManager );
		this->m_kReplicaManager.setPlugin( &this->m_kGamePlugin );

		// remap the entity factory
		this->m_pkNetBoidFactory = ET_NEW AbstractEntityReplicaFactory( &this->m_kReplicaManager );
		this->m_kGamePlugin.setEntityFactory( this->m_pkNetBoidFactory );
	}
	OS_IMPLEMENT_CLASSNAME( SoccerPeerPlugin )
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

	AbstractEntityReplicaFactory* m_pkNetBoidFactory;
	AbstractEntityReplicaManager m_kReplicaManager;	
};

//-----------------------------------------------------------------------------
// render server plugin
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
class RenderSoccerPeerPlugin : public OpenSteer::PluginArrayPluginMixin<SoccerPeerPlugin>
{
	ET_DECLARE_BASE( OpenSteer::PluginArrayPluginMixin<SoccerPeerPlugin> )
public:
	RenderSoccerPeerPlugin( bool bAddToRegistry = true ):BaseClass( bAddToRegistry ) 
	{
		this->addPlugin( ET_NEW OpenSteer::CameraPlugin() );
	};
	virtual ~RenderSoccerPeerPlugin() {};

	virtual float selectionOrderSortKey (void) const { return 3.0f ;}

	OS_IMPLEMENT_CLASSNAME( RenderSoccerPeerPlugin )
};

//-----------------------------------------------------------------------------
// client plugin
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// render server plugin
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
class RenderSoccerClientPlugin : public OpenSteer::PluginArrayPluginMixin<SoccerClientPlugin>
{
	ET_DECLARE_BASE( OpenSteer::PluginArrayPluginMixin<SoccerClientPlugin> )
public:
	RenderSoccerClientPlugin( bool bAddToRegistry = true ):BaseClass( bAddToRegistry ) 
	{
		this->addPlugin( ET_NEW OpenSteer::CameraPlugin() );
	};
	virtual ~RenderSoccerClientPlugin() {};

	virtual float selectionOrderSortKey (void) const { return 2.0f ;}

	OS_IMPLEMENT_CLASSNAME( RenderSoccerClientPlugin )
};

//-----------------------------------------------------------------------------
// client server plugin
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
class SoccerClientServerPlugin : public OpenSteer::PluginArray
{
	ET_DECLARE_BASE(OpenSteer::PluginArray);
public:

	SoccerClientServerPlugin(bool bAddToRegistry = true);
	virtual ~SoccerClientServerPlugin(){};

	virtual float selectionOrderSortKey (void) const { return 4.0f ;}

	OS_IMPLEMENT_CLASSNAME( SoccerClientServerPlugin );
};

//-----------------------------------------------------------------------------
SoccerClientServerPlugin::SoccerClientServerPlugin( bool bAddToRegistry ):
BaseClass( bAddToRegistry )
{
	this->addPlugin( ET_NEW SoccerPeerPlugin( false ) );
	this->addPlugin( ET_NEW SoccerClientPlugin( false ) );
	this->addPlugin( ET_NEW OpenSteer::CameraPlugin() );
}


//-----------------------------------------------------------------------------
class NetSoccerRenderOfflinePlugin :
            public OpenSteer::PluginArray
{
    ET_DECLARE_BASE ( OpenSteer::PluginArray )
public:
    NetSoccerRenderOfflinePlugin ( bool bAddToRegistry = true ) :
            BaseClass ( bAddToRegistry )
    {
        this->addPlugin ( ET_NEW NetSoccerPlugin ( false ) );
        this->addPlugin ( ET_NEW OpenSteer::CameraPlugin() );
    };

    virtual ~NetSoccerRenderOfflinePlugin() {};

	virtual float selectionOrderSortKey (void) const { return 1.0f ;}

    OS_IMPLEMENT_CLASSNAME ( SoccerOfflinePlugin )
};

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

NetSoccerRenderOfflinePlugin gNetNetSoccerRenderOfflinePlugin ( true );
RenderSoccerClientPlugin gSoccerClientPlugin( true );
RenderSoccerPeerPlugin gSoccerPeerPlugin( true );
SoccerClientServerPlugin gSoccerClientServerPlugin( true );

