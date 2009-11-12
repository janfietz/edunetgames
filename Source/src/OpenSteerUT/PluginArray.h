#ifndef __PLUGINARRAY_H__
#define __PLUGINARRAY_H__

#include "EduNetCommon/EduNetCommon.h"

#include "boost/smart_ptr.hpp" 


//-----------------------------------------------------------------------------
namespace OpenSteer
{
	typedef boost::shared_ptr<AbstractPlugin> AbstractPluginPtr;
	typedef std::vector<AbstractPluginPtr> TPluginArray;


	class PluginArray : public AbstractPlugin, protected TPluginArray
	{
	public:
		PluginArray(bool bAddToRegistry = true);
		virtual ~PluginArray();

		//---------------------------------------------------------------------
		// functionality PluginArray
		void addPlugin( AbstractPlugin* pkPlugin );
		void removePlugin( AbstractPlugin* pkPlugin );
		void removeAllPlugins( void );
		AbstractPlugin* findPlugin( AbstractPlugin* pkPlugin ) const;		
		AbstractPlugin* getPlugin( size_t uiIdx ) const;		

		// unit test function
		static void TestPluginArray( void );

		//---------------------------------------------------------------------
		// AbstractPlugin interface
		virtual void open(void);
		virtual void update(const float currentTime, const float elapsedTime); 
		virtual void redraw(const float currentTime, const float elapsedTime); 
		virtual void close(void);
		virtual void reset(void); 
		virtual const char *name(void) const; 
		virtual float selectionOrderSortKey(void) const; 
		virtual bool requestInitialSelection(void) const; 
		virtual void handleFunctionKeys(int keyNumber); 
		virtual void printMiniHelpForFunctionKeys(void) const; 
		virtual ProximityDatabase* accessProximityDataBase( void ) const {return NULL;};
		virtual const AVGroup& allVehicles(void) const;

		virtual AbstractPlugin* next(void) const;

		// returns pointer to the parent Plugin
		virtual AbstractPlugin* getParentPlugin(void) const { return m_pkParentPlugin; };

		// set a parent Plugin
		virtual void setParentPlugin( AbstractPlugin* pkPlugin ) { m_pkParentPlugin = pkPlugin; };

		// implement to initialize additional gui functionality
		virtual void initGui( void* pkUserdata );
	private:
		AVGroup m_kVehicles;
		AbstractPlugin* m_pkParentPlugin;

	};

}


#endif // __PLUGINARRAY_H__