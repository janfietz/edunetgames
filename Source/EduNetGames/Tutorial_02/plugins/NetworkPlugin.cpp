#include "NetworkPlugin.h"

#include "glui/GL/glui.h"

//-----------------------------------------------------------------------------
NetworkPlugin::NetworkPlugin(bool bAddToRegistry):
	BaseClass( bAddToRegistry )
{

}

//-----------------------------------------------------------------------------
NetworkPlugin::~NetworkPlugin(void) 
{

}

//-----------------------------------------------------------------------------
void NetworkPlugin::initGui( void* pkUserdata )
{
	GLUI* glui = ::getRootGLUI();
	GLUI_Panel* pluginPanel = static_cast<GLUI_Panel*>( pkUserdata );
	glui->add_button_to_panel( pluginPanel, "Connect" );
}



