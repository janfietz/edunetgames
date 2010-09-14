#include "EduNetConnect/ClientServerPlugin.h"


//-----------------------------------------------------------------------------
void EduNetConnect::queryConnectionsSettings( ConnectSettings& kSettings )
{
	kSettings.uiClientStartPort = CLIENT_PORT;
	kSettings.uiServerStartPort = SERVER_PORT;
	kSettings.sessionPassword = "Tutorial2";
	kSettings.uiPortPongCount = 10;
}

ClientServerPlugin* clientServerPlugin = NULL;
EmptyServerPlugin* peerPlugin = NULL;
EmptyClientPlugin* clientPlugin = NULL;

//-----------------------------------------------------------------------------
namespace EduNet
{
	void initializeDynamicPlugins( )
	{
		clientServerPlugin = ET_NEW ClientServerPlugin();
		peerPlugin = ET_NEW EmptyServerPlugin();
		clientPlugin = ET_NEW EmptyClientPlugin();
	}
	void shutdownDynamicPlugins( )
	{
		ET_SAFE_DELETE( clientServerPlugin );
		ET_SAFE_DELETE( peerPlugin );
		ET_SAFE_DELETE( clientPlugin );
	}
}

