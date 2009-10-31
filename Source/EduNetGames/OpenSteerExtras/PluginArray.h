#ifndef __PLUGINARRAY_H__
#define __PLUGINARRAY_H__

#include "EduNet/common/EduNetCommon.h"

#include "boost/smart_ptr.hpp" 


//-----------------------------------------------------------------------------
namespace OpenSteer
{
	typedef boost::shared_ptr<AbstractPlugIn> AbstractPlugInPtr;
	typedef std::vector<AbstractPlugInPtr> TPlugInArray;


	class PlugInArray : public AbstractPlugIn, protected TPlugInArray
	{
	public:
		PlugInArray();
		virtual ~PlugInArray();

		//---------------------------------------------------------------------
		// functionality PlugInArray
		void addPlugIn( AbstractPlugIn* pkPlugin );
		void removePlugIn( AbstractPlugIn* pkPlugin );
		AbstractPlugIn* getPlugIn( size_t uiIdx );


		static void TestPluginArray( void );

		//---------------------------------------------------------------------
		// interface AbstractPlugIn
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

		virtual AbstractPlugIn* next(void) const;

	private:
		AVGroup m_kVehicles;

	};

}


#endif // __PLUGINARRAY_H__