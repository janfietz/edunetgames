#include "PluginSelectionPlugin.h"

// TODO: remove cross link
#include "EduNetGames/Tutorial_03/NetBoidPlugins.h"

namespace{

	class NetPluginClientHost : public PluginClientPlugin
	{
	private:
		OpenSteer::AbstractPlugin* CreatePluginByName(
			const char* pszPluginName )
		{
			RakNet::RakString kPluginName(pszPluginName);
			if(kPluginName == "NetBoidRenderPeerPlugin")
			{
				return ET_NEW NetBoidRenderClientPlugin();
			}
			if(kPluginName == "NetBoidClientServerPlugin")
			{
				return ET_NEW NetBoidRenderClientPlugin();
			}
			return NULL;
		}

		virtual float selectionOrderSortKey (void) const { return 1.0f ;}
	};

	PluginServerPlugin<NetBoidRenderPeerPlugin> gNetServerHost;
	PluginServerPlugin<NetBoidClientServerPlugin> gNetClientServerHost;
	NetPluginClientHost gNetClientHost;
}