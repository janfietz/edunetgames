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

#include "OpenSteerUT/PluginArray.h"

#include "EduNetCommon/EduNetDraw.h"
#include "OpenSteerUT/AbstractVehicleGroup.h"
#include "OpenSteerUT/AbstractPluginUtilities.h"


using namespace OpenSteer;


//-----------------------------------------------------------------------------
PluginArray::PluginArray( bool bAddToRegistry )
{
	if( true == bAddToRegistry )
	{
		OpenSteer::Plugin::addToRegistry( this );
	}
}

//-----------------------------------------------------------------------------
PluginArray::~PluginArray()
{
	this->removeAllPlugins();
}

//-----------------------------------------------------------------------------
AbstractPlugin* PluginArray::getParentPlugin(void) const
{
	return this->getHostPlugin();
};

//-----------------------------------------------------------------------------
void PluginArray::setParentPlugin( AbstractPlugin* pkPlugin )
{
	this->setParentEntity( OpenSteer::CastToAbstractEntity( pkPlugin) );
};

//-----------------------------------------------------------------------------
void PluginArray::removeAllPlugins( void )
{
	AbstractPlugin* pkPlugin = this->getPlugin( 0 );
	while( NULL != pkPlugin )
	{
		this->removePlugin( pkPlugin );
		pkPlugin = this->getPlugin( 0 );
	}
	bool bTest = true;
	bTest = false;
}
//-----------------------------------------------------------------------------
void PluginArray::addPlugin( AbstractPlugin* pkPlugin )
{
	if( NULL == this->findPlugin( pkPlugin ) )
	{
		AbstractPluginPtr spPlugin(pkPlugin);
		pkPlugin->setParentPlugin( this );
		this->push_back( spPlugin );
	}
}

//-----------------------------------------------------------------------------
AbstractPlugin* PluginArray::findPlugin( AbstractPlugin* pkPlugin ) const
{
	TPluginArray::const_iterator kIter = this->begin();
	TPluginArray::const_iterator kEnd = this->end();
	while( kIter != kEnd  )
	{
		AbstractPlugin* pkArrayPlugin = (*kIter).get();
		if( pkArrayPlugin == pkPlugin )
		{
			return pkPlugin;
		}
		++kIter;
	}
	return NULL;
}

//-----------------------------------------------------------------------------
void PluginArray::removePlugin( AbstractPlugin* pkPlugin )
{
	TPluginArray::iterator kIter = this->begin();
	TPluginArray::iterator kEnd = this->end();
	while( kIter != kEnd  )
	{
		AbstractPlugin* pkArrayPlugin = (*kIter).get();
		if( pkArrayPlugin == pkPlugin )
		{
			pkArrayPlugin->close();
			pkArrayPlugin->setParentPlugin( NULL );
			this->erase( kIter );
			break;
		}
		++kIter;
	}
}

//-----------------------------------------------------------------------------
AbstractPlugin* PluginArray::getPlugin( size_t uiIdx ) const
{
	if( uiIdx < this->size() )
	{
		return (*this)[uiIdx].get();
	}
	return NULL;
}

//-----------------------------------------------------------------------------
void PluginArray::open(void)
{
	TPluginArray::iterator kIter = this->begin();
	TPluginArray::iterator kEnd = this->end();
	while( kIter != kEnd  )
	{
		AbstractPlugin* pkPlugin = (*kIter).get();
		pkPlugin->open();
		++kIter;
	}
}

//-----------------------------------------------------------------------------
void PluginArray::update(const float currentTime, const float elapsedTime)
{
	if( false == this->isEnabled() )
	{
		return;
	}
	TPluginArray::iterator kIter = this->begin();
	TPluginArray::iterator kEnd = this->end();
	while( kIter != kEnd  )
	{
		AbstractPlugin* pkPlugin = (*kIter).get();
		AbstractUpdated* pkUpdatedPlugin = dynamic_cast<osAbstractUpdated*>(pkPlugin);
		assert( NULL != pkUpdatedPlugin );
		pkUpdatedPlugin->update( currentTime, elapsedTime );
		++kIter;
	}
}

//-----------------------------------------------------------------------------
void PluginArray::redraw(const float currentTime, const float elapsedTime)
{
	if( false == this->isVisible() )
	{
		return;
	}
	TPluginArray::iterator kIter = this->begin();
	TPluginArray::iterator kEnd = this->end();
	while( kIter != kEnd  )
	{
		AbstractPlugin* pkPlugin = (*kIter).get();
		pkPlugin->redraw( currentTime, elapsedTime );
		++kIter;
	}
}

//-----------------------------------------------------------------------------
void PluginArray::close(void)
{
	TPluginArray::iterator kIter = this->begin();
	TPluginArray::iterator kEnd = this->end();
	while( kIter != kEnd  )
	{
		AbstractPlugin* pkPlugin = (*kIter).get();
		pkPlugin->close(  );
		++kIter;
	}
}

//-----------------------------------------------------------------------------
void PluginArray::reset(void)
{
	this->close();
	this->open();
/*
	TPluginArray::iterator kIter = this->begin();
	TPluginArray::iterator kEnd = this->end();
	while( kIter != kEnd  )
	{
		AbstractPlugin* pkPlugin = (*kIter).get();
		pkPlugin->reset(  );
		++kIter;
	}
*/
}

//-----------------------------------------------------------------------------
float PluginArray::selectionOrderSortKey(void) const
{
	return 1.0f;
}

//-----------------------------------------------------------------------------
bool PluginArray::requestInitialSelection(void) const
{
	return false;
}

//-----------------------------------------------------------------------------
void PluginArray::handleFunctionKeys(int keyNumber)
{
	TPluginArray::iterator kIter = this->begin();
	TPluginArray::iterator kEnd = this->end();
	while( kIter != kEnd  )
	{
		AbstractPlugin* pkPlugin = (*kIter).get();
		pkPlugin->handleFunctionKeys( keyNumber );
		++kIter;
	}
}

//-----------------------------------------------------------------------------
void PluginArray::printMiniHelpForFunctionKeys(void) const
{
	TPluginArray::const_iterator kIter = this->begin();
	TPluginArray::const_iterator kEnd = this->end();
	while( kIter != kEnd  )
	{
		AbstractPlugin* pkPlugin = (*kIter).get();
		pkPlugin->printMiniHelpForFunctionKeys( );
		++kIter;
	}
}

//-----------------------------------------------------------------------------
const AVGroup& PluginArray::allVehicles(void) const
{
	{
		const AVGroup& kVehicles = this->m_kVehicles;
		if( kVehicles.size() > 0 )
		{
			return kVehicles;
		}
	}
	TPluginArray::const_iterator kIter = this->begin();
	TPluginArray::const_iterator kEnd = this->end();

	// non remote plugins first
	while( kIter != kEnd  )
	{
		AbstractPlugin* pkPlugin = (*kIter).get();
		AbstractEntity* pkPluginEntity = dynamic_cast<AbstractEntity*>( pkPlugin );
		assert( NULL != pkPluginEntity );
		if( false == pkPluginEntity->isRemoteObject() )
		{
			const AVGroup& kVehicles = pkPlugin->allVehicles();
			if( kVehicles.size() > 0 )
			{
				return kVehicles;
			}
		}
		++kIter;
	}
	kIter = this->begin();
	while( kIter != kEnd  )
	{
		AbstractPlugin* pkPlugin = (*kIter).get();
		AbstractEntity* pkPluginEntity = dynamic_cast<AbstractEntity*>( pkPlugin );
		assert( NULL != pkPluginEntity );
		if( true == pkPluginEntity->isRemoteObject() )
		{
			const AVGroup& kVehicles = pkPlugin->allVehicles();
			if( kVehicles.size() > 0 )
			{
				return kVehicles;
			}
		}
		++kIter;
	}


	return this->m_kVehicles;
}

//-----------------------------------------------------------------------------
AVGroup& PluginArray::allVehicles(void)
{
	{
		AVGroup& kVehicles = this->m_kVehicles;
		if( kVehicles.size() > 0 )
		{
			return kVehicles;
		}
	}
	TPluginArray::const_iterator kIter = this->begin();
	TPluginArray::const_iterator kEnd = this->end();
	while( kIter != kEnd  )
	{
		AbstractPlugin* pkPlugin = (*kIter).get();
		AbstractEntity* pkPluginEntity = dynamic_cast<AbstractEntity*>( pkPlugin );
		assert( NULL != pkPluginEntity );
		if( false == pkPluginEntity->isRemoteObject() )
		{
			AVGroup& kVehicles = pkPlugin->allVehicles();
			if( kVehicles.size() > 0 )
			{
				return kVehicles;
			}
		}
		++kIter;
	}
	kIter = this->begin();
	while( kIter != kEnd  )
	{
		AbstractPlugin* pkPlugin = (*kIter).get();
		AbstractEntity* pkPluginEntity = dynamic_cast<AbstractEntity*>( pkPlugin );
		assert( NULL != pkPluginEntity );
		if( true == pkPluginEntity->isRemoteObject() )
		{
			AVGroup& kVehicles = pkPlugin->allVehicles();
			if( kVehicles.size() > 0 )
			{
				return kVehicles;
			}
		}
		++kIter;
	}
	return this->m_kVehicles;
}

//-----------------------------------------------------------------------------
void PluginArray::addVehicle ( OpenSteer::AbstractVehicle* pkVehicle)
{
	assert( true == false );
}
//-----------------------------------------------------------------------------
void PluginArray::removeVehicle ( OpenSteer::AbstractVehicle* pkVehicle)
{
	assert( true == false );
}
//-----------------------------------------------------------------------------
void PluginArray::addObstacle ( OpenSteer::AbstractObstacle* pkObstacle)
{
	if( NULL == pkObstacle )
	{
		return;
	}
	pkObstacle->setParentEntity( this );
	this->allObstacles().push_back( pkObstacle );
}
//-----------------------------------------------------------------------------
void PluginArray::removeObstacle ( OpenSteer::AbstractObstacle* pkObstacle)
{
	ObstacleGroup::iterator kIter = std::find( this->m_kAllObstacles.begin(), this->m_kAllObstacles.end(), pkObstacle );
	if( kIter != this->m_kAllObstacles.end() )
	{
		pkObstacle->setParentEntity( NULL );
		this->m_kAllObstacles.erase( kIter );
	}
}
//-----------------------------------------------------------------------------
void PluginArray::addPlayer ( OpenSteer::AbstractPlayer* pkPlayer)
{
	if( NULL == pkPlayer )
	{
		return;
	}
	pkPlayer->setParentEntity( this );
	this->allPlayers().push_back( pkPlayer );
}
//-----------------------------------------------------------------------------
void PluginArray::removePlayer ( OpenSteer::AbstractPlayer* pkPlayer)
{
	AbstractPlayerGroup::iterator kIter = std::find( this->m_kAllPlayers.begin(), this->m_kAllPlayers.end(), pkPlayer );
	if( kIter != this->m_kAllPlayers.end() )
	{
		pkPlayer->setParentEntity( NULL );
		this->m_kAllPlayers.erase( kIter );
	}
}
//-----------------------------------------------------------------------------
AbstractPlugin* PluginArray::next(void) const
{
	return OpenSteer::Plugin::findNextPlugin( this );
}

// implement to initialize additional gui functionality
//-----------------------------------------------------------------------------
void PluginArray::initGui( void* pkUserdata )
{
	GLUI* glui = ::getRootGLUI();
	GLUI_Panel* pluginPanel = static_cast<GLUI_Panel*>(pkUserdata);

	TPluginArray::iterator kIter = this->begin();
	TPluginArray::iterator kEnd = this->end();
	while( kIter != kEnd  )
	{
		AbstractPlugin* pkPlugin = (*kIter).get();
		GLUI_Panel* subPluginPanel = AbstractPluginGui::initSubPluginGui( pkPlugin, pkUserdata );
		if( NULL != subPluginPanel )
		{

		}
		++kIter;
	}
}

//-----------------------------------------------------------------------------
class FooPlugin : public Plugin
{
	ET_DECLARE_BASE(OpenSteer::Plugin)
public:
	FooPlugin (bool bAddToRegistry = false):BaseClass(bAddToRegistry){};

	// required methods:
	const char* name (void) const {return "Foo";}
	void open (void) { }
	void update (const float currentTime, const float elapsedTime) { }
	void redraw (const float currentTime, const float elapsedTime) { }
	void close (void) { }
	const AVGroup& allVehicles (void) const { return m_kVehicles; }

	// optional methods (see comments in AbstractPlugin for explanation):
	void reset (void) { } // default is to reset by doing close-then-open
	float selectionOrderSortKey (void) const {return 1234;}
	bool requestInitialSelection (void) const {return false;}
	void handleFunctionKeys (int keyNumber) { } // fkeys reserved for Plugins
	void printMiniHelpForFunctionKeys (void) { } // if fkeys are used
private:
	AVGroup m_kVehicles;
	ET_IMPLEMENT_CLASS_NO_COPY(FooPlugin)

};



//PluginArray gTestPluginArray;

void PluginArray::TestPluginArray( void )
{
	{
		PluginArray gTestPluginArray;

		PluginArray* pkAdd[] =
		{
			ET_NEW PluginArray,
			ET_NEW PluginArray,
			ET_NEW PluginArray
		};
		gTestPluginArray.addPlugin( pkAdd[0] );
		gTestPluginArray.addPlugin( pkAdd[1] );
		gTestPluginArray.addPlugin( pkAdd[2] );


		gTestPluginArray.removePlugin( pkAdd[0] );

		bool bTest = true;
		bTest = false;
	}

}
