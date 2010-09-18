#ifndef __NETBOIDPLUGINS_H__
#define __NETBOIDPLUGINS_H__
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

#include "NetBoidPeerPlugin.h"
#include "NetBoidClientPlugin.h"
#include "BoidsPlugin.h"
#include "OpenSteerUT/PluginArray.h"

//-----------------------------------------------------------------------------
class NetBoidRenderOfflinePlugin :
	public OpenSteer::PluginArrayPluginMixin<OpenSteer::NetBoidsPlugin>
{
	ET_DECLARE_BASE( OpenSteer::PluginArrayPluginMixin<OpenSteer::NetBoidsPlugin> )
public:
	NetBoidRenderOfflinePlugin( bool bAddToRegistry = true ):
	BaseClass( bAddToRegistry ) {}

	virtual ~NetBoidRenderOfflinePlugin() {};

	OS_IMPLEMENT_CLASSNAME( NetBoidRenderOfflinePlugin )

	virtual float selectionOrderSortKey (void) const { return 1.0f ;}

	virtual void initGui( void* pkUserdata )
	{
		this->prepare();
		BaseClass::initGui( pkUserdata );
	}

	void prepare(void);

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
	public OpenSteer::PluginArrayPluginMixin<NetBoidPeerPlugin>
{
	ET_DECLARE_BASE( OpenSteer::PluginArrayPluginMixin<NetBoidPeerPlugin> )
public:
	NetBoidRenderPeerPlugin( bool bAddToRegistry = true ):
	BaseClass( bAddToRegistry ) {}

	virtual ~NetBoidRenderPeerPlugin() {};

	OS_IMPLEMENT_CLASSNAME( NetBoidRenderPeerPlugin )

	virtual float selectionOrderSortKey (void) const { return 3.0f ;}

	virtual void initGui( void* pkUserdata )
	{
		this->prepare();
		BaseClass::initGui( pkUserdata );
	}

	void prepare(void);

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
class NetBoidMultiplePeerPlugin : public OpenSteer::PluginArray
{
	ET_DECLARE_BASE(OpenSteer::PluginArray);
public:

	NetBoidMultiplePeerPlugin(bool bAddToRegistry = false):
	  BaseClass( bAddToRegistry ) {}

	  virtual ~NetBoidMultiplePeerPlugin(){}

	  OS_IMPLEMENT_CLASSNAME( NetBoidMultiplePeerPlugin );

	  virtual float selectionOrderSortKey (void) const { return 4.0f ;}

	  //---------------------------------------------------------------------
	  // interface AbstractPlugin
	  virtual void initGui( void* pkUserdata )
	  {
		  this->prepare();
		  BaseClass::initGui( pkUserdata );
	  }

	  void prepare(void);

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

	void prepare(void);

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
class NetBoidRenderServerPlugin : public OpenSteer::PluginArray
{
	ET_DECLARE_BASE(OpenSteer::PluginArray);
public:

	NetBoidRenderServerPlugin(bool bAddToRegistry = true):
	  BaseClass( bAddToRegistry ) {}

	  virtual ~NetBoidRenderServerPlugin(){}

	  OS_IMPLEMENT_CLASSNAME( NetBoidRenderServerPlugin );

	  virtual float selectionOrderSortKey (void) const { return 4.0f ;}

	  //---------------------------------------------------------------------
	  // interface AbstractPlugin
	  virtual void initGui( void* pkUserdata )
	  {
		  this->prepare();
		  BaseClass::initGui( pkUserdata );
	  }

	  void prepare(void);

	  virtual void close(void)
	  {		
		  BaseClass::close();
		  this->removeAllPlugins();
	  }
};


#endif //__NETBOIDPLUGINS_H__