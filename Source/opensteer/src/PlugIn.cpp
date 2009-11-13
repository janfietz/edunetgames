// ----------------------------------------------------------------------------
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
// ----------------------------------------------------------------------------
//
//
// OpenSteerDemo Plugin class
// 
// 10-04-04 bk:  put everything into the OpenSteer namespace
// 11-13-02 cwr: created 
//
//
// ----------------------------------------------------------------------------


#include "OpenSteer/PlugIn.h"


// ----------------------------------------------------------------------------
// Plugin registry
//
// XXX replace with STL utilities


int OpenSteer::Plugin::itemsInRegistry = 0;
const int OpenSteer::Plugin::totalSizeOfRegistry = 1000;
OpenSteer::AbstractPlugin* OpenSteer::Plugin::registry [totalSizeOfRegistry];


// ----------------------------------------------------------------------------
// constructor
OpenSteer::Plugin::Plugin( bool bAddToRegistry ):
	m_pkParentPlugin( NULL )
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
            AbstractPlugin& pi = *registry[i];
            const char* s = pi.name();
            if (s && (strcmp (string, s) == 0)) return &pi;
        }
    }
    return NULL;
}

//-----------------------------------------------------------------------------
// apply a given function to all Plugins in the registry
void 
OpenSteer::Plugin::applyToAll (plugInCallBackFunction f)
{
    for (int i = 0; i < itemsInRegistry; i++)
    {
        f (*registry[i]);
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


// ----------------------------------------------------------------------------
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


// ----------------------------------------------------------------------------
