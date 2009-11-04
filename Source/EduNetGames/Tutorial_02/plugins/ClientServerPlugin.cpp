#include "ClientServerPlugin.h"


#include "Tutorial_01/plugins/EmptyPlugin.h"

#include "ClientPlugin.h"
#include "PeerPlugin.h"

EduNetGames::EmptyPlugIn gEmptyPlugIn;

using namespace OpenSteer;

typedef PeerPlugin<EduNetGames::EmptyPlugIn> EmtyServerPlugin;
typedef ClientPlugin<EduNetGames::EmptyPlugIn> EmtyClientPlugin;

//-----------------------------------------------------------------------------
ClientServerPlugin::ClientServerPlugin()
{
	Plugin::addToRegistry(this);

	this->addPlugIn( new EmtyServerPlugin( false ) );
	this->addPlugIn( new EmtyClientPlugin( false ) );
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