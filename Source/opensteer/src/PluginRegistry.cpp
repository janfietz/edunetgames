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

#include "OpenSteer/PluginRegistry.h"
#include "OpenSteer/Camera.h"
#include "OpenSteer/SimpleVehicle.h"
#include "OpenSteer/GlobalData.h"

//-----------------------------------------------------------------------------
namespace OpenSteer {

	PluginRegistry::PluginRegistry( void ):
		m_pkSelectedPlugin(NULL),
		m_itemsInRegistry(0),
		m_on_plugin_selected_func(NULL)
	{

	}

	PluginRegistry::~PluginRegistry( void )
	{
	}

	PluginRegistry* PluginRegistry::accessInstance( void )
	{
		return GlobalData::accessPluginRegistry();
	}

	//-------------------------------------------------------------------------
	OpenSteer::AbstractPlugin*
	PluginRegistry::findNextPlugin( const AbstractPlugin* pkThis ) const
	{
		for (size_t i = 0; i < this->m_itemsInRegistry; i++)
		{
			if (pkThis == this->m_registry[i])
			{
				const bool atEnd = (i == (this->m_itemsInRegistry - 1));
				return this->m_registry [atEnd ? 0 : i + 1];
			}
		}
		return NULL;
	}

	//-------------------------------------------------------------------------
	int PluginRegistry::getPluginIdx( const AbstractPlugin* pkPlugin ) const
	{
		for (size_t i = 0; i < this->m_itemsInRegistry; ++i)
		{
			if (pkPlugin == this->m_registry[i])
			{
				return i;
			}
		}
		return -1;
	}

	//-------------------------------------------------------------------------
	// search the class this->m_registry for a Plugin with the given name
	// returns NULL if none is found
	OpenSteer::AbstractPlugin*
	PluginRegistry::findByName (const char* string) const
	{
		if ( string && string[0] )
		{
			for (size_t i = 0; i < this->m_itemsInRegistry; i++)
			{
				AbstractPlugin* pi = this->m_registry[i];
				AbstractEntity* pe = dynamic_cast<AbstractEntity*>(pi);
				if( pe != NULL )
				{
					const char* s = pe->name();
					if (s && (strcmp (string, s) == 0)) return pi;
				}
			}
		}
		return NULL;
	}

	//-------------------------------------------------------------------------
	// apply a given function to all Plugins in the this->m_registry
	void
	PluginRegistry::applyToAll( plugInCallBackFunction f )
	{
		for (size_t i = 0; i < this->m_itemsInRegistry; i++)
		{
			f(*this->m_registry[i]);
		}
	}

	//-------------------------------------------------------------------------
	// sort Plugin this->m_registry by "selection order"
	//
	// XXX replace with STL utilities
	void
	PluginRegistry::sortBySelectionOrder (void)
	{
		// I know, I know, just what the world needs:
		// another inline shell sort implementation...

		// starting at each of the first n-1 elements of the array
		for (size_t i = 0; i < this->m_itemsInRegistry-1; i++)
		{
			// scan over subsequent pairs, swapping if larger value is first
			for (size_t j = i+1; j < this->m_itemsInRegistry; j++)
			{
				const float iKey = this->m_registry[i]->selectionOrderSortKey ();
				const float jKey = this->m_registry[j]->selectionOrderSortKey ();

				if (iKey > jKey)
				{
					AbstractPlugin* temporary = this->m_registry[i];
					this->m_registry[i] = this->m_registry[j];
					this->m_registry[j] = temporary;
				}
			}
		}
	}

	//-------------------------------------------------------------------------
	// returns pointer to default Plugin (currently, first in this->m_registry)
	OpenSteer::AbstractPlugin*
	PluginRegistry::findDefault (void) const
	{
		// return NULL if no PlugIns exist
		if (this->m_itemsInRegistry == 0) return NULL;

		// otherwise, return the first Plugin that requests initial selection
		for (size_t i = 0; i < this->m_itemsInRegistry; i++)
		{
			if (this->m_registry[i]->requestInitialSelection ()) return this->m_registry[i];
		}

		// otherwise, return the "first" Plugin (in "selection order")
		return this->m_registry[0];
	}

	//-------------------------------------------------------------------------
	// save this instance in the class's this->m_registry of instances
	// (for use by contractors)
	void
	PluginRegistry::addToRegistry (AbstractPlugin* pkPlugin)
	{
		// save this instance in the this->m_registry
		this->m_registry[this->m_itemsInRegistry++] = pkPlugin;
	}

	//-------------------------------------------------------------------------
	// return a group (an STL vector of AbstractVehicle pointers) of all
	// vehicles(/agents/characters) defined by the currently selected Plugin
	const OpenSteer::AVGroup&
	PluginRegistry::allVehiclesOfSelectedPlugin (void) const
	{
		static OpenSteer::AVGroup kTrash;
		if(this->getSelectedPlugin() )
			return this->getSelectedPlugin()->allVehicles ();
		else
			return kTrash;
	}

	//-------------------------------------------------------------------------
	void
	PluginRegistry::selectPlugin( AbstractPlugin* pkPlugin )
	{
		if( pkPlugin == this->getSelectedPlugin() )
		{
			return;
		}
		if( NULL != this->getSelectedPlugin() )
		{
			this->getSelectedPlugin()->close();
		}

		// reset camera and selected vehicle
		OpenSteer::Camera::accessInstance().reset ();
		SimpleVehicle::setSelectedVehicle( NULL );

		this->setSelectedPlugin( pkPlugin );
		if( NULL != this->getSelectedPlugin() )
		{
			this->getSelectedPlugin()->prepareOpen();
			// note: call the application
			//       might initialize the gui
			if( NULL != this->m_on_plugin_selected_func )
			{
				this->m_on_plugin_selected_func( this->getSelectedPlugin() );
			}
			this->getSelectedPlugin()->open();
		}
	}

	//-------------------------------------------------------------------------
	// select the "next" plug-in, cycling through "plug-in selection order"
	void
	PluginRegistry::selectNextPlugin (void)
	{
		if( NULL == this->getSelectedPlugin() )
		{
			return;
		}
		this->selectPlugin( this->getSelectedPlugin()->next () );
	}

	//-------------------------------------------------------------------------
	// select the plug-in by index
	void PluginRegistry::selectPluginByIndex (size_t idx)
	{
		AbstractPlugin* p = this->getPluginAt( idx );
		if( ( NULL != p ) && (p != this->getSelectedPlugin()) )
		{
			this->selectPlugin( p );
		}
	}

	//-------------------------------------------------------------------------
	// handle function keys an a per-plug-in basis
	void
	PluginRegistry::functionKeyForPlugin( int keyNumber ) const
	{
		if( NULL == this->getSelectedPlugin() )
		{
			return;
		}
		this->getSelectedPlugin()->handleFunctionKeys (keyNumber);
	}

	//-------------------------------------------------------------------------
	// return name of currently selected plug-in
	const char*
	PluginRegistry::nameOfSelectedPlugin (void) const
	{
		return (this->getSelectedPlugin() ? this->getSelectedPlugin()->pluginName() : "no Plugin");
	}

	//-------------------------------------------------------------------------
	// reset the currently selected plug-in
	void
	PluginRegistry::resetSelectedPlugin (void)
	{
		if( NULL == this->getSelectedPlugin() )
		{
			return;
		}
		// reset camera and selected vehicle
		OpenSteer::Camera::accessInstance().reset ();
		SimpleVehicle::setSelectedVehicle( NULL );

		this->getSelectedPlugin()->reset ();
	}


} //! namespace OpenSteer    
    

