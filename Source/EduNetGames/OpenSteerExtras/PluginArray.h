#ifndef __PLUGINARRAY_H__
#define __PLUGINARRAY_H__

#include "EduNet/common/EduNetCommon.h"

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
		AbstractPlugin* getPlugin( size_t uiIdx ) const;		


		static void TestPluginArray( void );

		//---------------------------------------------------------------------
		// interface AbstractPlugin
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
		virtual const AVGroup& allVehicles(void) const;

		virtual AbstractPlugin* next(void) const;

	private:
		AVGroup m_kVehicles;

	};

}


#endif // __PLUGINARRAY_H__