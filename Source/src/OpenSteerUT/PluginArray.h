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

//#include "EduNetCommon/EduNetCommon.h"
#include "EduNetCore/EduNetMacros.h"
#include "OpenSteer/Plugin.h"
#include "boost/smart_ptr.hpp"


//-----------------------------------------------------------------------------
namespace OpenSteer
{
	typedef boost::shared_ptr<AbstractPlugin> AbstractPluginPtr;
	typedef std::vector<AbstractPluginPtr> TPluginArray;



	//-------------------------------------------------------------------------
	class PluginArray : public EntityLocalSpace, public AbstractPlugin, protected TPluginArray
	{
	public:
		PluginArray(bool bAddToRegistry = false);
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
		virtual const char* pluginName (void) const
		{
			return this->name();
		}
		virtual void prepareOpen (void);
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


		virtual AbstractPlugin* next(void) const;

		//! returns pointer to the parent Plugin
		virtual AbstractPlugin* getParentPlugin(void) const;
		//! set a parent Plugin
		virtual void setParentPlugin( AbstractPlugin* pkPlugin );

		// implement to initialize additional gui functionality
		virtual void initGui( void* pkUserdata );

		// implement to create an entity of the specified class
		virtual AbstractEntity* createEntity( EntityClassId classId ) const
		{
			return Plugin::createSystemEntity( classId );
		}

		// implement to create a vehicle of the specified class
		virtual AbstractVehicle* createVehicle( EntityClassId ) const { return NULL; };

		// set an external vehicle factory
		virtual void setEntityFactory( AbstractEntityFactory* ){};

		virtual AbstractEntityFactory* getEntityFactory( void ) const {return NULL;};

		virtual const AVGroup& allVehicles(void) const;
		virtual AVGroup& allVehicles(void);
		virtual void addVehicle (OpenSteer::AbstractVehicle* pkVehicle);
		virtual void removeVehicle (OpenSteer::AbstractVehicle* pkVehicle);

		virtual bool queryVehicleColor( const OpenSteer::AbstractVehicle& kVehicle, OpenSteer::Color& kColor ) const;

		virtual ObstacleGroup& allObstacles( void ) { return m_kAllObstacles; };
		virtual const ObstacleGroup& allObstacles( void ) const { return m_kAllObstacles; };
		virtual void addObstacle (OpenSteer::AbstractObstacle* pkObstacle);
		virtual void removeObstacle (OpenSteer::AbstractObstacle* pkObstacle);

		virtual AbstractPlayerGroup& allPlayers( void ) { return m_kAllPlayers; };
		virtual const AbstractPlayerGroup& allPlayers( void ) const { return m_kAllPlayers; };
		virtual void addPlayer (OpenSteer::AbstractPlayer* pkPlayer);
		virtual void removePlayer (OpenSteer::AbstractPlayer* pkPlayer);
	protected:
		void redrawChildren(const float currentTime, const float elapsedTime);

	private:
		AbstractPlayerGroup m_kAllPlayers;
		ObstacleGroup m_kAllObstacles;
		AVGroup m_kVehicles;

	};

	//-------------------------------------------------------------------------
	template <class Super>
	class PluginArrayMixin : public Super
	{
		ET_DECLARE_BASE( Super )
	public:
		PluginArrayMixin(bool bAddToRegistry = true):Super( bAddToRegistry ),m_kPluginArray( false ) {};
		virtual ~PluginArrayMixin() {};

		//---------------------------------------------------------------------
		// functionality PluginArray
		void addPlugin( AbstractPlugin* pkPlugin )
		{
			m_kPluginArray.addPlugin( pkPlugin );
		}
		void removePlugin( AbstractPlugin* pkPlugin )
		{
			m_kPluginArray.removePlugin( pkPlugin );
		}
		void removeAllPlugins( void )
		{
			m_kPluginArray.removeAllPlugins( );
		}
		AbstractPlugin* findPlugin( AbstractPlugin* pkPlugin ) const
		{
			return m_kPluginArray.findPlugin( pkPlugin );
		}
		AbstractPlugin* getPlugin( size_t uiIdx ) const
		{
			return m_kPluginArray.getPlugin( uiIdx );
		}
	protected:
		PluginArray m_kPluginArray;
	};

	//-------------------------------------------------------------------------
	template <class Super>
	class PluginArrayPluginMixin : public PluginArrayMixin<Super>
	{
		ET_DECLARE_BASE( PluginArrayMixin<Super> )
	public:
		PluginArrayPluginMixin( bool bAddToRegistry = true ):BaseClass( bAddToRegistry ) {};
		virtual ~PluginArrayPluginMixin() {};

		virtual void open(void)
		{
			this->m_kPluginArray.open(  );
			Super::open();
		}
		virtual void update(const float currentTime, const float elapsedTime)
		{
			if( false == this->isEnabled() )
			{
				return;
			}

			const char* pszClassName = this->name();
			this->m_kPluginArray.update( currentTime, elapsedTime );
			Super::update( currentTime, elapsedTime );
		}
		virtual void redraw(const float currentTime, const float elapsedTime)
		{
			if( false == this->isVisible() )
			{
				return;
			}
			this->m_kPluginArray.redraw( currentTime, elapsedTime );
			Super::redraw( currentTime, elapsedTime );
		}
		virtual void close(void)
		{
			this->m_kPluginArray.close(  );
			Super::close();
		}

		virtual void reset(void)
		{
			this->m_kPluginArray.reset(  );
			Super::reset();
		}

		virtual void initGui( void* pkUserdata )
		{
			this->m_kPluginArray.initGui( pkUserdata );
			Super::initGui( pkUserdata );
		}

	private:
	};

}


#endif // __PLUGINARRAY_H__
