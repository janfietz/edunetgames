//-----------------------------------------------------------------------------
//
//
// OpenSteer -- Steering Behaviors for Autonomous Characters
//
// Copyright (c) 2002-2005, Sony Computer Entertainment America
// Original author: Craig Reynolds <craig_reynolds@playstation.sony.com>
//
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//
//
//-----------------------------------------------------------------------------
//
//
// OpenSteerDemo Plugin class
// 
// 10-04-04 bk:  put everything into the OpenSteer namespace
// 11-13-02 cwr: created 
//
//
//-----------------------------------------------------------------------------


#include "OpenSteer/PlugIn.h"
#include "OpenSteer/SimplePlayer.h"
#include "OpenSteer/SimpleVehicle.h"
#include "OpenSteer/Camera.h"


//-----------------------------------------------------------------------------
// Plugin registry
//
// XXX replace with STL utilities
int OpenSteer::Plugin::itemsInRegistry = 0;
const int OpenSteer::Plugin::totalSizeOfRegistry = 1000;
OpenSteer::AbstractPlugin* OpenSteer::Plugin::registry [totalSizeOfRegistry];

//-----------------------------------------------------------------------------
// currently selected plug-in (user can choose or cycle through them)
OpenSteer::AbstractPlugin* OpenSteer::Plugin::selectedPlugin = NULL;
on_plugin_selected_func OpenSteer::Plugin::ms_on_plugin_selected_func = NULL;
//-----------------------------------------------------------------------------
// constructor
OpenSteer::Plugin::Plugin( bool bAddToRegistry ):
	m_pkParentPlugin( NULL ),
	m_pkVehicleFactory( NULL )
{
    // save this new instance in the registry
	if( true == bAddToRegistry )
	{
		addToRegistry ( this );
	}
}

//-----------------------------------------------------------------------------
// destructor
OpenSteer::Plugin::~Plugin() 
{
	if( this == OpenSteer::Plugin::selectedPlugin )
	{
		OpenSteer::Plugin::selectPlugin( NULL );
	}
}

//-----------------------------------------------------------------------------
OpenSteer::AbstractEntity* 
OpenSteer::Plugin::createSystemEntity( EntityClassId classId )
{
	OpenSteer::AbstractEntity* pkEntity = NULL;
	OpenSteer::AbstractObstacle* pkObstacle = NULL;
	switch( classId )
	{
	case( OS_CID_PLAYER ):
		{
			OpenSteer::SimplePlayer* pkPlayer = OS_NEW OpenSteer::SimplePlayer();
			pkEntity = pkPlayer;
		}
		break;
	case( OS_CID_PLAYERCONTROLLER ):
		{
			OpenSteer::SimpleController* pkController = OS_NEW OpenSteer::SimpleController();
			pkEntity = pkController;
		}
		break;
	case( OS_CID_OBSTACLE ):
		{
// this is an abstract class no way to create it
//			pkObstacle = OS_NEW OpenSteer::Obstacle();
		}
		break;
	case( OS_CID_SPHEREOBSTACLE ):
		{
			pkObstacle = OS_NEW OpenSteer::SphereObstacle();
			pkEntity = pkObstacle;
		}
		break;
	case( OS_CID_BOXOBSTACLE ):
		{
			pkObstacle = OS_NEW OpenSteer::BoxObstacle();
			pkEntity = pkObstacle;
		}
		break;
	case( OS_CID_PLANEOBSTACLE ):
		{
			pkObstacle = OS_NEW OpenSteer::PlaneObstacle();
			pkEntity = pkObstacle;
		}
		break;
	case( OS_CID_RECTANGLEOBSTACLE ):
		{
			pkObstacle = OS_NEW OpenSteer::RectangleObstacle();
			pkEntity = pkObstacle;
		}
		break;
	}
	return pkEntity;
}

//-----------------------------------------------------------------------------
// returns pointer to the next Plugin in "selection order"
//-----------------------------------------------------------------------------
OpenSteer::AbstractPlugin* 
OpenSteer::Plugin::next (void) const
{
	return OpenSteer::Plugin::findNextPlugin( this );
}

//-----------------------------------------------------------------------------
OpenSteer::AbstractPlugin* 
OpenSteer::Plugin::findNextPlugin( const AbstractPlugin* pkThis )
{
	for (int i = 0; i < itemsInRegistry; i++)
	{
		if (pkThis == registry[i])
		{
			const bool atEnd = (i == (itemsInRegistry - 1));
			return registry [atEnd ? 0 : i + 1];
		}
	}
	return NULL;
}

//-----------------------------------------------------------------------------
int OpenSteer::Plugin::getPluginIdx( const AbstractPlugin* pkPlugin )
{
	for (int i = 0; i < itemsInRegistry; ++i)
	{
		if (pkPlugin == registry[i])
		{
			return i;
		}
	}
	return -1;
}

//-----------------------------------------------------------------------------
// search the class registry for a Plugin with the given name
// returns NULL if none is found
OpenSteer::AbstractPlugin* 
OpenSteer::Plugin::findByName (const char* string)
{
    if (string)
    {
        for (int i = 0; i < itemsInRegistry; i++)
        {
            AbstractPlugin* pi = registry[i];
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

//-----------------------------------------------------------------------------
// apply a given function to all Plugins in the registry
void 
OpenSteer::Plugin::applyToAll( plugInCallBackFunction f )
{
    for (int i = 0; i < itemsInRegistry; i++)
    {
        f(*registry[i]);
    }
}

//-----------------------------------------------------------------------------
// sort Plugin registry by "selection order"
//
// XXX replace with STL utilities
void 
OpenSteer::Plugin::sortBySelectionOrder (void)
{
    // I know, I know, just what the world needs:
    // another inline shell sort implementation...

    // starting at each of the first n-1 elements of the array
    for (int i = 0; i < itemsInRegistry-1; i++)
    {
        // scan over subsequent pairs, swapping if larger value is first
        for (int j = i+1; j < itemsInRegistry; j++)
        {
            const float iKey = registry[i]->selectionOrderSortKey ();
            const float jKey = registry[j]->selectionOrderSortKey ();

            if (iKey > jKey)
            {
                AbstractPlugin* temporary = registry[i];
                registry[i] = registry[j];
                registry[j] = temporary;
            }
        }
    }
}

//-----------------------------------------------------------------------------
// returns pointer to default Plugin (currently, first in registry)
OpenSteer::AbstractPlugin* 
OpenSteer::Plugin::findDefault (void)
{
    // return NULL if no PlugIns exist
    if (itemsInRegistry == 0) return NULL;

    // otherwise, return the first Plugin that requests initial selection
    for (int i = 0; i < itemsInRegistry; i++)
    {
        if (registry[i]->requestInitialSelection ()) return registry[i];
    }

    // otherwise, return the "first" Plugin (in "selection order")
    return registry[0];
}

//-----------------------------------------------------------------------------
// save this instance in the class's registry of instances
// (for use by contractors)
void 
OpenSteer::Plugin::addToRegistry (AbstractPlugin* pkPlugin)
{
    // save this instance in the registry
    registry[itemsInRegistry++] = pkPlugin;
}

//-----------------------------------------------------------------------------
// return a group (an STL vector of AbstractVehicle pointers) of all
// vehicles(/agents/characters) defined by the currently selected Plugin
const OpenSteer::AVGroup& 
OpenSteer::Plugin::allVehiclesOfSelectedPlugin (void)
{
	static OpenSteer::AVGroup kTrash;
	if(OpenSteer::Plugin::selectedPlugin )
		return OpenSteer::Plugin::selectedPlugin->allVehicles ();
	else
		return kTrash;
}

//-----------------------------------------------------------------------------
void 
OpenSteer::Plugin::selectPlugin( AbstractPlugin* pkPlugin )
{
	if( pkPlugin == OpenSteer::Plugin::selectedPlugin )
	{
		return;
	}
	OpenSteer::Plugin::closeSelectedPlugin();
	OpenSteer::Plugin::selectedPlugin = pkPlugin;
	if( NULL != Plugin::ms_on_plugin_selected_func )
	{
		Plugin::ms_on_plugin_selected_func();
	}
	OpenSteer::Plugin::openSelectedPlugin();
}

//-----------------------------------------------------------------------------
// select the "next" plug-in, cycling through "plug-in selection order"
void 
OpenSteer::Plugin::selectNextPlugin (void)
{
	if( NULL == OpenSteer::Plugin::selectedPlugin )
	{
		return;
	}
	Plugin::selectPlugin( OpenSteer::Plugin::selectedPlugin->next () );
}

//-----------------------------------------------------------------------------
// select the plug-in by index
void OpenSteer::Plugin::selectPluginByIndex (size_t idx)
{
	AbstractPlugin* p = Plugin::getPluginAt( idx );
	if( ( NULL != p ) && (p != OpenSteer::Plugin::selectedPlugin) )
	{
		Plugin::selectPlugin( p );
	}
}

//-----------------------------------------------------------------------------
// handle function keys an a per-plug-in basis
void 
OpenSteer::Plugin::functionKeyForPlugin (int keyNumber)
{
	if( NULL == OpenSteer::Plugin::selectedPlugin )
	{
		return;
	}
	OpenSteer::Plugin::selectedPlugin->handleFunctionKeys (keyNumber);
}

//-----------------------------------------------------------------------------
// return name of currently selected plug-in
const char* 
OpenSteer::Plugin::nameOfSelectedPlugin (void)
{
	return (OpenSteer::Plugin::selectedPlugin ? OpenSteer::Plugin::selectedPlugin->pluginName() : "no Plugin");
}

//-----------------------------------------------------------------------------
// open the currently selected plug-in
void 
OpenSteer::Plugin::openSelectedPlugin (void)
{
	if( NULL == OpenSteer::Plugin::selectedPlugin )
	{
		return;
	}
	OpenSteer::Camera::camera.reset ();
	SimpleVehicle::selectedVehicle = NULL;
	OpenSteer::Plugin::selectedPlugin->open ();
}

//-----------------------------------------------------------------------------
// close the currently selected plug-in
void 
OpenSteer::Plugin::closeSelectedPlugin (void)
{
	if( NULL == OpenSteer::Plugin::selectedPlugin )
	{
		return;
	}
	OpenSteer::Plugin::selectedPlugin->close ();
}

//-----------------------------------------------------------------------------
// reset the currently selected plug-in
void 
OpenSteer::Plugin::resetSelectedPlugin (void)
{
	if( NULL == OpenSteer::Plugin::selectedPlugin )
	{
		return;
	}
	OpenSteer::Plugin::selectedPlugin->reset ();
}
