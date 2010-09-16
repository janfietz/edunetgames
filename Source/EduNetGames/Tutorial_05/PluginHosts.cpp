#include "PluginSelectionPlugin.h"
#include "EduNetApplication/EduNetModulePluginLoader.h"

//-----------------------------------------------------------------------------
namespace EduNet
{

	//-----------------------------------------------------------------------------
	class ServerModulePluginLoader : public ModulePluginLoader
	{
		ET_DECLARE_BASE(EduNet::ModulePluginLoader)
	protected:
		virtual OpenSteer::AbstractPlugin* createPluginFromFactoryByName(
			PluginFactory* pkFactory,
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

	ServerModulePluginLoader* gLoadPlugin = NULL;

	//-----------------------------------------------------------------------------
	class NetPluginClientHost : public ClientPluginSelectionPlugin
	{
		ET_DECLARE_BASE(ClientPluginSelectionPlugin)
	public:
		virtual float selectionOrderSortKey (void) const { 
			// want to be selected
			return 0.0001f ;
		}

		//-----------------------------------------------------------------------------
		virtual void update(const float currentTime, const float elapsedTime)
		{
			BaseClass::update( currentTime, elapsedTime );
		}
	protected:
		OpenSteer::AbstractPlugin* createPluginByName(
			const char* pszPluginName )
		{
			printf("try to create plugin: %s ...\n", pszPluginName);
			OpenSteer::AbstractPlugin* plugin = NULL;
			enString_t kPluginName(pszPluginName);
			if( kPluginName == "NetBoidRenderPeerPlugin" )
			{
				plugin = gLoadPlugin->createPluginByName( kPluginName.c_str() );
			}
			else if( kPluginName == "NetBoidMultiplePeerPlugin" )
			{
				plugin = gLoadPlugin->createPluginByName("NetBoidRenderPeerPlugin");
			}
			else if( kPluginName == "NetBoidRenderServerPlugin" )
			{
				plugin = gLoadPlugin->createPluginByName("NetBoidClientPlugin");
			}
			else if( kPluginName == "RenderSoccerPeerPlugin" )
			{
				plugin = gLoadPlugin->createPluginByName("RenderSoccerClientPlugin");
			}
			else
			{
				printf("plugin not supported\n");
			}

			printf("try to create plugin: %s -> result plugin: %s\n", 
				pszPluginName, 
				NULL != plugin ? plugin->pluginName() : "FAILED" );

			return plugin;
		}
	private:

	};

	NetPluginClientHost* gNetClientHost = NULL;

	//-----------------------------------------------------------------------------
	void initializeDynamicPlugins( )
	{
		gLoadPlugin = ET_NEW ServerModulePluginLoader();
		ModuleManager kModuleManager;
		gLoadPlugin->loadModules( kModuleManager.getCurrentModulePath() );
		gLoadPlugin->createPluginsFromModules();

		gNetClientHost = ET_NEW NetPluginClientHost();
	}

	//-----------------------------------------------------------------------------
	void shutdownDynamicPlugins( )
	{
		ET_SAFE_DELETE( gNetClientHost );

		gLoadPlugin->unloadModules();
		ET_SAFE_DELETE(gLoadPlugin);
	}



}