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
// This class encapsulates the state of the OpenSteerDemo application and the
// services it provides to its plug-ins.  It is never instantiated, all of
// its members are static (belong to the class as a whole.)
//
// 10-04-04 bk:  put everything into the OpenSteer namespace
// 11-14-02 cwr: created
//
//
//-----------------------------------------------------------------------------

// 10-30-09 cp/jf: modified for educational purpose
#include "EduNetGames.h"
#include "EduNetApplication.h"
#include "EduNetCommon/EduNetDraw.h"
#include "EduNetCommon/EduNetOptions.h"
#include "OpenSteerUT/OpenSteerUT.h"



//-----------------------------------------------------------------------------
namespace
{
int framePeriod = 100;//todo: test if this value should be 0

const char* appVersionName = EDUNET_APPNAME;

// The number of our GLUT window
int windowID = 0;

bool gMouseAdjustingCameraAngle = false;
bool gMouseAdjustingCameraRadius = false;
int gMouseAdjustingCameraLastX;
int gMouseAdjustingCameraLastY;

//-------------------------------------------------------------------------
// XXX this is used by CaptureTheFlag
// XXX it was moved here from main.cpp on 12-4-02
// XXX I'm not sure if this is a useful feature or a bogus hack
// XXX needs to be reconsidered.


bool gDelayedResetPluginXXX = false;

void printPlugin ( OpenSteer::AbstractPlugin& pi )
{
    std::ostringstream message;
    message << " " << pi;
    EduNet::Log::printLine ( message );
}
} // anonymous namespace


//-----------------------------------------------------------------------------
// keeps track of both "real time" and "simulation time"
OpenSteer::Clock& OpenSteer::OpenSteerDemo::clock = OpenSteer::Clock::processClock();

//-----------------------------------------------------------------------------
// phase: identifies current phase of the per-frame update cycle
int OpenSteer::OpenSteerDemo::phase = OpenSteer::OpenSteerDemo::overheadPhase;


//-----------------------------------------------------------------------------
// XXX apparently MS VC6 cannot handle initialized static const members,
// XXX so they have to be initialized not-inline.
const int OpenSteer::OpenSteerDemo::overheadPhase = 0;
const int OpenSteer::OpenSteerDemo::updatePhase = 1;
const int OpenSteer::OpenSteerDemo::drawPhase = 2;

//-----------------------------------------------------------------------------
// manage OpenSteerDemo phase transitions (xxx and maintain phase timers)
int OpenSteer::OpenSteerDemo::phaseStackIndex = 0;
const int OpenSteer::OpenSteerDemo::phaseStackSize = 5;
int OpenSteer::OpenSteerDemo::phaseStack [OpenSteer::OpenSteerDemo::phaseStackSize];


//-----------------------------------------------------------------------------
// initialize OpenSteerDemo application

//-----------------------------------------------------------------------------
// Find the AbstractVehicle whose screen position is nearest the current the
// mouse position.  Returns NULL if mouse is outside this window or if
// there are no AbstractVehicle.
OpenSteer::AbstractVehicle*
OpenSteer::OpenSteerDemo::vehicleNearestToMouse ( void )
{
    if ( mouseInWindow )
    {
        return VehicleUtilities::findVehicleNearestScreenPosition ( mouseX, mouseY );
    }
    else
    {
        return NULL;
    }
}

//-----------------------------------------------------------------------------
void
OpenSteer::OpenSteerDemo::initialize ( void )
{
    // select and initialize the default Plugin
    OpenSteer::OpenSteerDemo::selectDefaultPlugin ();

    {
        // XXX this block is for debugging purposes,
        // XXX should it be replaced with something permanent?
        std::ostringstream message0;
        message0 << "Known plugins:" << std::ends;
        EduNet::Log::printLine ( message0 );

        Plugin::applyToAll ( printPlugin );

        // identify default Plugin
        if ( NULL == OpenSteer::Plugin::selectedPlugin )
		{
            errorExit ( "no default Plugin" );
		}

        std::ostringstream message;
        message << std::endl << "Default plugin:" << std::endl;
        message << " " << *OpenSteer::Plugin::selectedPlugin << std::endl;
        message << std::endl;
        EduNet::Log::printLine ( message );
    }
}

//-----------------------------------------------------------------------------
// main update function: step simulation forward and redraw scene
void
OpenSteer::OpenSteerDemo::updateSimulationAndRedraw ( void )
{
    // update global simulation clock
    clock.update ();

    //  start the phase timer (XXX to accurately measure "overhead" time this
    //  should be in displayFunc, or somehow account for time outside this
    //  routine)
    initPhaseTimers ();

    // run selected Plugin (with simulation's current time and step size)
    updateSelectedPlugin ( clock.getTotalSimulationTime (),
                           clock.getElapsedSimulationTime () );

    // redraw selected Plugin (based on real time)
    redrawSelectedPlugin ( clock.getTotalRealTime (),
                           clock.getElapsedRealTime () );
}

//-----------------------------------------------------------------------------
// exit OpenSteerDemo with a given text message or error code
void
OpenSteer::OpenSteerDemo::errorExit ( const char* message )
{
    EduNet::Log::printMessage ( message );
	EduNet::Application::_SDMCleanup();
	EduNet::Application::_SDMShutdown();
#ifdef _MSC_VER
    MessageBox ( 0, message, "OpenSteerDemo Unfortunate Event", MB_ICONERROR );
#endif
    exit ( -1 );
}

//-----------------------------------------------------------------------------
void
OpenSteer::OpenSteerDemo::exit ( int exitCode )
{
    OpenSteer::Plugin::selectPlugin ( NULL );
	EduNet::Application::_SDMCleanup();
	EduNet::Application::_SDMShutdown();
    ::exit ( exitCode );
}

//-----------------------------------------------------------------------------
// select the default Plugin
void
OpenSteer::OpenSteerDemo::selectDefaultPlugin ( void )
{
    const char* pszPluginName = EduNetOptions::accessOptions().getSelectedPlugin();
    OpenSteer::AbstractPlugin* pkPlugin = Plugin::findByName ( pszPluginName );
    if ( NULL == pkPlugin )
    {
        pkPlugin = Plugin::findDefault();
    }
    OpenSteer::Plugin::selectPlugin ( pkPlugin );
}

//-----------------------------------------------------------------------------
// do a simulation update for the currently selected plug-in
void
OpenSteer::OpenSteerDemo::updateSelectedPlugin ( const float currentTime,
        const float elapsedTime )
{
    // switch to Update phase
    pushPhase ( updatePhase );

    // service queued reset request, if any
    doDelayedResetPluginXXX();

    // update the local player object
    // note: we simply use the custom update mechanism to pass
    //       values from the local to the in game controller
    //       the same mechanism might be used for a networking
    //       client controller
#if 1 // ET_HAVE_LOCALPLAYER_CONTROL
    if ( EduNet::Application::AccessApplication().allowLocalPlayer() )
    {
        AbstractPlayer* pkPlayer = SimplePlayer::accessLocalPlayer();
        AbstractController* pkController = pkPlayer->accessController();
        if ( NULL != pkController )
        {
            pkController->setCustomUpdated ( LocalPlayerController::accessLocalPlayerController() );
        }
        pkPlayer->update ( currentTime, elapsedTime );
        AbstractEntity* pkControlledEntity = pkPlayer->getControlledEntity();
        AbstractEntity* pkNewControlledEntity = pkControlledEntity;
        if ( SimpleVehicle::getSelectedVehicle() != pkNewControlledEntity )
        {
            pkNewControlledEntity = SimpleVehicle::getSelectedVehicle();
        }
        if ( pkNewControlledEntity != pkControlledEntity )
        {
            if ( NULL != pkNewControlledEntity )
            {
                // right now only authorities
                if ( pkNewControlledEntity->isRemoteObject() )
                {
                    pkNewControlledEntity = NULL;
                }
            }
            pkPlayer->play ( pkNewControlledEntity );
        }
    }
#endif

    // invoke selected Plugin's Update method
    EduNet::Application::AccessApplication().updateSelectedPlugin ( currentTime, elapsedTime );

    // return to previous phase
    popPhase();
}

//-----------------------------------------------------------------------------
// redraw graphics for the currently selected plug-in
void
OpenSteer::OpenSteerDemo::redrawSelectedPlugin ( const float currentTime,
        const float elapsedTime )
{
    // switch to Draw phase
    pushPhase ( drawPhase );

    // nearest mouse (to be highlighted)
    SimpleVehicle::setNearestMouseVehicle( OpenSteerDemo::vehicleNearestToMouse() );

    // invoke selected Plugin's Draw method
    EduNet::Application::AccessApplication().redrawSelectedPlugin ( currentTime, elapsedTime );

    // draw any annotation queued up during selected Plugin's Update method
    drawAllDeferredLines ();
    drawAllDeferredCirclesOrDisks ();

    // return to previous phase
    popPhase ();
}

//-----------------------------------------------------------------------------
void
OpenSteer::OpenSteerDemo::queueDelayedResetPluginXXX ( void )
{
    gDelayedResetPluginXXX = true;
}

//-----------------------------------------------------------------------------
void
OpenSteer::OpenSteerDemo::doDelayedResetPluginXXX ( void )
{
    if ( gDelayedResetPluginXXX )
    {
        OpenSteer::Plugin::resetSelectedPlugin ();
        gDelayedResetPluginXXX = false;
    }
}

//-----------------------------------------------------------------------------
// for storing most recent mouse state
int OpenSteer::OpenSteerDemo::mouseX = 0;
int OpenSteer::OpenSteerDemo::mouseY = 0;
bool OpenSteer::OpenSteerDemo::mouseInWindow = false;

// ------------------------------------------------------------------------
// print list of known commands
//
// XXX this list should be assembled automatically,
// XXX perhaps from a list of "command" objects created at initialization
void
OpenSteer::OpenSteerDemo::keyboardMiniHelp ( void )
{
    EduNet::Log::printMessage ( "" );
    EduNet::Log::printMessage ( "defined single key commands:" );
    EduNet::Log::printMessage ( "  r      restart current Plugin." );
    EduNet::Log::printMessage ( "  n      select next vehicle." );
    EduNet::Log::printMessage ( "  c      select next camera mode." );
    EduNet::Log::printMessage ( "  f      select next preset frame rate" );
    EduNet::Log::printMessage ( "  Tab    select next Plugin." );
    EduNet::Log::printMessage ( "  o      toggle annotation on/off." );
    EduNet::Log::printMessage ( "  Space  toggle between Run and Pause." );
    EduNet::Log::printMessage ( "  ->     step forward one frame." );
    EduNet::Log::printMessage ( "  Esc    exit." );
    EduNet::Log::printMessage ( "" );

    // allow Plugin to print mini help for the function keys it handles
    OpenSteer::Plugin::selectedPlugin->printMiniHelpForFunctionKeys ();
}

void
OpenSteer::OpenSteerDemo::pushPhase ( const int newPhase )
{
    g_openSteerUTData.updatePhaseActive = updatePhaseActive = newPhase == OpenSteer::OpenSteerDemo::updatePhase;
    g_openSteerUTData.drawPhaseActive = drawPhaseActive = newPhase == OpenSteer::OpenSteerDemo::drawPhase;

    // update timer for current (old) phase: add in time since last switch
    updatePhaseTimers ();

    // save old phase
    phaseStack[phaseStackIndex++] = phase;

    // set new phase
    phase = newPhase;

    // check for stack overflow
    if ( phaseStackIndex >= phaseStackSize ) errorExit ( "phaseStack overflow" );
}

//-----------------------------------------------------------------------------
void
OpenSteer::OpenSteerDemo::popPhase ( void )
{
    // update timer for current (old) phase: add in time since last switch
    updatePhaseTimers ();

    // restore old phase
    phase = phaseStack[--phaseStackIndex];
    g_openSteerUTData.updatePhaseActive = updatePhaseActive = phase == OpenSteer::OpenSteerDemo::updatePhase;
    g_openSteerUTData.drawPhaseActive = drawPhaseActive = phase == OpenSteer::OpenSteerDemo::drawPhase;
}

//-----------------------------------------------------------------------------
float OpenSteer::OpenSteerDemo::phaseTimerBase = 0;
float OpenSteer::OpenSteerDemo::phaseTimers [drawPhase+1];

//-----------------------------------------------------------------------------
void
OpenSteer::OpenSteerDemo::initPhaseTimers ( void )
{
    phaseTimers[drawPhase] = 0;
    phaseTimers[updatePhase] = 0;
    phaseTimers[overheadPhase] = 0;
    phaseTimerBase = clock.getTotalRealTime ();
}

//-----------------------------------------------------------------------------
void
OpenSteer::OpenSteerDemo::updatePhaseTimers ( void )
{
    const float currentRealTime = clock.realTimeSinceFirstClockUpdate();
    phaseTimers[phase] += currentRealTime - phaseTimerBase;
    phaseTimerBase = currentRealTime;
}


//-----------------------------------------------------------------------------
namespace
{


//-------------------------------------------------------------------------
// initialize GL mode settings


void
initGL ( void )
{
    // background = dark gray
    // @todo bknafla Changed the background color to make some screenshots.
    glClearColor ( 0.3f, 0.3f, 0.3f, 0 );
    // glClearColor( 1.0f, 1.0f, 1.0f, 0.0f );

    // enable depth buffer clears
    glClearDepth ( 1.0f );

    // select smooth shading
    glShadeModel ( GL_SMOOTH );

    // enable  and select depth test
    glDepthFunc ( GL_LESS );
    glEnable ( GL_DEPTH_TEST );

    // turn on backface culling
    glEnable ( GL_CULL_FACE );
    glCullFace ( GL_BACK );

    // enable blending and set typical "blend into frame buffer" mode
    glEnable ( GL_BLEND );
    glBlendFunc ( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

    // reset projection matrix
    glMatrixMode ( GL_PROJECTION );
    glLoadIdentity();
}


//-------------------------------------------------------------------------
// handler for window resizing
void
reshapeFunc ( int width, int height )
{
	ViewPort& viewPort = g_openSteerUTData.viewPort;
    GLUI_Master.get_viewport_area ( &viewPort.tx, &viewPort.ty, &viewPort.tw, &viewPort.th );
    glViewport ( viewPort.tx, viewPort.ty, viewPort.tw, viewPort.th );

    // set perspective transformation
    glMatrixMode ( GL_PROJECTION );
    glLoadIdentity ();
    const GLfloat w = viewPort.tw;
    const GLfloat h = viewPort.th;
    const GLfloat aspectRatio = ( height == 0 ) ? 1 : w/h;
    const GLfloat fieldOfViewY = 45;
    const GLfloat hither = 1;  // put this on Camera so Plugins can frob it
    const GLfloat yon = 400;   // put this on Camera so Plugins can frob it
    gluPerspective ( fieldOfViewY, aspectRatio, hither, yon );

    // leave in modelview mode
    glMatrixMode ( GL_MODELVIEW );
}

//-------------------------------------------------------------------------
// This is called (by GLUT) each time a mouse button pressed or released.
void
mouseButtonFunc ( int button, int state, int x, int y )
{
    // if the mouse button has just been released
    if ( state == GLUT_UP )
    {
        // end any ongoing mouse-adjusting-camera session
        gMouseAdjustingCameraAngle = false;
        gMouseAdjustingCameraRadius = false;
    }

    // if the mouse button has just been pushed down
    if ( state == GLUT_DOWN )
    {
        // names for relevant values of "button" and "state"
        const int  mods       = glutGetModifiers ();
        const bool modNone    = ( mods == 0 );
        const bool modCtrl    = ( mods == GLUT_ACTIVE_CTRL );
        const bool modAlt     = ( mods == GLUT_ACTIVE_ALT );
        const bool modCtrlAlt = ( mods == ( GLUT_ACTIVE_CTRL | GLUT_ACTIVE_ALT ) );
        const bool mouseL     = ( button == GLUT_LEFT_BUTTON );
        const bool mouseM     = ( button == GLUT_MIDDLE_BUTTON );
        const bool mouseR     = ( button == GLUT_RIGHT_BUTTON );

#if __APPLE__ && __MACH__
        const bool macosx = true;
#else
        const bool macosx = false;
#endif

        // mouse-left (with no modifiers): select vehicle
        if ( modNone && mouseL )
        {
            OpenSteer::VehicleUtilities::selectVehicleNearestScreenPosition ( x, y );
        }

        // control-mouse-left: begin adjusting camera angle (on Mac OS X
        // control-mouse maps to mouse-right for "context menu", this makes
        // OpenSteerDemo's control-mouse work work the same on OS X as on Linux
        // and Windows, but it precludes using a mouseR context menu)
        if ( ( modCtrl && mouseL ) ||
             ( modNone && mouseR && macosx ) )
        {
            gMouseAdjustingCameraLastX = x;
            gMouseAdjustingCameraLastY = y;
            gMouseAdjustingCameraAngle = true;
        }

        // control-mouse-middle: begin adjusting camera radius
        // (same for: control-alt-mouse-left and control-alt-mouse-middle,
        // and on Mac OS X it is alt-mouse-right)
        if ( ( modCtrl    && mouseM ) ||
             ( modCtrlAlt && mouseL ) ||
             ( modCtrlAlt && mouseM ) ||
             ( modAlt     && mouseR && macosx ) )
        {
            gMouseAdjustingCameraLastX = x;
            gMouseAdjustingCameraLastY = y;
            gMouseAdjustingCameraRadius = true;
        }
    }
}

//-------------------------------------------------------------------------
// called when mouse moves and any buttons are down
void
mouseMotionFunc ( int x, int y )
{
    // are we currently in the process of mouse-adjusting the camera?
    if ( gMouseAdjustingCameraAngle || gMouseAdjustingCameraRadius )
    {
        // speed factors to map from mouse movement in pixels to 3d motion
        const float dSpeed = 0.005f;
        const float rSpeed = 0.01f;

        // XY distance (in pixels) that mouse moved since last update
        const float dx = x - gMouseAdjustingCameraLastX;
        const float dy = y - gMouseAdjustingCameraLastY;
        gMouseAdjustingCameraLastX = x;
        gMouseAdjustingCameraLastY = y;

        OpenSteer::Vec3 cameraAdjustment;

        // set XY values according to mouse motion on screen space
        if ( gMouseAdjustingCameraAngle )
        {
            cameraAdjustment.x = dx * -dSpeed;
            cameraAdjustment.y = dy * +dSpeed;
        }

        // set Z value according vertical to mouse motion
        if ( gMouseAdjustingCameraRadius )
        {
            cameraAdjustment.z = dy * rSpeed;
        }

        // pass adjustment vector to camera's mouse adjustment routine
        OpenSteer::Camera::camera.mouseAdjustOffset ( cameraAdjustment );
    }
}

//-------------------------------------------------------------------------
// called when mouse moves and no buttons are down
void
mousePassiveMotionFunc ( int x, int y )
{
    OpenSteer::OpenSteerDemo::mouseX = x;
    OpenSteer::OpenSteerDemo::mouseY = y;
}

//-------------------------------------------------------------------------
// called when mouse enters or exits the window
void
mouseEnterExitWindowFunc ( int state )
{
    if ( state == GLUT_ENTERED )
        OpenSteer::OpenSteerDemo::mouseInWindow = true;
    if ( state == GLUT_LEFT )
        OpenSteer::OpenSteerDemo::mouseInWindow = false;
}

// This is used to control the frame rate (60Hz).
void timerFunc ( int )
{
//              glutSetWindow(windowID);
//              glutPostRedisplay();
    glutTimerFunc ( framePeriod, timerFunc, 0 );
}

//-------------------------------------------------------------------------
// draw PlugI name in upper lefthand corner of screen
void
drawDisplayPluginName ( void )
{
    const float h = glutGet ( GLUT_WINDOW_HEIGHT );
    const OpenSteer::Vec3 screenLocation ( 10, h-20, 0 );
    draw2dTextAt2dLocation ( OpenSteer::Plugin::nameOfSelectedPlugin (),
                             screenLocation,
                             OpenSteer::gWhite, OpenSteer::drawGetWindowWidth(), OpenSteer::drawGetWindowHeight() );
}

//-------------------------------------------------------------------------
// draw camera mode name in lower lefthand corner of screen
void
drawDisplayCameraModeName ( void )
{
    const OpenSteer::Vec3& camPosition = OpenSteer::Camera::camera.position();
    std::ostringstream message;
    message << std::setprecision ( 2 );
    message << std::setiosflags ( std::ios::fixed );

    message << "Camera: "
    << OpenSteer::Camera::camera.modeName ()
    << " ( " << camPosition.x
    << ", " << camPosition.y
    << ", " << camPosition.z
    << " ) fixedDistDistance( " << OpenSteer::Camera::camera.fixedDistDistance << " )" << std::ends;
    const OpenSteer::Vec3 screenLocation ( 10, 10, 0 );
    OpenSteer::draw2dTextAt2dLocation ( message, screenLocation, OpenSteer::gWhite, OpenSteer::drawGetWindowWidth(), OpenSteer::drawGetWindowHeight() );
}

//-------------------------------------------------------------------------
// helper for drawDisplayFPS
void
writePhaseTimerReportToStream ( float phaseTimer,
                                std::ostringstream& stream )
{
    // write the timer value in seconds in floating point
    stream << std::setprecision ( 5 ) << std::setiosflags ( std::ios::fixed );
    stream << phaseTimer;

    // restate value in another form
    stream << std::setprecision ( 0 ) << std::setiosflags ( std::ios::fixed );
    stream << " (";

    // different notation for variable and fixed frame rate
    if ( OpenSteer::OpenSteerDemo::clock.getVariableFrameRateMode() )
    {
        // express as FPS (inverse of phase time)
        stream << 1 / phaseTimer;
        stream << " fps)\n";
    }
    else
    {
        // quantify time as a percentage of frame time
        const int fps = OpenSteer::OpenSteerDemo::clock.getFixedFrameRate ();
        stream << ( ( 100 * phaseTimer ) / ( 1.0f / fps ) );
        stream << "% of 1/";
        stream << fps;
        stream << "sec)\n";
    }
}

//-------------------------------------------------------------------------
// draw text showing (smoothed, rounded) "frames per second" rate
// (and later a bunch of related stuff was dumped here, a reorg would be nice)
//
// XXX note: drawDisplayFPS has morphed considerably and should be called
// something like displayClockStatus, and that it should be part of
// OpenSteerDemo instead of Draw  (cwr 11-23-04)

float gSmoothedTimerDraw = 0;
float gSmoothedTimerUpdate = 0;
float gSmoothedTimerOverhead = 0;

void
drawDisplayFPS ( void )
{
    // skip several frames to allow frame rate to settle
    static int skipCount = 10;
    if ( skipCount > 0 )
    {
        skipCount--;
    }
    else
    {
        // keep track of font metrics and start of next line
        const int lh = 16; // xxx line height
        const int cw = 9; // xxx character width
        OpenSteer::Vec3 screenLocation ( 10, 10, 0 );

        // target and recent average frame rates
        const int targetFPS = OpenSteer::OpenSteerDemo::clock.getFixedFrameRate ();
        const float smoothedFPS = OpenSteer::OpenSteerDemo::clock.getSmoothedFPS ();

        // describe clock mode and frame rate statistics
        screenLocation.y += lh;
        std::ostringstream clockStr;
        clockStr << "Clock: ";
        if ( OpenSteer::OpenSteerDemo::clock.getAnimationMode () )
        {
            clockStr << "animation mode (";
            clockStr << targetFPS << " fps,";
            clockStr << " display "<< OpenSteer::round ( smoothedFPS ) << " fps, ";
            const float ratio = smoothedFPS / targetFPS;
            clockStr << ( int ) ( 100 * ratio ) << "% of nominal speed)";
        }
        else
        {
            clockStr << "real-time mode, ";
            if ( OpenSteer::OpenSteerDemo::clock.getVariableFrameRateMode () )
            {
                clockStr << "variable frame rate (";
                clockStr << OpenSteer::round ( smoothedFPS ) << " fps)";
            }
            else
            {
                clockStr << "fixed frame rate (target: " << targetFPS;
                clockStr << " actual: " << OpenSteer::round ( smoothedFPS ) << ", ";

                OpenSteer::Vec3 sp;
                sp = screenLocation;
                sp.x += cw * ( int ) clockStr.tellp ();

                // create usage description character string
                std::ostringstream xxxStr;
                xxxStr << std::setprecision ( 0 )
                << std::setiosflags ( std::ios::fixed )
                << "usage: " << OpenSteer::OpenSteerDemo::clock.getSmoothedUsage ()
                << "%"
                << std::ends;

                const int usageLength = ( ( int ) xxxStr.tellp () ) - 1;
                for ( int i = 0; i < usageLength; i++ ) clockStr << " ";
                clockStr << ")";

                // display message in lower left corner of window
                // (draw in red if the instantaneous usage is 100% or more)
                const float usage = OpenSteer::OpenSteerDemo::clock.getUsage ();
                const OpenSteer::Color color = ( usage >= 100 ) ? OpenSteer::gRed : OpenSteer::gWhite;
                draw2dTextAt2dLocation ( xxxStr, sp, color, OpenSteer::drawGetWindowWidth(), OpenSteer::drawGetWindowHeight() );
            }
        }
        if ( OpenSteer::OpenSteerDemo::clock.getPausedState () )
            clockStr << " [paused]";
        clockStr << std::ends;
        draw2dTextAt2dLocation ( clockStr, screenLocation, OpenSteer::gWhite, OpenSteer::drawGetWindowWidth(), OpenSteer::drawGetWindowHeight() );

        // get smoothed phase timer information
        const float ptd = OpenSteer::OpenSteerDemo::phaseTimerDraw();
        const float ptu = OpenSteer::OpenSteerDemo::phaseTimerUpdate();
        const float pto = OpenSteer::OpenSteerDemo::phaseTimerOverhead();
        const float smoothRate = OpenSteer::OpenSteerDemo::clock.getSmoothingRate ();
        OpenSteer::blendIntoAccumulator ( smoothRate, ptd, gSmoothedTimerDraw );
        OpenSteer::blendIntoAccumulator ( smoothRate, ptu, gSmoothedTimerUpdate );
        OpenSteer::blendIntoAccumulator ( smoothRate, pto, gSmoothedTimerOverhead );

        // display phase timer information
        screenLocation.y += lh * 4;
        std::ostringstream timerStr;
        timerStr << "update: ";
        writePhaseTimerReportToStream ( gSmoothedTimerUpdate, timerStr );
        timerStr << "draw:   ";
        writePhaseTimerReportToStream ( gSmoothedTimerDraw, timerStr );
        timerStr << "other:  ";
        writePhaseTimerReportToStream ( gSmoothedTimerOverhead, timerStr );
        timerStr << std::ends;
        draw2dTextAt2dLocation ( timerStr, screenLocation, OpenSteer::gGreen, OpenSteer::drawGetWindowWidth(), OpenSteer::drawGetWindowHeight() );
    }
}

// ------------------------------------------------------------------------
// cycle through frame rate presets  (XXX move this to OpenSteerDemo)
void
selectNextPresetFrameRate ( void )
{
    // note that the cases are listed in reverse order, and that
    // the default is case 0 which causes the index to wrap around
    static int frameRatePresetIndex = 0;
    switch ( ++frameRatePresetIndex )
    {
    case 3:
        // animation mode at 60 fps
        OpenSteer::OpenSteerDemo::clock.setFixedFrameRate ( 60 );
        OpenSteer::OpenSteerDemo::clock.setAnimationMode ( true );
        OpenSteer::OpenSteerDemo::clock.setVariableFrameRateMode ( false );
        break;
    case 2:
        // real-time fixed frame rate mode at 60 fps
        OpenSteer::OpenSteerDemo::clock.setFixedFrameRate ( 60 );
        OpenSteer::OpenSteerDemo::clock.setAnimationMode ( false );
        OpenSteer::OpenSteerDemo::clock.setVariableFrameRateMode ( false );
        break;
    case 1:
        // real-time fixed frame rate mode at 24 fps
        OpenSteer::OpenSteerDemo::clock.setFixedFrameRate ( 24 );
        OpenSteer::OpenSteerDemo::clock.setAnimationMode ( false );
        OpenSteer::OpenSteerDemo::clock.setVariableFrameRateMode ( false );
        break;
    case 0:
    default:
        // real-time variable frame rate mode ("as fast as possible")
        frameRatePresetIndex = 0;
        OpenSteer::OpenSteerDemo::clock.setFixedFrameRate ( 0 );
        OpenSteer::OpenSteerDemo::clock.setAnimationMode ( false );
        OpenSteer::OpenSteerDemo::clock.setVariableFrameRateMode ( true );
        break;
    }
}

// ------------------------------------------------------------------------
// This function is called (by GLUT) each time a key is pressed.
//
// XXX the bulk of this should be moved to OpenSteerDemo
//
// parameter names commented out to prevent compiler warning from "-W"
void keyboardSpecialUpFunc ( int key, int x, int y )
{
}

void
keyboardFuncUp ( unsigned char key, int x, int y )
{
    if ( OpenSteer::LocalPlayerController::keyboardFuncUp ( key, x, y ) )
    {
        return;
    }
}

void
keyboardFunc ( unsigned char key, int x, int y )
{
    if ( OpenSteer::LocalPlayerController::keyboardFunc ( key, x, y ) )
    {
        return;
    }

    std::ostringstream message;

    // ascii codes
    const int tab = 9;
    const int space = 32;
    const int esc = 27; // escape key

    switch ( key )
    {
        // reset selected Plugin
    case 'r':
        OpenSteer::Plugin::resetSelectedPlugin ();
        message << "reset Plugin "
        << '"' << OpenSteer::Plugin::nameOfSelectedPlugin () << '"'
        << std::ends;
        EduNet::Log::printMessage ( message );
        break;

        // cycle selection to next vehicle
    case 'n':
        EduNet::Log::printMessage ( "select next vehicle/agent" );
        OpenSteer::VehicleUtilities::selectNextVehicle ();
        break;

        // camera mode cycle
    case 'c':
        OpenSteer::Camera::camera.selectNextMode ();
        message << "select camera mode "
        << '"' << OpenSteer::Camera::camera.modeName () << '"' << std::ends;
        EduNet::Log::printMessage ( message );
        break;

        // select next Plugin
    case tab:
        OpenSteer::Plugin::selectNextPlugin ();
        message << "select next Plugin: "
        << '"' << OpenSteer::Plugin::nameOfSelectedPlugin () << '"'
        << std::ends;
        EduNet::Log::printMessage ( message );
        break;

        // toggle annotation state
    case 'o':
        EduNet::Log::printMessage ( OpenSteer::toggleAnnotationState () ?
                                    "annotation ON" : "annotation OFF" );
        break;

        // toggle run/pause state
    case space:
        EduNet::Log::printMessage ( OpenSteer::OpenSteerDemo::clock.togglePausedState () ?
                                    "pause" : "run" );
        break;

        // cycle through frame rate (clock mode) presets
    case 'f':
        selectNextPresetFrameRate ();
        message << "set clock to ";
        if ( OpenSteer::OpenSteerDemo::clock.getAnimationMode () )
            message << "animation mode, fixed frame rate ("
            << OpenSteer::OpenSteerDemo::clock.getFixedFrameRate () << " fps)";
        else
        {
            message << "real-time mode, ";
            if ( OpenSteer::OpenSteerDemo::clock.getVariableFrameRateMode () )
                message << "variable frame rate";
            else
                message << "fixed frame rate ("
                << OpenSteer::OpenSteerDemo::clock.getFixedFrameRate () << " fps)";
        }
        message << std::ends;
        EduNet::Log::printMessage ( message );
        break;

        // print minimal help for single key commands
    case '?':
        OpenSteer::OpenSteerDemo::keyboardMiniHelp ();
        break;

        // exit application with normal status
    case esc:
    {
#if 0
// to be investigated
        EduNet::Log::printMessage ( "exit." );
        GLUI* glui = ::getRootGLUI();
        if ( NULL != glui )
        {
            int glutWindow = glutGetWindow();
            GLUI_Master.close_all();
            //glui->close();
            //                                          OpenSteer::Plugin::selectedPlugin( NULL );
            //OpenSteer::OpenSteerDemo::exit (0);
        }
        else
        {
            glutDestroyWindow ( windowID );
            OpenSteer::OpenSteerDemo::exit ( 0 );
        }
#endif
        EduNet::Application::_SDMCleanup();
		EduNet::Application::_SDMShutdown();
        OpenSteer::OpenSteerDemo::exit ( 0 );
    }
    break;
//                      glutDestroyWindow (windowID);
//                      EduNet::Log::printMessage ("exit.");
//                      OpenSteer::OpenSteerDemo::exit (0);

    default:
        message << "unrecognized single key command: " << key;
        message << " (" << ( int ) key << ") x(" << x << ") y(" << y << ")";
        message << std::ends;
        EduNet::Log::printMessage ( "" );
        EduNet::Log::printMessage ( message );
//                      OpenSteer::OpenSteerDemo::keyboardMiniHelp ();
    }
}


// ------------------------------------------------------------------------
// handles "special" keys,
// function keys are handled by the Plugin
//
// parameter names commented out to prevent compiler warning from "-W"
void
keyboardSpecialFunc ( int key, int /*x*/, int /*y*/ )
{
    //          if (demo)
    //          {
    //                  demo->keyboardSpecialFunc(key,x,y);
    //          }
    std::ostringstream message;

    switch ( key )
    {
        /*case GLUT_KEY_F1:  OpenSteer::OpenSteerDemo::functionKeyForPlugin (1);  break;
        case GLUT_KEY_F2:  OpenSteer::OpenSteerDemo::functionKeyForPlugin (2);  break;
        case GLUT_KEY_F3:  OpenSteer::OpenSteerDemo::functionKeyForPlugin (3);  break;
        case GLUT_KEY_F4:  OpenSteer::OpenSteerDemo::functionKeyForPlugin (4);  break;
        case GLUT_KEY_F5:  OpenSteer::OpenSteerDemo::functionKeyForPlugin (5);  break;
        case GLUT_KEY_F6:  OpenSteer::OpenSteerDemo::functionKeyForPlugin (6);  break;
        case GLUT_KEY_F7:  OpenSteer::OpenSteerDemo::functionKeyForPlugin (7);  break;
        case GLUT_KEY_F8:  OpenSteer::OpenSteerDemo::functionKeyForPlugin (8);  break;
        case GLUT_KEY_F9:  OpenSteer::OpenSteerDemo::functionKeyForPlugin (9);  break;
        case GLUT_KEY_F10: OpenSteer::OpenSteerDemo::functionKeyForPlugin (10); break;
        case GLUT_KEY_F11: OpenSteer::OpenSteerDemo::functionKeyForPlugin (11); break;
        case GLUT_KEY_F12: OpenSteer::OpenSteerDemo::functionKeyForPlugin (12); break;*/

    case GLUT_KEY_RIGHT:
        OpenSteer::OpenSteerDemo::clock.setPausedState ( true );
        message << "single step forward (frame time: "
        << OpenSteer::OpenSteerDemo::clock.advanceSimulationTimeOneFrame ()
        << ")"
        << std::endl;
        EduNet::Log::printMessage ( message );
        break;
        /*case GLUT_KEY_UP: OpenSteer::OpenSteerDemo::functionKeyForPlugin (GLUT_KEY_UP); break;
        case GLUT_KEY_DOWN: OpenSteer::OpenSteerDemo::functionKeyForPlugin (GLUT_KEY_DOWN); break;
        */
    default:
        OpenSteer::Plugin::functionKeyForPlugin ( key );
        break;
    }
}


void
displayFunc000 ( void )
{
    {
        ET_PROFILE ( update );

        // update global simulation clock
        OpenSteer::OpenSteerDemo::clock.update ();
        //  start the phase timer (XXX to accurately measure "overhead" time this
        //  should be in displayFunc, or somehow account for time outside this
        //  routine)
        OpenSteer::OpenSteerDemo::initPhaseTimers ();

        // run selected Plugin (with simulation's current time and step size)
        OpenSteer::OpenSteerDemo::updateSelectedPlugin ( OpenSteer::OpenSteerDemo::clock.getTotalSimulationTime (),
                OpenSteer::OpenSteerDemo::clock.getElapsedSimulationTime () );
        // clear color and depth buffers
    }


    {
        ET_PROFILE ( draw );

        // run simulation and draw associated graphics
        //              OpenSteer::OpenSteerDemo::updateSimulationAndRedraw ();

        glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

        // redraw selected Plugin (based on real time)
        OpenSteer::OpenSteerDemo::redrawSelectedPlugin (
            OpenSteer::OpenSteerDemo::clock.getTotalRealTime(),
            OpenSteer::OpenSteerDemo::clock.getElapsedRealTime()
        );

        // draw the name of the selected Plugin
        drawDisplayPluginName ();

        if ( false == EduNet::Application::AccessApplication().isProfileVisible() )
        {
            if ( true == EduNet::Application::AccessApplication().isOpenSteerProfileVisible() )
            {
                // draw text showing (smoothed, rounded) "frames per second" rate
                drawDisplayFPS ();


                // draw the name of the camera's current mode
                drawDisplayCameraModeName ();
            }
        }

        // draw crosshairs to indicate aimpoint (xxx for debugging only?)
        // drawReticle ();

        // check for errors in drawing module, if so report and exit
        OpenSteer::checkForDrawError ( "OpenSteerDemo::updateSimulationAndRedraw" );

    }
    {
        EduNet::Application::AccessApplication().drawProfile (
            OpenSteer::OpenSteerDemo::clock.getTotalRealTime(),
            OpenSteer::OpenSteerDemo::clock.getElapsedRealTime()
        );
    }
    {
        // double buffering, swap back and front buffers
        glFlush ();
        glutSwapBuffers();
    }
}

// ------------------------------------------------------------------------
void displayFunc ( void )
{
    ET_PROFILE ( displayFunc );

	if ( glutGetWindow() != windowID )
		glutSetWindow ( windowID );

    displayFunc000();

    
}
// ------------------------------------------------------------------------
void idleFunc ( void )
{
#ifdef ET_DEBUG
    EduNet::sleep ( 5 );
#else
    EduNet::sleep ( 1 );
#endif
#if 0
    /* According to the GLUT specification, the current window is
    undefined during an idle callback.  So we need to explicitly change
    it if necessary */
    if ( glutGetWindow() != windowID )
	{
        glutSetWindow ( windowID );
	}

    glutPostRedisplay();
#endif
    displayFunc();
}

//-----------------------------------------------------------------------------
// console exit callback
//-----------------------------------------------------------------------------
void consoleExit ( int i )
{
    EduNet::Log::printMessage ( "console exit ..." );
    EduNet::Application::_SDMCleanup();
	EduNet::Application::_SDMShutdown();
}

//-----------------------------------------------------------------------------
// window exit callback
//-----------------------------------------------------------------------------
void windowExit ( int i )
{
    EduNet::Log::printMessage ( "window exit ..." );
	// note a valid cleanup at this point has to be implemented

    EduNet::Application::_SDMCleanup();
	EduNet::Application::_SDMShutdown();
	::exit ( i );
}

} // annonymous namespace


//-----------------------------------------------------------------------------
void
OpenSteer::OpenSteerDemo::initializeGraphics ( int argc, char **argv )
{
    // initialize GLUT state based on command line arguments
#ifndef WIN32
    glutInit ( &argc, argv );
#else
    __glutInitWithExit ( &argc, argv, consoleExit );
#endif //WIN32

    // display modes: RGB+Z and double buffered
    GLint mode = GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE;
    mode = GLUT_RGBA | GLUT_DOUBLE |GLUT_DEPTH | GLUT_STENCIL;
    glutInitDisplayMode ( mode );

    // create and initialize our window with GLUT tools
    // (center window on screen with size equal to "ws" times screen size)
    const int sw = glutGet ( GLUT_SCREEN_WIDTH );
    const int sh = glutGet ( GLUT_SCREEN_HEIGHT );
    const float ws = 0.5f; // window_size / screen_size
//      const float ws = 0.8f; // window_size / screen_size
    const int ww = ( int ) ( sw * ws );
    const int wh = ( int ) ( sh * ws );
    glutInitWindowPosition ( ( int ) ( sw * ( 1-ws ) /2 ), ( int ) ( sh * ( 1-ws ) /2 ) );
    glutInitWindowSize ( ww, wh );

#ifndef WIN32
    windowID = glutCreateWindow ( appVersionName );
#else
    windowID = __glutCreateWindowWithExit ( appVersionName, windowExit );
#endif //WIN32


    //reshapeFunc ( ww, wh );
    initGL ();

    // register our display function, make it the idle handler too
	setGlutFunctions();
	
	GLUI_Master.set_glutReshapeFunc ( &reshapeFunc );
    GLUI_Master.set_glutKeyboardFunc ( &keyboardFunc );
    GLUI_Master.set_glutSpecialFunc ( NULL );
    GLUI_Master.set_glutIdleFunc ( &idleFunc );
    GLUI_Master.set_glutDisplayFunc ( &displayFunc );
    GLUI_Master.set_glutMouseFunc ( &mouseButtonFunc );

	/****************************************/
	/*         Here's the GLUI code         */
	/****************************************/
	
	printf( "GLUI version: %3.2f\n", GLUI_Master.get_version() );
	
    GLUI* glui = GLUI_Master.create_glui_subwindow ( windowID,
                 GLUI_SUBWINDOW_RIGHT );
	
	// sort plugins before adding them to the gui
    Plugin::sortBySelectionOrder();
    // add common gui elements
    EduNet::Application::AccessApplication().addGuiElements ( glui );

    // GLUI setup
    /*GLUI_Master.set_glutReshapeFunc ( &reshapeFunc );
    GLUI_Master.set_glutKeyboardFunc ( &keyboardFunc );
    GLUI_Master.set_glutSpecialFunc ( &keyboardSpecialFunc );
    GLUI_Master.set_glutIdleFunc ( &idleFunc );
    GLUI_Master.set_glutDisplayFunc ( &displayFunc );
    GLUI_Master.set_glutMouseFunc ( &mouseButtonFunc );
*/
	
    glui->set_main_gfx_window ( windowID );   
}
//-----------------------------------------------------------------------------
void OpenSteer::OpenSteerDemo::setGlutFunctions( void )
{
	glutDisplayFunc ( &displayFunc );
    glutIdleFunc ( &idleFunc );
	
    // register handler for window reshaping
    glutReshapeFunc ( &reshapeFunc );
	
    // register handler for keyboard events
    glutKeyboardFunc ( &keyboardFunc );
    glutKeyboardUpFunc ( &keyboardFuncUp );
	
    glutSpecialFunc ( &keyboardSpecialFunc );
	
    // register handler for mouse button events
    glutMouseFunc ( &mouseButtonFunc );
	
    // register handler to track mouse motion when any button down
    glutMotionFunc ( mouseMotionFunc );
	
    // register handler to track mouse motion when no buttons down
    glutPassiveMotionFunc ( mousePassiveMotionFunc );
	
    // register handler for when mouse enters or exists the window
    glutEntryFunc ( mouseEnterExitWindowFunc );
}
//-----------------------------------------------------------------------------
// run graphics event loop
void
OpenSteer::OpenSteerDemo::runGraphics ( void )
{
    // Use a timer to control the frame rate.
    glutTimerFunc ( framePeriod, timerFunc, 0 );
    glutMainLoop ();
}

