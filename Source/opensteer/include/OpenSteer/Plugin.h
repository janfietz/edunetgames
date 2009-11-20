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
#include "OpenSteer/Obstacle.h"

//-----------------------------------------------------------------------------
namespace OpenSteer {

	class AbstractVehicleFactory;

	//-------------------------------------------------------------------------
    class AbstractPlugin
    {
    public:
        
        virtual ~AbstractPlugin() { /* Nothing to do. */ }
        
        //! generic Plugin actions: open, update, redraw, close and reset
        virtual void open (void) = 0;
        virtual void update (const float currentTime, const float elapsedTime) = 0;
        virtual void redraw (const float currentTime, const float elapsedTime) = 0;
        virtual void close (void) = 0;
        virtual void reset (void) = 0;

        //! return a pointer to this instance's character string name
        virtual const char* name (void) const = 0;

        //! numeric sort key used to establish user-visible Plugin ordering
        //! ("built ins" have keys greater than 0 and less than 1)
        virtual float selectionOrderSortKey (void) const = 0;

        //! allows a Plugin to nominate itself as OpenSteerDemo's initially selected
        //! (default) Plugin, which is otherwise the first in "selection order"
        virtual bool requestInitialSelection (void) const = 0;

        //! handle function keys (which are reserved by SterTest for Plugins)
        virtual void handleFunctionKeys (int keyNumber) = 0;

        //! print "mini help" documenting function keys handled by this Plugin
        virtual void printMiniHelpForFunctionKeys (void) const = 0;

		//! return the current vehicle proximity database for this plugin
		virtual ProximityDatabase* accessProximityDataBase( void ) const = 0;

       //! return an AVGroup (an STL vector of AbstractVehicle pointers) of
        //! all vehicles(/agents/characters) defined by the Plugin
        virtual const AVGroup& allVehicles (void) const = 0;
  
		//! returns pointer to the next Plugin in "selection order"
		virtual AbstractPlugin* next(void) const = 0;

		//! returns pointer to the parent Plugin
		virtual AbstractPlugin* getParentPlugin(void) const = 0;

		//! set a parent Plugin
		virtual void setParentPlugin( AbstractPlugin* ) = 0;

		//! implement to initialize additional gui functionality
		virtual void initGui( void* /*pkUserdata*/ ) = 0;

		//! set an external vehicle factory
		virtual void setVehicleFactory( AbstractVehicleFactory* ) = 0;

		virtual AbstractVehicleFactory* getVehicleFactory( void ) const = 0;

		//! implement to create a vehicle of the specified class
		virtual AbstractEntity* createEntity( EntityClassId ) const = 0;

		//! implement to create a vehicle of the specified class
		virtual AbstractVehicle* createVehicle( EntityClassId, ProximityDatabase* ) const = 0;

		//! format instance to characters for printing to stream
		friend std::ostream& operator<< (std::ostream& os, AbstractPlugin& pi)
		{
			os << "<Plugin " << '"' << pi.name() << '"' << ">";
			return os;
		}
	
	};

	//-------------------------------------------------------------------------
	class Plugin : public EntityLocalSpace, public AbstractPlugin
    {
    public:
		// currently selected plug-in (user can choose or cycle through them)
		static AbstractPlugin* selectedPlugin;

        //! prototypes for function pointers used with Plugins
        typedef void (* plugInCallBackFunction) (AbstractPlugin& clientObject);
        typedef void (* voidCallBackFunction) (void);
        typedef void (* timestepCallBackFunction) (const float currentTime,
                                                   const float elapsedTime);

        //! constructor
        Plugin( bool bAddToRegistry = true );

        //! destructor
        virtual ~Plugin();

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
		virtual void setVehicleFactory( AbstractVehicleFactory* pkVehicleFactory ) { this->m_pkVehicleFactory = pkVehicleFactory; }

		virtual AbstractVehicleFactory* getVehicleFactory( void ) const { return this->m_pkVehicleFactory; };

		virtual AbstractEntity* createEntity( EntityClassId classId ) const
		{
			return Plugin::createSystemEntity( classId );
		}

		//! implement to create a vehicle of the specified class
		virtual AbstractVehicle* createVehicle( EntityClassId, ProximityDatabase* ) const { return NULL; };
        
		//! format instance to characters for printing to stream
        friend std::ostream& operator<< (std::ostream& os, Plugin& pi)
        {
            os << "<Plugin " << '"' << pi.name() << '"' << ">";
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
	protected:
		AbstractVehicleFactory* m_pkVehicleFactory;

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
