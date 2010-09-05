#include "EduNetConnect/ClientServerPlugin.h"


//-----------------------------------------------------------------------------
void EduNetConnect::queryConnectionsSettings( ConnectSettings& kSettings )
{
	kSettings.uiClientStartPort = CLIENT_PORT;
	kSettings.uiServerStartPort = SERVER_PORT;
	kSettings.sessionPassword = "Tutorial5";
	kSettings.uiPortPongCount = 10;
}


ClientServerPlugin gClientServerPlugin;
EmptyServerPlugin gPeerPlugin;
EmptyClientPlugin gClientPlugin;