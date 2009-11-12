#include "ClientServerPlugin.h"


#include "OpenSteerUT/EmptyPlugin.h"

#include "ClientPlugin.h"
#include "PeerPlugin.h"

EduNet::EmptyPlugin gEmptyPlugin;

using namespace OpenSteer;

typedef PeerPlugin<EduNet::EmptyPlugin> EmtyServerPlugin;
typedef ClientPlugin<EduNet::EmptyPlugin> EmtyClientPlugin;

//-----------------------------------------------------------------------------
ClientServerPlugin::ClientServerPlugin()
{
	Plugin::addToRegistry(this);

	this->addPlugin( new EmtyServerPlugin( false ) );
	this->addPlugin( new EmtyClientPlugin( false ) );
}

//-----------------------------------------------------------------------------
ClientServerPlugin::~ClientServerPlugin()
{

}

//-----------------------------------------------------------------------------
const char* ClientServerPlugin::name(void) const
{
	return "ClientServerPlugin";
}

//-----------------------------------------------------------------------------
void ClientServerPlugin::initGui( void* pkUserdata ) 
{
	BaseClass::initGui( pkUserdata );
	GLUI* glui = ::getRootGLUI();
	GLUI_Panel* pluginPanel = static_cast<GLUI_Panel*>( pkUserdata );

// 	glui->add_button_to_panel( pluginPanel, "Connect Client" );
// 	glui->add_button_to_panel( pluginPanel, "Connect Server" );
};


ClientServerPlugin gClientServerPlugin;