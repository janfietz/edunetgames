#ifndef OPENSTEER_PLUGINREGISTRY_H
#define OPENSTEER_PLUGINREGISTRY_H

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

#include "OpenSteer/AbstractPlugin.h"
#include "OpenSteer/AbstractVehicle.h"

//-----------------------------------------------------------------------------
namespace OpenSteer {


	typedef void (* plugInCallBackFunction) ( AbstractPlugin& clientObject );
	typedef void (* voidCallBackFunction) (void);
	typedef void (* timestepCallBackFunction) (const float currentTime,
		const float elapsedTime);

	class PluginRegistry
	{
	public:
		PluginRegistry( void );
		virtual ~PluginRegistry( void );

		static PluginRegistry* accessInstance( void );

		AbstractPlugin* getSelectedPlugin( void ) const;


		//! search the class registry for a Plugin with the given name
		AbstractPlugin* findByName (const char* string) const;

		//! apply a given function to all Plugins in the class registry
		void applyToAll (plugInCallBackFunction f);

		//! sort Plugin registry by "selection order"
		void sortBySelectionOrder (void);

		//! returns pointer to default Plugin (currently, first in registry)
		AbstractPlugin* findDefault (void) const;

		//! save this instance in the class's registry of instances
		void addToRegistry (AbstractPlugin*);
		AbstractPlugin* findNextPlugin( const AbstractPlugin* pkThis ) const;

		//! 
		int getNumPlugins( void ) const { return m_itemsInRegistry; };
		AbstractPlugin* getPluginAt( size_t idx ) const { return m_registry[idx]; };
		int getPluginIdx( const AbstractPlugin* pkPlugin ) const;

		//! utility function
		const AVGroup& allVehiclesOfSelectedPlugin( void ) const;

		// not public right now ... needs to forward gui update
		// select the "next" plug-in, cycling through "plug-in selection order"
		void selectNextPlugin (void);

		void selectPlugin( AbstractPlugin* pkPlugin );

		// select the plug-in by index
		void selectPluginByIndex( size_t idx );

		// handle function keys an a per-plug-in basis
		void functionKeyForPlugin( int keyNumber ) const;

		// return name of currently selected plug-in
		const char* nameOfSelectedPlugin( void ) const;

		// reset the currently selected plug-in
		void resetSelectedPlugin( void );

		void setOnPluginSelectedFunc( on_plugin_selected_func func )
		{
			this->m_on_plugin_selected_func = func;
		}

	private:

		void setSelectedPlugin( AbstractPlugin* pkSelectedPlugin );

		on_plugin_selected_func m_on_plugin_selected_func;
		AbstractPlugin* m_pkSelectedPlugin;

		enum	{
			EPluginRegistry_Capacity = 1000
		};
		size_t m_itemsInRegistry;
		AbstractPlugin* m_registry[EPluginRegistry_Capacity];

	};

	inline
	void PluginRegistry::setSelectedPlugin( AbstractPlugin* pkPlugin )
	{
		this->m_pkSelectedPlugin = pkPlugin;
	}
	inline
	AbstractPlugin* PluginRegistry::getSelectedPlugin( void ) const
	{
		return this->m_pkSelectedPlugin;
	}

} //! namespace OpenSteer    
    

//-----------------------------------------------------------------------------
#endif //! OPENSTEER_PLUGINREGISTRY_H
