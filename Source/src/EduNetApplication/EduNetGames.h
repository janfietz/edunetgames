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
// GameDemo
//
// This class encapsulates the state of the GameDemo application and
// the services it provides to its plug-ins
//
// 10-04-04 bk:  put everything into the OpenSteer namespace
// 11-14-02 cwr: recast App class as GameDemo
// 06-26-02 cwr: App class created
//
//
//-----------------------------------------------------------------------------


#ifndef OPENSTEER_OPENSTEERDEMO_H
#define OPENSTEER_OPENSTEERDEMO_H

#include "EduNetCommon/EduNetCommon.h"

namespace OpenSteer
{

	class Color;
	class Vec3;
}

namespace EduNet
{

//
//
//class GameDemo
//{
//public:
//	// ------------------------------------------------------ component objects
//
//	// clock keeps track of both "real time" and "simulation time"
//	static Clock& clock;
//
//	// ------------------------------------------ addresses of selected objects
//
//	// -------------------------------------------- initialize, update and exit
//
//	// initialize GameDemo
//	//     XXX  if I switch from "totally static" to "singleton"
//	//     XXX  class structure this becomes the constructor
//	static void initialize ( void );
//
//	// main update function: step simulation forward and redraw scene
//	static void updateSimulationAndRedraw ( void );
//
//	// exit GameDemo with a given text message or error code
//	static void errorExit ( const char* message );
//	static void exit ( int exitCode );
//
//	// ------------------------------------------------------- Plugin interface
//
//	// select the default Plugin
//	static void selectDefaultPlugin ( void );
//
//
//	// do a simulation update for the currently selected plug-in
//	static void updateSelectedPlugin ( const float currentTime,
//		const float elapsedTime );
//
//	// redraw graphics for the currently selected plug-in
//	static void redrawSelectedPlugin ( const float currentTime,
//		const float elapsedTime );
//
//
//
//	// ---------------------------------------------------- GameDemo phase
//
//	static bool phaseIsDraw ( void )
//	{
//		return phase == drawPhase;
//	}
//	static bool phaseIsUpdate ( void )
//	{
//		return phase == updatePhase;
//	}
//	static bool phaseIsOverhead ( void )
//	{
//		return phase == overheadPhase;
//	}
//
//	static float phaseTimerDraw ( void )
//	{
//		return phaseTimers[drawPhase];
//	}
//	static float phaseTimerUpdate ( void )
//	{
//		return phaseTimers[updatePhase];
//	}
//	// XXX get around shortcomings in current implementation, see note
//	// XXX in updateSimulationAndRedraw
//	//static float phaseTimerOverhead(void){return phaseTimers[overheadPhase];}
//	static float phaseTimerOverhead ( void )
//	{
//		return ( clock.getElapsedRealTime() -
//			( phaseTimerDraw() + phaseTimerUpdate() ) );
//	}
//
//	// ------------------------------------------------------ delayed reset XXX
//
//	// XXX to be reconsidered
//	static void queueDelayedResetPluginXXX ( void );
//	static void doDelayedResetPluginXXX ( void );
//
//	// ------------------------------------------------------ vehicle selection
//
//	// Find the AbstractVehicle whose screen position is nearest the
//	// current the mouse position.  Returns NULL if mouse is outside
//	// this window or if there are no AbstractVehicles.
//	static AbstractVehicle* vehicleNearestToMouse ( void );
//
//	// for storing most recent mouse state
//	static int mouseX;
//	static int mouseY;
//	static bool mouseInWindow;
//
//	// ------------------------------------------------ graphics and annotation
//
//
//	//-----------------------------------------------------------------------------
//	// run graphics event loop
//	static void runGraphics ( void );
//
//	//-----------------------------------------------------------------------------
//	// do all initialization related to graphics
//	static void initializeGraphics ( int argc, char **argv );
//
//	// do all initialization related to graphics
//	static void initializeGraphics ( void );
//	static void initGluiGraphics( int windowID );
//
//	// ----------------------------------------------------------- console text
//	// print list of known commands
//	static void keyboardMiniHelp ( void );
//
//	// ---------------------------------------------------------------- private
//	static void initPhaseTimers ( void );
//private:
//	static int phase;
//	static int phaseStack[];
//	static int phaseStackIndex;
//	static float phaseTimers[];
//	static float phaseTimerBase;
//	static const int phaseStackSize;
//	static void pushPhase ( const int newPhase );
//	static void popPhase ( void );
//	static void updatePhaseTimers ( void );
//
//	static void setGlutFunctions( void );
//	// XXX apparently MS VC6 cannot handle initialized static const members,
//	// XXX so they have to be initialized not-inline.
//	// static const int drawPhase = 2;
//	// static const int updatePhase = 1;
//	// static const int overheadPhase = 0;
//	static const int drawPhase;
//	static const int updatePhase;
//	static const int overheadPhase;
//};
//
//
//
////-----------------------------------------------------------------------------
//// accessors for GLUT's window dimensions
//float extern_drawGetWindowHeight ( void );
//float extern_drawGetWindowWidth ( void );
//
} // namespace OpenSteer


//-----------------------------------------------------------------------------




//-----------------------------------------------------------------------------
#endif // OPENSTEER_OPENSTEERDEMO_H
