#include "EmptyPlugin.h"

#include "glui/GL/glui.h"


EduNet::InstanceCount EduNetGames::EmptyVehicle::ms_kInstanceCount;


using namespace EduNetGames;
//-----------------------------------------------------------------------------
void EmptyPlugin::initGui( void* pkUserdata )
{
	GLUI* glui = ::getRootGLUI();
	GLUI_Panel* pluginPanel = static_cast<GLUI_Panel*>( pkUserdata );
	glui->add_statictext_to_panel( pluginPanel, "no options" );
}



