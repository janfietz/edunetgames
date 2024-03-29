#ifndef __NETSOCCERPLUGINS_H__
#define __NETSOCCERPLUGINS_H__
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

//-----------------------------------------------------------------------------
#include "OpenSteerUT/PluginArray.h"

#include "NetSoccerClientPlugin.h"
#include "NetSoccerPeerPlugin.h"
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// render server plugin
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

#endif // __NETSOCCERPLUGINS_H__
