#include "PluginSelectionPlugin.h"
#include "EduNetApplication/EduNetPluginLoadPlugin.h"

//-----------------------------------------------------------------------------
class EduNetServerPluginLoadPlugin : public EduNet::PluginLoadPlugin
{
	ET_DECLARE_BASE(EduNet::PluginLoadPlugin)
protected:
	virtual OpenSteer::AbstractPlugin* createPluginFromFactoryByName(
		EduNetPluginFactory* pkFactory,
		const char* pszPluginName )
	{
		OpenSteer::AbstractPlugin* pkPlugin = 
			BaseClass::createPluginFromFactoryByName(pkFactory, pszPluginName);
		if (NULL != pkPlugin)
		{
			// create wrapper plugin to serve it to the client
			PluginServerPlugin* pkSrvPlugin = ET_NEW PluginServerPlugin();
			pkSrvPlugin->addPlugin( pkPlugin );
			return pkSrvPlugin;
		}
		return NULL;
	}
};

//-----------------------------------------------------------------------------
namespace EduNet
{
	EduNetServerPluginLoadPlugin gLoadPlugin;
	void initializeStaticPlugins( )
	{
		ModuleManager kModuleManager;
		gLoadPlugin.loadModules( kModuleManager.getCurrentModulePath() );
		gLoadPlugin.createPluginsFromModules();

	}
	void shutdownStaticPlugins( )
	{
		gLoadPlugin.unloadModules();
	}

	class NetPluginClientHost : public PluginClientPlugin
	{
		ET_DECLARE_BASE(PluginClientPlugin)
	private:
		OpenSteer::AbstractPlugin* CreatePluginByName(
			const char* pszPluginName )
		{
			RakNet::RakString kPluginName(pszPluginName);
			if(kPluginName == "NetBoidRenderPeerPlugin")
			{
				return gLoadPlugin.createPluginByName(kPluginName.C_String());
			}
			if(kPluginName == "NetBoidMultiplePeerPlugin")
			{
				return gLoadPlugin.createPluginByName("NetBoidRenderPeerPlugin");
			}
			if(kPluginName == "NetBoidRenderServerPlugin")
			{
				return gLoadPlugin.createPluginByName("NetBoidClientPlugin");
			}
			return NULL;
		}
	public:
		virtual float selectionOrderSortKey (void) const { return 1.0f ;}

		//-----------------------------------------------------------------------------
		virtual void update(const float currentTime, const float elapsedTime)
		{
			// temporary fix
			OpenSteer::SimpleVehicle::selectedVehicle = NULL;
			OpenSteer::SimpleVehicle::nearestMouseVehicle = NULL;
			BaseClass::update( currentTime, elapsedTime );
		}

	};

	NetPluginClientHost gNetClientHost;
}