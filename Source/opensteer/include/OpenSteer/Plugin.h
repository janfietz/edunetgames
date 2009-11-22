#ifndef OPENSTEER_PLUGIN_H
#define OPENSTEER_PLUGIN_H

//-----------------------------------------------------------------------------
//
//
//! OpenSteer -- Steering Behaviors for Autonomous Characters
//
//! Copyright (c) 2002-2005, Sony Computer Entertainment America
//! Original author: Craig Reynolds <craig_reynolds@playstation.sony.com>
//
//! Permission is hereby granted, free of charge, to any person obtaining a
//! copy of this software and associated documentation files (the "Software"),
//! to deal in the Software without restriction, including without limitation
//! the rights to use, copy, modify, merge, publish, distribute, sublicense,
//! and/or sell copies of the Software, and to permit persons to whom the
//! Software is furnished to do so, subject to the following conditions:
//
//! The above copyright notice and this permission notice shall be included in
//! all copies or substantial portions of the Software.
//
//! THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//! IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//! FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
//! THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//! LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
//! FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
//! DEALINGS IN THE SOFTWARE.
//
//
//-----------------------------------------------------------------------------
//
//
//! OpenSteerDemo Plugin class
//
//! Provides AbstractPlugin a pure abstract base class, and Plugin a partial
//! implementation providing default methods to be sub-classed by the
//! programmer defining a new "MyPlugin".
//
//! 10-04-04 bk:  put everything into the OpenSteer namespace
//! 11-13-02 cwr: created 
//
//
//-----------------------------------------------------------------------------
//! A pattern for a specific user-defined OpenSteerDemo Plugin class called Foo.
//! Defines class FooPlugin, then makes a single instance (singleton) of it.
/*


class FooPlugin : public Plugin
{
    //! required methods:
    const char* name (void) const {return "Foo";}
    void open (void) {...}
    void update (const float currentTime, const float elapsedTime) {...}
    void redraw (const float currentTime, const float elapsedTime) {...}
    void close (void) {...}
    const AVGroup& allVehicles (void) const {...}

    //! optional methods (see comments in AbstractPlugin for explanation):
    void reset (void) {...} //! default is to reset by doing close-then-open
    float selectionOrderSortKey (void) const {return 1234;}
    bool requestInitialSelection (void) const {return true;}
    void handleFunctionKeys (int keyNumber) {...} //! fkeys reserved for Plugins
    void printMiniHelpForFunctionKeys (void) {...} //! if fkeys are used
};

FooPlugin gFooPlugin;


*/
//-----------------------------------------------------------------------------

#include <iostream>
#include "OpenSteer/Entity.h"
#include "OpenSteer/AbstractVehicle.h"
#include "OpenSteer/AbstractPlayer.h"
#include "OpenSteer/Obstacle.h"


typedef void (*on_plugin_selected_func)( void );

//-----------------------------------------------------------------------------
namespace OpenSteer {

	class AbstractEntityFactory;

	//-------------------------------------------------------------------------
    class AbstractPlugin
    {
    public:
        
        virtual ~AbstractPlugin() { /* Nothing to do. */ }
        
        //! generic Plugin actions: open, update, redraw, close and reset
        virtual void open (void) OS_ABSTRACT;
        virtual void update (const float currentTime, const float elapsedTime) OS_ABSTRACT;
        virtual void redraw (const float currentTime, const float elapsedTime) OS_ABSTRACT;
        virtual void close (void) OS_ABSTRACT;
        virtual void reset (void) OS_ABSTRACT;

        //! return a pointer to this instance's character string name
        virtual const char* pluginName (void) const OS_ABSTRACT;

        //! numeric sort key used to establish user-visible Plugin ordering
        //! ("built ins" have keys greater than 0 and less than 1)
        virtual float selectionOrderSortKey (void) const OS_ABSTRACT;

        //! allows a Plugin to nominate itself as OpenSteerDemo's initially selected
        //! (default) Plugin, which is otherwise the first in "selection order"
        virtual bool requestInitialSelection (void) const OS_ABSTRACT;

        //! handle function keys (which are reserved by SterTest for Plugins)
        virtual void handleFunctionKeys (int keyNumber) OS_ABSTRACT;

        //! print "mini help" documenting function keys handled by this Plugin
        virtual void printMiniHelpForFunctionKeys (void) const OS_ABSTRACT;

		//! return the current vehicle proximity database for this plugin
		virtual ProximityDatabase* accessProximityDataBase( void ) const OS_ABSTRACT;

       //! return an AVGroup (an STL vector of AbstractVehicle pointers) of
        //! all vehicles(/agents/characters) defined by the Plugin
        virtual const AVGroup& allVehicles (void) const OS_ABSTRACT;
		virtual AVGroup& allVehicles (void) OS_ABSTRACT;

		virtual ObstacleGroup& allObstacles( void ) OS_ABSTRACT;
		virtual const ObstacleGroup& allObstacles( void ) const OS_ABSTRACT;

		virtual PlayerGroup& allPlayers( void ) OS_ABSTRACT;
		virtual const PlayerGroup& allPlayers( void ) const OS_ABSTRACT;
		
 
		//! returns pointer to the next Plugin in "selection order"
		virtual AbstractPlugin* next(void) const OS_ABSTRACT;

		//! returns pointer to the parent Plugin
		virtual AbstractPlugin* getParentPlugin(void) const OS_ABSTRACT;

		//! set a parent Plugin
		virtual void setParentPlugin( AbstractPlugin* ) OS_ABSTRACT;

		//! implement to initialize additional gui functionality
		virtual void initGui( void* /*pkUserdata*/ ) OS_ABSTRACT;

		//! set an external vehicle factory
		virtual void setEntityFactory( AbstractEntityFactory* ) OS_ABSTRACT;

		virtual AbstractEntityFactory* getEntityFactory( void ) const OS_ABSTRACT;

		//! implement to create a vehicle of the specified class
		virtual AbstractEntity* createEntity( EntityClassId ) const OS_ABSTRACT;

		//! implement to create a vehicle of the specified class
		virtual AbstractVehicle* createVehicle( EntityClassId ) const OS_ABSTRACT;

		//! format instance to characters for printing to stream
		friend std::ostream& operator<< (std::ostream& os, AbstractPlugin& pi)
		{
			os << "<Plugin " << '"' << pi.pluginName() << '"' << ">";
			return os;
		}
	
	};

	//-------------------------------------------------------------------------
	class Plugin : public EntityLocalSpace, public AbstractPlugin
    {
    public:
        //! prototypes for function pointers used with Plugins
        typedef void (* plugInCallBackFunction) ( AbstractPlugin& clientObject );
        typedef void (* voidCallBackFunction) (void);
        typedef void (* timestepCallBackFunction) (const float currentTime,
                                                   const float elapsedTime);

		// TODO: make this one private		
		//! currently selected plug-in (user can choose or cycle through them)
		static AbstractPlugin* selectedPlugin;


        //! constructor
        Plugin( bool bAddToRegistry = true );

        //! destructor
        virtual ~Plugin();

		//! return a pointer to this instance's character string name
		virtual const char* pluginName (void) const
		{
			return this->name();
		}

       //! default reset method is to do a close then an open
		void reset (void) {close (); open ();}

        //! default sort key (after the "built ins")
        float selectionOrderSortKey (void) const {return 1.0f;}

        //! default is to NOT request to be initially selected
        bool requestInitialSelection (void) const {return false;}

        //! default function key handler: ignore all
        //! (parameter names commented out to prevent compiler warning from "-W")
        void handleFunctionKeys (int /*keyNumber*/) {}

        //! default "mini help": print nothing
        void printMiniHelpForFunctionKeys (void) const {}

		//! return the current vehicle proximity database for this plugin
		virtual ProximityDatabase* accessProximityDataBase( void ) const { return NULL; };

        //! returns pointer to the next Plugin in "selection order"
        AbstractPlugin* next (void) const;

		//! returns pointer to the parent Plugin
		virtual AbstractPlugin* getParentPlugin(void) const { return m_pkParentPlugin; };

		//! set a parent Plugin
		virtual void setParentPlugin( AbstractPlugin* pkPlugin ) { m_pkParentPlugin = pkPlugin; };

		//! implement to initialize additional gui functionality
		virtual void initGui( void* /*pkUserdata*/ ) {};

		//! set an external vehicle factory
		virtual void setEntityFactory( AbstractEntityFactory* pkEntityFactory ) { this->m_pkEntityFactory = pkEntityFactory; }

		virtual AbstractEntityFactory* getEntityFactory( void ) const { return this->m_pkEntityFactory; };

		virtual AbstractEntity* createEntity( EntityClassId classId ) const;

		//! implement to create a vehicle of the specified class
		virtual AbstractVehicle* createVehicle( EntityClassId ) const { return NULL; };

		virtual ObstacleGroup& allObstacles( void ) { return m_kAllObstacles; };
		virtual const ObstacleGroup& allObstacles( void ) const { return m_kAllObstacles; };

		virtual PlayerGroup& allPlayers( void ) { return m_kAllPlayers; };
		virtual const PlayerGroup& allPlayers( void ) const { return m_kAllPlayers; };
        
		//! format instance to characters for printing to stream
        friend std::ostream& operator<< (std::ostream& os, Plugin& pi)
        {
            os << "<Plugin " << '"' << pi.pluginName() << '"' << ">";
            return os;
        }

        //! CLASS FUNCTIONS

		static AbstractEntity* createSystemEntity( EntityClassId );


        //! search the class registry for a Plugin with the given name
        static AbstractPlugin* findByName (const char* string);

        //! apply a given function to all Plugins in the class registry
        static void applyToAll (plugInCallBackFunction f);

        //! sort Plugin registry by "selection order"
        static void sortBySelectionOrder (void);

        //! returns pointer to default Plugin (currently, first in registry)
        static AbstractPlugin* findDefault (void);

		//! save this instance in the class's registry of instances
		static void addToRegistry (AbstractPlugin*);
		static AbstractPlugin* findNextPlugin( const AbstractPlugin* pkThis );

		//! 
		static int getNumPlugins( void ) { return Plugin::itemsInRegistry; };
		static AbstractPlugin* getPluginAt( size_t idx ) { return registry[idx]; };
		static int getPluginIdx( const AbstractPlugin* pkPlugin );

		//! utility function
		static const AVGroup& allVehiclesOfSelectedPlugin(void);

		// not public right now ... needs to forward gui update
		// select the "next" plug-in, cycling through "plug-in selection order"
		static void selectNextPlugin (void);

		static void selectPlugin( AbstractPlugin* pkPlugin );

		// select the plug-in by index
		static void selectPluginByIndex( size_t idx );

		// handle function keys an a per-plug-in basis
		static void functionKeyForPlugin( int keyNumber );

		// return name of currently selected plug-in
		static const char* nameOfSelectedPlugin( void );

		// open the currently selected plug-in
		static void openSelectedPlugin( void );

		// close the currently selected plug-in
		static void closeSelectedPlugin( void );

		// reset the currently selected plug-in
		static void resetSelectedPlugin( void );

		static on_plugin_selected_func ms_on_plugin_selected_func;
	protected:
		AbstractEntityFactory* m_pkEntityFactory;
		ObstacleGroup m_kAllObstacles;
		PlayerGroup m_kAllPlayers;

    private:

		AbstractPlugin* m_pkParentPlugin;

        //! This array stores a list of all Plugins.  It is manipulated by the
        //! constructor and destructor, and used in findByName and applyToAll.
        static const int totalSizeOfRegistry;
        static int itemsInRegistry;
        static AbstractPlugin* registry[];
    };

} //! namespace OpenSteer    
    

//-----------------------------------------------------------------------------
#endif //! OPENSTEER_PLUGIN_H
