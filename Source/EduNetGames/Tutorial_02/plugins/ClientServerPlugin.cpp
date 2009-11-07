#include "ClientServerPlugin.h"


#include "Tutorial_01/plugins/EmptyPlugin.h"

#include "ClientPlugin.h"
#include "PeerPlugin.h"
#include "glui/GL/glui.h"

EduNetGames::EmptyPlugin gEmptyPlugin;

using namespace OpenSteer;

typedef PeerPlugin<EduNetGames::EmptyPlugin> EmtyServerPlugin;
typedef ClientPlugin<EduNetGames::EmptyPlugin> EmtyClientPlugin;

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
void ClientServerPlugin::initGui(void) 
{
	GLUI* glui = OpenSteerDemo::ms_kApplication.getAppGui();
	GLUI_Panel* pluginPanel = OpenSteerDemo::ms_kApplication.getPluginPanel();

	glui->add_button_to_panel( pluginPanel, "Connect Client" );
	glui->add_button_to_panel( pluginPanel, "Connect Server" );
};


ClientServerPlugin gClientServerPlugin;