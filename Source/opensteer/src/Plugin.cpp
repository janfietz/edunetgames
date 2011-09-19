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


#include "OpenSteer/Plugin.h"
#include "OpenSteer/SimplePlayer.h"
#include "OpenSteer/SimpleVehicle.h"
#include "OpenSteer/Camera.h"
#include "OpenSteer/PluginRegistry.h"

//-----------------------------------------------------------------------------
// constructor
OpenSteer::Plugin::Plugin( bool bAddToRegistry ):
	m_pkEntityFactory( NULL )
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
	if( this == OpenSteer::Plugin::getSelectedPlugin() )
	{
		OpenSteer::Plugin::selectPlugin( NULL );
	}
	PluginRegistry::accessInstance()->removeFromRegistry( this );
}

//-----------------------------------------------------------------------------
// default reset method is to do a close then an open
void OpenSteer::Plugin::reset (void) 
{
	close (); 
	open ();
}

//-----------------------------------------------------------------------------
OpenSteer::AbstractEntity*
OpenSteer::Plugin::createEntity( EntityClassId classId ) const
{
	OpenSteer::AbstractEntity* pkEntity = Plugin::createSystemEntity( classId );
	if( NULL == pkEntity )
	{
		pkEntity = this->createVehicle( classId );
	}
	return pkEntity;
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
	return PluginRegistry::accessInstance()->findNextPlugin( this );
}

//-----------------------------------------------------------------------------
OpenSteer::AbstractPlugin*
OpenSteer::Plugin::findNextPlugin( const AbstractPlugin* pkThis )
{
	return PluginRegistry::accessInstance()->findNextPlugin( pkThis );
}

//-----------------------------------------------------------------------------
int OpenSteer::Plugin::getPluginIdx( const AbstractPlugin* pkPlugin )
{
	return PluginRegistry::accessInstance()->getPluginIdx(pkPlugin);
}

//-----------------------------------------------------------------------------
// search the class registry for a Plugin with the given name
// returns NULL if none is found
OpenSteer::AbstractPlugin*
OpenSteer::Plugin::findByName (const char* string)
{
	return PluginRegistry::accessInstance()->findByName(string);
}

//-----------------------------------------------------------------------------
// apply a given function to all Plugins in the registry
void
OpenSteer::Plugin::applyToAll( plugInCallBackFunction f )
{
	PluginRegistry::accessInstance()->applyToAll(f);
}

//-----------------------------------------------------------------------------
// sort Plugin registry by "selection order"
//
// XXX replace with STL utilities
void
OpenSteer::Plugin::sortBySelectionOrder (void)
{
	PluginRegistry::accessInstance()->sortBySelectionOrder();
}

//-----------------------------------------------------------------------------
// returns pointer to default Plugin (currently, first in registry)
OpenSteer::AbstractPlugin*
OpenSteer::Plugin::findDefault (void)
{
	return PluginRegistry::accessInstance()->findDefault();
}

//-----------------------------------------------------------------------------
// save this instance in the class's registry of instances
// (for use by contractors)
void
OpenSteer::Plugin::addToRegistry (AbstractPlugin* pkPlugin)
{
	PluginRegistry::accessInstance()->addToRegistry(pkPlugin);
}

//-----------------------------------------------------------------------------
// return a group (an STL vector of AbstractVehicle pointers) of all
// vehicles(/agents/characters) defined by the currently selected Plugin
const OpenSteer::AVGroup&
OpenSteer::Plugin::allVehiclesOfSelectedPlugin (void)
{
	return PluginRegistry::accessInstance()->allVehiclesOfSelectedPlugin();
}

//-----------------------------------------------------------------------------
void
OpenSteer::Plugin::selectPlugin( AbstractPlugin* pkPlugin )
{
	PluginRegistry::accessInstance()->selectPlugin(pkPlugin);
}

//-----------------------------------------------------------------------------
// select the "next" plug-in, cycling through "plug-in selection order"
void
OpenSteer::Plugin::selectNextPlugin (void)
{
	PluginRegistry::accessInstance()->selectNextPlugin();
}

//-----------------------------------------------------------------------------
// select the plug-in by index
void OpenSteer::Plugin::selectPluginByIndex (size_t idx)
{
	PluginRegistry::accessInstance()->selectPluginByIndex(idx);
}

//-----------------------------------------------------------------------------
// handle function keys an a per-plug-in basis
void
OpenSteer::Plugin::functionKeyForPlugin (int keyNumber)
{
	PluginRegistry::accessInstance()->functionKeyForPlugin(keyNumber);
}

//-----------------------------------------------------------------------------
// return name of currently selected plug-in
const char*
OpenSteer::Plugin::nameOfSelectedPlugin (void)
{
	return PluginRegistry::accessInstance()->nameOfSelectedPlugin();
}

//-----------------------------------------------------------------------------
// reset the currently selected plug-in
void
OpenSteer::Plugin::resetSelectedPlugin (void)
{
	PluginRegistry::accessInstance()->resetSelectedPlugin();
}

//-----------------------------------------------------------------------------
int OpenSteer::Plugin::getNumPlugins( void ) 
{ 
	return PluginRegistry::accessInstance()->getNumPlugins(); 
}

//-----------------------------------------------------------------------------
OpenSteer::AbstractPlugin* OpenSteer::Plugin::getPluginAt( size_t idx ) 
{ 
	return PluginRegistry::accessInstance()->getPluginAt(idx); 
}

OpenSteer::AbstractPlugin* OpenSteer::Plugin::getSelectedPlugin( void )
{
	return PluginRegistry::accessInstance()->getSelectedPlugin();
}

//-----------------------------------------------------------------------------
void OpenSteer::Plugin::addVehicle (
	OpenSteer::AbstractVehicle* pkVehicle)
{
	assert( true == false );
}

//-----------------------------------------------------------------------------
void OpenSteer::Plugin::removeVehicle (
	OpenSteer::AbstractVehicle* pkVehicle)
{
	assert( true == false );
}

//-----------------------------------------------------------------------------
bool OpenSteer::Plugin::queryVehicleColor( const OpenSteer::AbstractVehicle& kVehicle, OpenSteer::Color& kColor ) const 
{ 
	AbstractPlugin* pkParent = this->getParentPlugin();
	if( NULL != pkParent )
	{
		return pkParent->queryVehicleColor( kVehicle, kColor );
	}

	return false; 
}

//-----------------------------------------------------------------------------
void OpenSteer::Plugin::addObstacle (
	OpenSteer::AbstractObstacle* pkObstacle)
{
	if( NULL == pkObstacle )
	{
		return;
	}
	pkObstacle->setParentEntity( this );
	this->allObstacles().push_back( pkObstacle );
}

//-----------------------------------------------------------------------------
void OpenSteer::Plugin::removeObstacle (
	OpenSteer::AbstractObstacle* pkObstacle)
{
	ObstacleGroup::iterator kIter = std::find( this->m_kAllObstacles.begin(), this->m_kAllObstacles.end(), pkObstacle );
	if( kIter != this->m_kAllObstacles.end() )
	{
		pkObstacle->setParentEntity( NULL );
		this->m_kAllObstacles.erase( kIter );
	}
}

//-----------------------------------------------------------------------------
void OpenSteer::Plugin::addPlayer (
	OpenSteer::AbstractPlayer* pkPlayer)
{
	if( NULL == pkPlayer )
	{
		return;
	}
	pkPlayer->setParentEntity( this );
	this->allPlayers().push_back( pkPlayer );
}

//-----------------------------------------------------------------------------
void OpenSteer::Plugin::removePlayer (
	OpenSteer::AbstractPlayer* pkPlayer)
{
	AbstractPlayerGroup::iterator kIter = std::find( this->m_kAllPlayers.begin(), this->m_kAllPlayers.end(), pkPlayer );
	if( kIter != this->m_kAllPlayers.end() )
	{
		pkPlayer->setParentEntity( NULL );
		this->m_kAllPlayers.erase( kIter );
	}
}
