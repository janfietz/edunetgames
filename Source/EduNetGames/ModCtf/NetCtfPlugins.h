#ifndef __NETCTFPLUGINS_H__
#define __NETCTFPLUGINS_H__

#include "NetCtfEntityFactory.h"

#include "OpenSteerUT/PluginArray.h"
#include "OpenSteerUT/GridPlugin.h"
#include "OpenSteerUT/CameraPlugin.h"
#include "OpenSteerUT/LocalPlayer.h"

#include "EduNetConnect/ClientPlugin.h"
#include "EduNetConnect/PeerPlugin.h"
#include "EduNetConnect/AbstractEntityReplica.h"
#include "EduNetConnect/AbstractEntityReplicaConnection.h"

#include "NetCtfPlugin.h"

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
	CtfPeerPlugin( bool bAddToRegistry = true );

	virtual ~CtfPeerPlugin();

	OS_IMPLEMENT_CLASSNAME( CtfPeerPlugin )
	virtual const char* name() const;
	//-------------------------------------------------------------------------
	void StartNetworkSession( void );
	//-------------------------------------------------------------------------
	void CreateContent( void );
	//-------------------------------------------------------------------------
	void handleFunctionKeys (int keyNumber);
	//-------------------------------------------------------------------------
	virtual void initGui( void* pkUserdata );
	//-------------------------------------------------------------------------
	void DeleteContent( void );
private:
	AbstractEntityReplicaFactory* m_pkPeerEntityFactory;
	AbstractEntityReplicaManager m_kReplicaManager;
};



//-----------------------------------------------------------------------------
// client plugin
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
class CtfClientPlugin : public TCtfClientPlugin
{
	ET_DECLARE_BASE(TCtfClientPlugin)
public:
	CtfClientPlugin( bool bAddToRegistry = true );

	virtual ~CtfClientPlugin();

	OS_IMPLEMENT_CLASSNAME( CtfClientPlugin )
	virtual const char* name() const;

	//-------------------------------------------------------------------------
	void StartNetworkSession( void );
	//-------------------------------------------------------------------------
	void CreateContent( void );

	//-------------------------------------------------------------------------
	void DeleteContent( void );

	void update (const float currentTime, const float elapsedTime);

private:
	AbstractEntityReplicaFactory* m_pkClientEntityFactory;
	AbstractEntityReplicaManager m_kReplicaManager;
	AbstractEntity* m_pkNetworkPlayer;
};


//-----------------------------------------------------------------------------
// render server plugin
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
class CtfRenderPeerPlugin : public OpenSteer::PluginArray
{
	ET_DECLARE_BASE(OpenSteer::PluginArray);
public:
	CtfRenderPeerPlugin( bool bAddToRegistry = true );
	virtual ~CtfRenderPeerPlugin();

	OS_IMPLEMENT_CLASSNAME( CtfRenderPeerPlugin )

	virtual void initGui(void* pkUserdata);

	virtual void open(void);
	virtual void close(void);
};


//-----------------------------------------------------------------------------
// render client plugin
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
class CtfRenderClientPlugin : public OpenSteer::PluginArray
{
	ET_DECLARE_BASE(OpenSteer::PluginArray);
public:
	CtfRenderClientPlugin( bool bAddToRegistry = true );
	virtual ~CtfRenderClientPlugin();

	OS_IMPLEMENT_CLASSNAME( CtfRenderClientPlugin )

	virtual void initGui(void* pkUserdata);

	virtual void open(void);
	virtual void close(void);
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

	virtual void open(void);
	virtual void close(void);

	//---------------------------------------------------------------------
	// interface AbstractPlugin
	virtual void initGui( void* pkUserdata );
};

#endif //  __NETCTFPLUGINS_H__