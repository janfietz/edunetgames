#include "EduNetConnect/ClientServerPlugin.h"


//-----------------------------------------------------------------------------
void EduNetConnect::queryConnectionsSettings( ConnectSettings& kSettings )
{
	kSettings.uiClientStartPort = CLIENT_PORT;
	kSettings.uiServerStartPort = SERVER_PORT;
	kSettings.sessionPassword = "Tutorial2";
	kSettings.uiPortPongCount = 10;
}

namespace EduNet
{
	void initializeStaticPlugins( )
	{

	}
	void shutdownStaticPlugins( )
	{

	}
}

ClientServerPlugin gClientServerPlugin;
EmptyServerPlugin gPeerPlugin;
EmptyClientPlugin gClientPlugin;