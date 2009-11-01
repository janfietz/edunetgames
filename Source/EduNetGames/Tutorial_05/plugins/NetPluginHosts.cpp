#include "EmptyRpcPlugin.h"
#include "Tutorial_03/plugins/NetBoidPlugin.h"

namespace{

	class NetPluginServerHost : public EmptyServerRpcPlugin
	{
	private:
		void InitializeGamePlugin( void )
		{
			this->m_kGamePlugIn.addPlugIn(new NetPeerBoidPlugin() );
		}
	};

	class NetPluginClientHost : public EmptyClientRpcPlugin
	{
	private:
		OpenSteer::AbstractPlugIn* CreatePluginByName(
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

	NetPluginServerHost gNetServerHost;
	NetPluginClientHost gNetClientHost;
}