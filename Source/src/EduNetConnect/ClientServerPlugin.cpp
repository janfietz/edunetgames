#include "ClientServerPlugin.h"






EduNet::EmptyPlugin gEmptyPlugin(false);

using namespace OpenSteer;

//-----------------------------------------------------------------------------
ClientServerPlugin::ClientServerPlugin()
{
	Plugin::addToRegistry(this);

	this->addPlugin( new EmptyServerPlugin( false ) );
	this->addPlugin( new EmptyClientPlugin( false ) );
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
