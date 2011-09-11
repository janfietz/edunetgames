#ifndef __NETBOIDPLUGINS_H__
#define __NETBOIDPLUGINS_H__
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

#include "NetBoidPlugin.h"
#include "OpenSteerUT/PluginArray.h"
#include "OpenSteerUT/CameraPlugin.h"
#include "OpenSteerUT/GridPlugin.h"

//-----------------------------------------------------------------------------
class NetBoidRenderOfflinePlugin : 
	public OpenSteer::PluginArrayPluginMixin<OpenSteer::BoidsPlugin>
{
	ET_DECLARE_BASE( OpenSteer::PluginArrayPluginMixin<OpenSteer::BoidsPlugin> )
public:
	NetBoidRenderOfflinePlugin( bool bAddToRegistry = true ):
	BaseClass( bAddToRegistry ) {}

	virtual ~NetBoidRenderOfflinePlugin() {};

	OS_IMPLEMENT_CLASSNAME( NetBoidRenderOfflinePlugin )

	virtual float selectionOrderSortKey (void) const { return 5.0f ;}

	virtual void initGui( void* pkUserdata )
	{
		this->prepare();
		BaseClass::initGui( pkUserdata );
	}

	void prepare(void)
	{
		this->addPlugin( ET_NEW OpenSteer::GridPlugin() );		
		this->addPlugin( ET_NEW OpenSteer::CameraPlugin() );
	}
	
	virtual void close(void)
	{		
		BaseClass::close();
		this->removeAllPlugins();
	}

};


//-----------------------------------------------------------------------------
// render client plugin
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
class NetBoidRenderClientPlugin : 
	public OpenSteer::PluginArrayPluginMixin<NetClientBoidPlugin>
{
	ET_DECLARE_BASE( OpenSteer::PluginArrayPluginMixin<NetClientBoidPlugin> )
public:
	NetBoidRenderClientPlugin( bool bAddToRegistry = true ):
	BaseClass( bAddToRegistry ) {}

	virtual ~NetBoidRenderClientPlugin() {};

	OS_IMPLEMENT_CLASSNAME( NetBoidRenderClientPlugin )

	virtual float selectionOrderSortKey (void) const { return 2.0f ;}
	
	virtual void initGui( void* pkUserdata )
	{
		this->prepare();
		BaseClass::initGui( pkUserdata );
	}

	void prepare(void)
	{
		this->addPlugin( ET_NEW OpenSteer::CameraPlugin() );		
	}

	virtual void close(void)
	{		
		BaseClass::close();
		this->removeAllPlugins();
	}
};


//-----------------------------------------------------------------------------
// render server plugin
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
class NetBoidRenderPeerPlugin : 
	public OpenSteer::PluginArrayPluginMixin<NetPeerBoidPlugin>
{
	ET_DECLARE_BASE( OpenSteer::PluginArrayPluginMixin<NetPeerBoidPlugin> )
public:
	NetBoidRenderPeerPlugin( bool bAddToRegistry = true ):
	BaseClass( bAddToRegistry ) {}

	virtual ~NetBoidRenderPeerPlugin() {};

	OS_IMPLEMENT_CLASSNAME( NetBoidRenderPeerPlugin )
	
	virtual float selectionOrderSortKey (void) const { return 1.0f ;}

	virtual void initGui( void* pkUserdata )
	{
		this->prepare();
		BaseClass::initGui( pkUserdata );
	}

	void prepare(void)
	{
		this->addPlugin( ET_NEW OpenSteer::CameraPlugin() );		
	}

	virtual void close(void)
	{		
		BaseClass::close();
		this->removeAllPlugins();
	}
};


//-----------------------------------------------------------------------------
// client server plugin
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
class NetBoidClientServerPlugin : public OpenSteer::PluginArray
{
	ET_DECLARE_BASE(OpenSteer::PluginArray);
public:

	NetBoidClientServerPlugin(bool bAddToRegistry = true):
	  BaseClass( bAddToRegistry ) {}

	  virtual ~NetBoidClientServerPlugin(){}

	  OS_IMPLEMENT_CLASSNAME( NetBoidClientServerPlugin );

	  virtual float selectionOrderSortKey (void) const { return 4.0f ;}

	  //---------------------------------------------------------------------
	  // interface AbstractPlugin
	  virtual void initGui( void* pkUserdata )
	  {
		  this->prepare();
		  BaseClass::initGui( pkUserdata );
	  }

	  void prepare(void)
	  {
		  this->addPlugin( ET_NEW OpenSteer::CameraPlugin() );
		  this->addPlugin( ET_NEW NetPeerBoidPlugin( false ) );
		  this->addPlugin( ET_NEW NetClientBoidPlugin( false ) );		  
	  }

	  virtual void close(void)
	  {		
		  BaseClass::close();
		  this->removeAllPlugins();
	  }
};




#endif //__NETBOIDPLUGINS_H__