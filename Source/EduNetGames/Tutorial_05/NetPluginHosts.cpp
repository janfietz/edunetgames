#include "PluginSelectionPlugin.h"

// TODO: remove cross link
#include "../../EduNetGames/Tutorial_03/NetBoidPlugin.h"

namespace{

	class NetPluginClientHost : public PluginClientPlugin
	{
	private:
		OpenSteer::AbstractPlugin* CreatePluginByName(
			const char* pszPluginName )
		{
			RakNet::RakString kPluginName(pszPluginName);
			if(kPluginName == "NetPeerBoidPlugin")
			{
				return new NetClientBoidPlugin();
			}
			return NULL;
		}
	};

	PluginServerPlugin<NetPeerBoidPlugin> gNetServerHost;
	NetPluginClientHost gNetClientHost;
}