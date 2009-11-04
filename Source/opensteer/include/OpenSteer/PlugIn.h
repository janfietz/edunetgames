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
// Provides AbstractPlugIn a pure abstract base class, and Plugin a partial
// implementation providing default methods to be sub-classed by the
// programmer defining a new "MyPlugIn".
//
// 10-04-04 bk:  put everything into the OpenSteer namespace
// 11-13-02 cwr: created 
//
//
// ----------------------------------------------------------------------------
// A pattern for a specific user-defined OpenSteerDemo Plugin class called Foo.
// Defines class FooPlugIn, then makes a single instance (singleton) of it.
/*


class FooPlugIn : public Plugin
{
    // required methods:
    const char* name (void) const {return "Foo";}
    void open (void) {...}
    void update (const float currentTime, const float elapsedTime) {...}
    void redraw (const float currentTime, const float elapsedTime) {...}
    void close (void) {...}
    const AVGroup& allVehicles (void) const {...}

    // optional methods (see comments in AbstractPlugIn for explanation):
    void reset (void) {...} // default is to reset by doing close-then-open
    float selectionOrderSortKey (void) const {return 1234;}
    bool requestInitialSelection (void) const {return true;}
    void handleFunctionKeys (int keyNumber) {...} // fkeys reserved for PlugIns
    void printMiniHelpForFunctionKeys (void) {...} // if fkeys are used
};

FooPlugIn gFooPlugIn;


*/
// ----------------------------------------------------------------------------


#ifndef OPENSTEER_PLUGIN_H
#define OPENSTEER_PLUGIN_H

#include <iostream>
#include "OpenSteer/AbstractVehicle.h"


// ----------------------------------------------------------------------------


namespace OpenSteer {

    class AbstractPlugIn
    {
    public:
        
        virtual ~AbstractPlugIn() { /* Nothing to do. */ }
        
        // generic Plugin actions: open, update, redraw, close and reset
        virtual void open (void) = 0;
        virtual void update (const float currentTime, const float elapsedTime) = 0;
        virtual void redraw (const float currentTime, const float elapsedTime) = 0;
        virtual void close (void) = 0;
        virtual void reset (void) = 0;

        // return a pointer to this instance's character string name
        virtual const char* name (void) const = 0;

        // numeric sort key used to establish user-visible Plugin ordering
        // ("built ins" have keys greater than 0 and less than 1)
        virtual float selectionOrderSortKey (void) const = 0;

        // allows a Plugin to nominate itself as OpenSteerDemo's initially selected
        // (default) Plugin, which is otherwise the first in "selection order"
        virtual bool requestInitialSelection (void) const = 0;

        // handle function keys (which are reserved by SterTest for PlugIns)
        virtual void handleFunctionKeys (int keyNumber) = 0;

        // print "mini help" documenting function keys handled by this Plugin
        virtual void printMiniHelpForFunctionKeys (void) const = 0;

        // return an AVGroup (an STL vector of AbstractVehicle pointers) of
        // all vehicles(/agents/characters) defined by the Plugin
        virtual const AVGroup& allVehicles (void) const = 0;
  
		// returns pointer to the next Plugin in "selection order"
		virtual AbstractPlugIn* next(void) const = 0;

		// format instance to characters for printing to stream
		friend std::ostream& operator<< (std::ostream& os, AbstractPlugIn& pi)
		{
			os << "<Plugin " << '"' << pi.name() << '"' << ">";
			return os;
		}
	
	};

	class Plugin : public AbstractPlugIn
    {
    public:
        // prototypes for function pointers used with PlugIns
        typedef void (* plugInCallBackFunction) (AbstractPlugIn& clientObject);
        typedef void (* voidCallBackFunction) (void);
        typedef void (* timestepCallBackFunction) (const float currentTime,
                                                   const float elapsedTime);

        // constructor
        Plugin (bool bAddToRegistry = true);

        // destructor
        virtual ~Plugin();

        // default reset method is to do a close then an open
        void reset (void) {close (); open ();}

        // default sort key (after the "built ins")
        float selectionOrderSortKey (void) const {return 1.0f;}

        // default is to NOT request to be initially selected
        bool requestInitialSelection (void) const {return false;}

        // default function key handler: ignore all
        // (parameter names commented out to prevent compiler warning from "-W")
        void handleFunctionKeys (int /*keyNumber*/) {}

        // default "mini help": print nothing
        void printMiniHelpForFunctionKeys (void) const {}

        // returns pointer to the next Plugin in "selection order"
        AbstractPlugIn* next (void) const;

        // format instance to characters for printing to stream
        friend std::ostream& operator<< (std::ostream& os, Plugin& pi)
        {
            os << "<Plugin " << '"' << pi.name() << '"' << ">";
            return os;
        }

        // CLASS FUNCTIONS

        // search the class registry for a Plugin with the given name
        static AbstractPlugIn* findByName (const char* string);

        // apply a given function to all PlugIns in the class registry
        static void applyToAll (plugInCallBackFunction f);

        // sort Plugin registry by "selection order"
        static void sortBySelectionOrder (void);

        // returns pointer to default Plugin (currently, first in registry)
        static AbstractPlugIn* findDefault (void);

		// save this instance in the class's registry of instances
		static void addToRegistry (AbstractPlugIn*);
		static AbstractPlugIn* findNextPlugin( const AbstractPlugIn* pkThis );
    private:

        // This array stores a list of all PlugIns.  It is manipulated by the
        // constructor and destructor, and used in findByName and applyToAll.
        static const int totalSizeOfRegistry;
        static int itemsInRegistry;
        static AbstractPlugIn* registry[];
    };

} // namespace OpenSteer    
    

// ----------------------------------------------------------------------------
#endif // OPENSTEER_PLUGIN_H
