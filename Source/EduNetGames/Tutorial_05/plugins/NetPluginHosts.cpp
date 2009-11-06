#include "EmptyRpcPlugin.h"
#include "Tutorial_03/plugins/NetBoidPlugin.h"

namespace{

	class NetPluginClientHost : public EmptyClientRpcPlugin
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

	EmptyServerRpcPlugin<NetPeerBoidPlugin> gNetServerHost;
	NetPluginClientHost gNetClientHost;
}