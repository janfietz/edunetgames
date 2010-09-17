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

#include "NetPedestrianPlugins.h"
#include "NetPedestrianReplicaConnection.h"


#include "OpenSteerUT/GridPlugin.h"
#include "OpenSteerUT/CameraPlugin.h"

#include "EduNetConnect/ClientPlugin.h"
#include "EduNetConnect/PeerPlugin.h"

#include "EduNetCommon/EduNetDraw.h"

//-----------------------------------------------------------------------------
// now the basic network plugins
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// offline client
//-----------------------------------------------------------------------------

void OfflinePedestrianPlugin::initGui(void* pkUserdata)
{
	this->addPlugin( ET_NEW OpenSteer::CameraPlugin() );
	this->addPlugin( ET_NEW OpenSteer::GridPlugin() );
	this->addPlugin( ET_NEW NetPedestrianPlugin( false ) );
	BaseClass::initGui( pkUserdata );
}


//-----------------------------------------------------------------------------
// render client plugin
//-----------------------------------------------------------------------------

void PedestrianRenderClientPlugin::initGui(void* pkUserdata)
{
	this->addPlugin( ET_NEW OpenSteer::CameraPlugin() );
	this->addPlugin( ET_NEW OpenSteer::GridPlugin() );
	this->addPlugin( ET_NEW PedestrianClientPlugin( false ) );
	BaseClass::initGui( pkUserdata );
}

//-----------------------------------------------------------------------------
// render server plugin
//-----------------------------------------------------------------------------

void PedestrianRenderPeerPlugin::initGui(void* pkUserdata)
{
	this->addPlugin( ET_NEW OpenSteer::CameraPlugin() );
	this->addPlugin( ET_NEW OpenSteer::GridPlugin() );
	this->addPlugin( ET_NEW PedestrianClientPlugin( false ) );
	BaseClass::initGui( pkUserdata );
}

//-----------------------------------------------------------------------------
// client server plugin
//-----------------------------------------------------------------------------


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
	this->addPlugin( ET_NEW OpenSteer::CameraPlugin() );
	this->addPlugin( ET_NEW OpenSteer::GridPlugin() );
	this->addPlugin( ET_NEW PedestrianPeerPlugin( false ) );
	this->addPlugin( ET_NEW PedestrianClientPlugin( false ) );
	BaseClass::initGui( pkUserdata );
	GLUI* glui = ::getRootGLUI();
	GLUI_Panel* pluginPanel = static_cast<GLUI_Panel*>( pkUserdata );
};

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

OfflinePedestrianPlugin* pedestrianPlugin = NULL;
PedestrianClientServerPlugin* clientServerPlugin = NULL;
PedestrianRenderClientPlugin* pedestrianClientPlugin = NULL;
PedestrianRenderPeerPlugin* pedestrianPeerPlugin = NULL;

//-----------------------------------------------------------------------------
namespace EduNet
{
	void initializeDynamicPlugins( )
	{
		pedestrianPlugin = ET_NEW OfflinePedestrianPlugin();
		clientServerPlugin = ET_NEW PedestrianClientServerPlugin();
		pedestrianClientPlugin = ET_NEW PedestrianRenderClientPlugin( true );
		pedestrianPeerPlugin = ET_NEW PedestrianRenderPeerPlugin( true );

	}
	void shutdownDynamicPlugins( )
	{
		ET_SAFE_DELETE( pedestrianPlugin );
		ET_SAFE_DELETE( clientServerPlugin );
		ET_SAFE_DELETE( pedestrianClientPlugin );
		ET_SAFE_DELETE( pedestrianPeerPlugin );

	}
}

