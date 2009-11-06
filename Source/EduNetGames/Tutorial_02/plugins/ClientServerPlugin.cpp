#include "ClientServerPlugin.h"


#include "Tutorial_01/plugins/EmptyPlugin.h"

#include "ClientPlugin.h"
#include "PeerPlugin.h"

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

ClientServerPlugin gClientServerPlugin;