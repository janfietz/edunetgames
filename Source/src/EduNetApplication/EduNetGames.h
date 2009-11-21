

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
// OpenSteerDemo
//
// This class encapsulates the state of the OpenSteerDemo application and
// the services it provides to its plug-ins
//
// 10-04-04 bk:  put everything into the OpenSteer namespace
// 11-14-02 cwr: recast App class as OpenSteerDemo 
// 06-26-02 cwr: App class created 
//
//
//-----------------------------------------------------------------------------


#ifndef OPENSTEER_OPENSTEERDEMO_H
#define OPENSTEER_OPENSTEERDEMO_H

#include "EduNetCommon/EduNetCommon.h"
#include "EduNetCommon/EduNetOptions.h"
#include "EduNetCommon/EduNetLog.h"

#include "OpenSteer/Clock.h"
#include "OpenSteer/Plugin.h"
#include "OpenSteer/Camera.h"
#include "OpenSteer/Utilities.h"


namespace OpenSteer {

    class Color;
    class Vec3;
    

    class OpenSteerDemo
    {
    public:
		// options
		static EduNetOptions options;

        // ------------------------------------------------------ component objects

        // clock keeps track of both "real time" and "simulation time"
        static Clock& clock;

        // ------------------------------------------ addresses of selected objects



        // -------------------------------------------- initialize, update and exit

        // initialize OpenSteerDemo
        //     XXX  if I switch from "totally static" to "singleton"
        //     XXX  class structure this becomes the constructor
        static void initialize (void);

        // main update function: step simulation forward and redraw scene
        static void updateSimulationAndRedraw (void);

        // exit OpenSteerDemo with a given text message or error code
        static void errorExit (const char* message);
        static void exit (int exitCode);

        // ------------------------------------------------------- Plugin interface

        // select the default Plugin
        static void selectDefaultPlugin (void);
        
        // select the "next" plug-in, cycling through "plug-in selection order"
        static void selectNextPlugin (void);

		// select the plug-in by index
		static void selectPluginByIndex (size_t idx);

		static void selectPlugin( AbstractPlugin* pkPlugin );

        // handle function keys an a per-plug-in basis
        static void functionKeyForPlugin (int keyNumber);

        // return name of currently selected plug-in
        static const char* nameOfSelectedPlugin (void);

        // open the currently selected plug-in
        static void openSelectedPlugin (void);

        // do a simulation update for the currently selected plug-in
        static void updateSelectedPlugin (const float currentTime,
                                          const float elapsedTime);

        // redraw graphics for the currently selected plug-in
        static void redrawSelectedPlugin (const float currentTime,
                                          const float elapsedTime);

        // close the currently selected plug-in
        static void closeSelectedPlugin (void);

        // reset the currently selected plug-in
        static void resetSelectedPlugin (void);

        static const AVGroup& allVehiclesOfSelectedPlugin(void);

        // ---------------------------------------------------- OpenSteerDemo phase

        static bool phaseIsDraw     (void) {return phase == drawPhase;}
        static bool phaseIsUpdate   (void) {return phase == updatePhase;}
        static bool phaseIsOverhead (void) {return phase == overheadPhase;}

        static float phaseTimerDraw     (void) {return phaseTimers[drawPhase];}
        static float phaseTimerUpdate   (void) {return phaseTimers[updatePhase];}
        // XXX get around shortcomings in current implementation, see note
        // XXX in updateSimulationAndRedraw
        //static float phaseTimerOverhead(void){return phaseTimers[overheadPhase];}
        static float phaseTimerOverhead (void)
        {
            return (clock.getElapsedRealTime() -
                    (phaseTimerDraw() + phaseTimerUpdate()));
        }

        // ------------------------------------------------------ delayed reset XXX

        // XXX to be reconsidered
        static void queueDelayedResetPluginXXX (void);
        static void doDelayedResetPluginXXX (void);

        // ------------------------------------------------------ vehicle selection

        // select the "next" vehicle: cycle through the registry
        static void selectNextVehicle (void);

        // select vehicle nearest the given screen position (e.g.: of the mouse)
        static void selectVehicleNearestScreenPosition (int x, int y);

        // ---------------------------------------------------------- mouse support

        // Find the AbstractVehicle whose screen position is nearest the
        // current the mouse position.  Returns NULL if mouse is outside
        // this window or if there are no AbstractVehicles.
        static AbstractVehicle* vehicleNearestToMouse (void);

        // Find the AbstractVehicle whose screen position is nearest the
        // given window coordinates, typically the mouse position.  Note
        // this will return NULL if there are no AbstractVehicles.
        static AbstractVehicle* findVehicleNearestScreenPosition (int x, int y);

        // for storing most recent mouse state
        static int mouseX;
        static int mouseY;
        static bool mouseInWindow;

        // ------------------------------------------------ graphics and annotation

        // do all initialization related to graphics
        static void initializeGraphics (void);

        // ground plane grid-drawing utility used by several plug-ins
        static void gridUtility (const Vec3& gridTarget);

        // draws a gray disk on the XZ plane under a given vehicle
        static void highlightVehicleUtility (const AbstractVehicle& vehicle);

        // draws a gray circle on the XZ plane under a given vehicle
        static void circleHighlightVehicleUtility (const AbstractVehicle& vehicle);

        // draw a box around a vehicle aligned with its local space
        // xxx not used as of 11-20-02
        static void drawBoxHighlightOnVehicle (const AbstractVehicle& v,
                                               const Color& color);

        // draws a colored circle (perpendicular to view axis) around the center
        // of a given vehicle.  The circle's radius is the vehicle's radius times
        // radiusMultiplier.
        static void drawCircleHighlightOnVehicle (const AbstractVehicle& v,
                                                  const float radiusMultiplier,
                                                  const Color& color);

        // ----------------------------------------------------------- console text


        // print list of known commands
        static void keyboardMiniHelp (void);

        // ---------------------------------------------------------------- private

		static void initPhaseTimers (void);
    private:
        static int phase;
        static int phaseStack[];
        static int phaseStackIndex;
        static float phaseTimers[];
        static float phaseTimerBase;
        static const int phaseStackSize;
        static void pushPhase (const int newPhase);
        static void popPhase (void);
        static void updatePhaseTimers (void);



        // XXX apparently MS VC6 cannot handle initialized static const members,
        // XXX so they have to be initialized not-inline.
        // static const int drawPhase = 2;
        // static const int updatePhase = 1;
        // static const int overheadPhase = 0;
        static const int drawPhase;
        static const int updatePhase;
        static const int overheadPhase;
    };

    //-----------------------------------------------------------------------------
    // do all initialization related to graphics


    void initializeGraphics (int argc, char **argv);


    //-----------------------------------------------------------------------------
    // run graphics event loop


    void runGraphics (void);


    //-----------------------------------------------------------------------------
    // accessors for GLUT's window dimensions


    float drawGetWindowHeight (void);
    float drawGetWindowWidth (void);

} // namespace OpenSteer
    
    
//-----------------------------------------------------------------------------


#include "OpenSteer/Draw.h"


//-----------------------------------------------------------------------------
#endif // OPENSTEER_OPENSTEERDEMO_H
