#ifndef __PLUGINARRAY_H__
#define __PLUGINARRAY_H__

//-----------------------------------------------------------------------------
// Copyright (c) 2009, Jan Fietz, Cyrus Preuss
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without modification, 
// are permitted provided that the following conditions are met:
// 
// * Redistributions of source code must retain the above copyright notice, 
//   this list of conditions and the following disclaimer.
// * Redistributions in binary form must reproduce the above copyright notice, 
//   this list of conditions and the following disclaimer in the documentation 
//   and/or other materials provided with the distribution.
// * Neither the name of EduNetGames nor the names of its contributors
//   may be used to endorse or promote products derived from this software
//   without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
// IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES 
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON 
// ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, 
// EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//-----------------------------------------------------------------------------

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

		OS_IMPLEMENT_CLASSNAME( PluginArray )
		//---------------------------------------------------------------------
		// AbstractPlugin interface
		virtual void open(void);
		virtual void update(const float currentTime, const float elapsedTime); 
		virtual void redraw(const float currentTime, const float elapsedTime); 
		virtual void close(void);
		virtual void reset(void); 
		const char* name (void) const {return this->getClassName();}
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

		// implement to create a vehicle of the specified class
		virtual AbstractVehicle* createVehicle( EntityClassId, ProximityDatabase* ) const { return NULL; };
	private:
		AVGroup m_kVehicles;
		AbstractPlugin* m_pkParentPlugin;

	};

}


#endif // __PLUGINARRAY_H__