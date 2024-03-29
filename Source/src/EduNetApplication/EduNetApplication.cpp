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

#include "EduNetApplication.h"
#include "EduNetGames.h"
#include "EduNetProcessProfile.h"

// #include "EduNetProfile/GraphPlot.h"
// #include "EduNetProfile/EduNetProfile.h"

#include "EduNetCommon/EduNetOptions.h"

#include "OpenSteerUT/OpenSteerUT.h"
#include "OpenSteer/GlobalSelection.h"
#include "OpenSteer/GlobalData.h"
#include "OpenSteer/PluginRegistry.h"
#include "OpenSteer/Draw.h"

using namespace OpenSteer;
using namespace EduNet;

bool g_bRunCalled = false;

//-----------------------------------------------------------------------------
bool InitializeGlobals( void )
{
	static EduNet::ProcessProfile kProfile;
	OpenSteerUTData::_SDMInitApp(&kProfile);

	//static OpenSteer::OpenGLRenderer kRenderer;
	//OpenSteer::GlobalData::getInstance()->setRenderer( &kRenderer );
	return true;
}

bool bGlobalsInitialized = InitializeGlobals();

//-----------------------------------------------------------------------------
namespace
{
	int profReportMode = 0;

	int pluginSelection = 0;
	int pluginIndex = 0;

	GLUI_Listbox* pluginList = NULL;
	GLUI_Listbox* profileModesList = NULL;

	GLUI_Panel* pluginPanel = NULL;
	GLUI_StaticText* simulationFPS = NULL;
	GLUI_StaticText* cpuFPS = NULL;

	float fSimulationFPS = 50.0f;
}

//-----------------------------------------------------------------------------
void OpenSteer::handleGlobalDataInstanceFailure( void )
{
	if( ( false == GlobalData::hasInstance() ) )
	{
		// caution this function should never get called
		// it actually means that you are working with global data
		bGlobalsInitialized = InitializeGlobals();
	}
}

//-----------------------------------------------------------------------------
void setDefaultSettings()
{

}

//-----------------------------------------------------------------------------
void setDefaultSettingsAndSync()
{
	setDefaultSettings();
//	glui->sync_live();
}

//-----------------------------------------------------------------------------
void gluiNextPlugin()
{
	OpenSteer::Plugin::selectNextPlugin();
}

//-----------------------------------------------------------------------------
void gluiSelectPlugin()
{
	AbstractPlugin* currentPlugin = OpenSteer::Plugin::getSelectedPlugin();
	if( pluginSelection != pluginIndex )
	{
		Plugin::selectPluginByIndex( pluginSelection );
		pluginIndex = pluginSelection;
	}
}

//-----------------------------------------------------------------------------
void onPluginSelectedCallback( OpenSteer::AbstractPlugin* plugin )
{
	EduNet::Application::AccessApplication().onPluginSelected( plugin );
}

//-----------------------------------------------------------------------------
void gluiProfMoveCursorNext()
{
	Prof_move_cursor(1);
}

//-----------------------------------------------------------------------------
void gluiProfMoveCursorPrevious()
{
	Prof_move_cursor(-1);
}

//-----------------------------------------------------------------------------
void gluiProfSelect()
{
	profReportMode = Prof_CALL_GRAPH;
	profileModesList->do_selection( profReportMode );
	Prof_select();
}

//-----------------------------------------------------------------------------
void gluiProfSelectParent()
{
	Prof_select_parent();
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
Application& Application::AccessApplication( void )
{
	PluginRegistry::accessInstance()->setOnPluginSelectedFunc( onPluginSelectedCallback );
	Application* application = Application::accessInstance();
	if( NULL == application )
	{
		Application::createInstance();
	}
	return *Application::accessInstance();
}

ET_IMPLEMENT_SINGLETON(Application);
//-----------------------------------------------------------------------------
Application::Application( void ):
m_fSimulationFPS( fSimulationFPS ),
m_fTimeFactor(1.0f),
m_bFixedSimulationFPS(1),
m_bEnableAnnotation(0),
m_bShowOpenSteerProfile( 0 ),
m_bShowCPUProfile(0),
m_bShowCPUProfileGraph(0),
m_bUpdateCPUProfile(0),
m_fUpdateCPUTime( 0.0f ),
m_bAllowLocalPlayer(false)
{
	setDefaultSettings();
	this->m_kUpdatePeriod.SetPeriodFrequency( m_fSimulationFPS );
}

//-----------------------------------------------------------------------------
Application::~Application( void )
{
}

//-----------------------------------------------------------------------------
void fnExit0 (void)
{
	EduNet::Application::_SDMCleanup();
	EduNet::Application::_SDMShutdown();
}

//-----------------------------------------------------------------------------
void Application::_SDMInit( void )
{
	//atexit (fnExit0);
	Application* application = Application::accessInstance();
	if( NULL == application )
	{
		Application::createInstance();
	}
}

//-----------------------------------------------------------------------------
void Application::_SDMCleanup( void )
{

//	EduNet::Log::printMessage( "Application cleanup ..." );
	static bool bCleanedup = false;
	if( true == bCleanedup )
	{
		return;
	}
	bCleanedup = true;
	if( NULL != OpenSteer::Plugin::getSelectedPlugin() )
	{
		OpenSteer::Plugin::selectPlugin( NULL );
	}

	if( NULL != OpenSteerUTData::g_openSteerUTDataPtr->appGlui )
	{
//		GLUI_Master.close_all();
//		g_openSteerUTData.appGlui->close_internal();
// 		g_openSteerUTData.appGlui->close();
// 		g_openSteerUTData.appGlui = NULL;
	}


//	EduNet::Log::printMessage( "Application cleanup - done." );
}

//-----------------------------------------------------------------------------
void Application::_SDMShutdown( void )
{
	//EduNet::Log::printMessage( "Application shutdown ..." );
	static bool bShutdown = false;
	if( true == bShutdown )
	{
		return;
	}
	bShutdown = true;
	Application::destroyInstance();
	//sEduNet::Log::printMessage( "Application shutdown - done." );

	static bool bDumpedMemLeaks = false;
	if( false == bDumpedMemLeaks )
	{
		bDumpedMemLeaks = true;
		// note: activate to see all the leaks
#if 0
#ifdef ET_DEBUG
#ifdef WIN32
		_CrtSetReportMode( _CRT_WARN, _CRTDBG_MODE_DEBUG);
		_CrtSetReportMode( _CRT_ERROR, _CRTDBG_MODE_DEBUG );

		// manually dump leaks now as we are exiting the glut way
		_CrtDumpMemoryLeaks();
#endif
#endif
#endif
	}
}

//-----------------------------------------------------------------------------
int Application::Run(int argc, char **argv)
{
	//g_bRunCalled = true;
	//// register dynamic plugins
	//EduNet::initializeDynamicPlugins( );

	//// check if there is a default plugin
	//const char* pszPluginName = EduNet::Options::accessOptions().getSelectedPlugin();
	//OpenSteer::AbstractPlugin* pkPlugin = Plugin::findByName ( pszPluginName );
	//if ( NULL == pkPlugin )
	//{
	//	pkPlugin = Plugin::findDefault();
	//}
	//if( NULL != pkPlugin )
	//{
	//	// initialize graphics
	//	EduNet::GameDemo::initializeGraphics (argc, argv);

	//	// initialize OpenSteerDemo application
	//	EduNet::GameDemo::initialize ();

	//	// run the main event processing loop
	//	EduNet::GameDemo::runGraphics ();
	//}
	//else
	//{
	//	printf( "no plugin found - shutting down\n" );
	//}
	return EXIT_SUCCESS;
}

//-----------------------------------------------------------------------------
void Application::addGuiElements( GLUI* glui )
{
	OpenSteerUTData::g_openSteerUTDataPtr->appGlui = glui;

	glui->add_statictext("Plugins");
	pluginList = glui->add_listbox( "", &pluginSelection );

	int numPlugins = Plugin::getNumPlugins();
	for (int i = 0; i < numPlugins; ++i)
	{
		AbstractPlugin* pi = Plugin::getPluginAt(i);
		const char* s = pi->pluginName();
		pluginList->add_item(i, s);
	}

	pluginIndex = pluginSelection = Plugin::getPluginIdx( OpenSteer::Plugin::getSelectedPlugin() );
	pluginList->do_selection( pluginSelection );

	glui->add_button("Next Plugin", 0,(GLUI_Update_CB)gluiNextPlugin);

#if EDUNET_HAVE_PROFILE
	// profiler gui
	glui->add_separator();

	{
		GLUI_Rollout* rollout = glui->add_rollout( "Profile", false );
		GLUI_Panel* subPanel = rollout;

		glui->add_checkbox_to_panel( subPanel, "Show OpenSteer Profile", &this->m_bShowOpenSteerProfile);

		glui->add_separator_to_panel( subPanel );
		profileModesList = glui->add_listbox_to_panel( subPanel, "Modes", &profReportMode );
		profileModesList->add_item(0, "Self Time");
		profileModesList->add_item(1, "Hierarchical Time");
		profileModesList->add_item(2, "Call Graph");
		profileModesList->do_selection( profReportMode );

		glui->add_checkbox_to_panel( subPanel, "Update CPU Profile", &this->m_bUpdateCPUProfile);
		glui->add_checkbox_to_panel( subPanel, "Show CPU Profile", &this->m_bShowCPUProfile);
		glui->add_checkbox_to_panel( subPanel, "Show CPU Profile Graph", &this->m_bShowCPUProfileGraph);

		glui->add_separator_to_panel( subPanel );
		glui->add_button_to_panel( subPanel, "Next", 0,(GLUI_Update_CB)gluiProfMoveCursorNext );
		glui->add_button_to_panel( subPanel, "Previous", 0,(GLUI_Update_CB)gluiProfMoveCursorPrevious );

		glui->add_separator_to_panel( subPanel );
		glui->add_button_to_panel( subPanel, "Select", 0,(GLUI_Update_CB)gluiProfSelect );
		glui->add_button_to_panel( subPanel, "Select parent", 0,(GLUI_Update_CB)gluiProfSelectParent );
	}

#endif
	glui->add_separator();

	// simulation gui
	{
		// general
		GLUI_Rollout* rollout = glui->add_rollout( "Simulation", false );
		GLUI_Panel* subPanel = rollout;
		GLUI_Spinner* timefactorSpinner =
			glui->add_spinner_to_panel( subPanel, "Timefactor", GLUI_SPINNER_FLOAT, &this->m_fTimeFactor);
		timefactorSpinner->set_float_limits(0.01f, 10.0f);

		GLUI_Spinner* simulationFPSSpinner =
			glui->add_spinner_to_panel( subPanel, "Simulation FPS", GLUI_SPINNER_FLOAT, &this->m_fSimulationFPS);
		simulationFPSSpinner->set_float_limits(10.0f, 100.0f);

		glui->add_checkbox_to_panel( subPanel, "Fixed Timestep", &this->m_bFixedSimulationFPS);
		simulationFPS = glui->add_statictext_to_panel( subPanel, "FPS" );

		cpuFPS = glui->add_statictext_to_panel( subPanel, "CPU" );

		glui->add_separator_to_panel( subPanel );
		// steering force update

		GLUI_Spinner* steeringForceFPSSpinner =
			glui->add_spinner_to_panel( subPanel, "Steering FPS", GLUI_SPINNER_FLOAT, &GlobalData::getInstance()->m_SteeringForceFPS);
		steeringForceFPSSpinner->set_float_limits(5.0f, 60.0f);

		GLUI_Spinner* vehicleReplicationFPSSpinner =
			glui->add_spinner_to_panel( subPanel, "Vehicle Replication FPS", GLUI_SPINNER_FLOAT, &GlobalData::getInstance()->m_NetWriteFPS);
		vehicleReplicationFPSSpinner->set_float_limits(0.1f, 30.0f);

	}

	glui->add_separator();
	glui->add_checkbox("Enable Annotation", &this->m_bEnableAnnotation);
	glui->add_checkbox("Vehicle Annotation", &OpenSteer::GlobalData::getInstance()->m_collect3DAnnotations);

	glui->add_separator();

	// initially add the specific plugin panel
	this->onPluginSelected( OpenSteer::Plugin::getSelectedPlugin() );
}

//-----------------------------------------------------------------------------
void Application::onPluginSelected( OpenSteer::AbstractPlugin* pkPlugin )
{
	if( OpenSteerUTData::g_openSteerUTDataPtr->appGlui != NULL )
	{
		AbstractPlugin* pi = Plugin::getPluginAt( pluginSelection );
		if( pkPlugin != pi )
		{
			int numPlugins = Plugin::getNumPlugins();
			for (int i = 0; i < numPlugins; ++i)
			{
				AbstractPlugin* piList = Plugin::getPluginAt(i);
				if( piList == pkPlugin )
				{
					pluginSelection = pluginIndex = i;
					pluginList->do_selection( i );
					break;
				}
			}
		}

		if( pluginPanel != NULL )
		{
			pluginPanel->unlink();
			delete pluginPanel;
			pluginPanel = NULL;
		}
		if( NULL != pkPlugin )
		{
			if( pluginPanel == NULL )
			{
				pluginPanel = AbstractPluginGui::initRootPluginGui( pkPlugin );
			}
		}
	}
}
//
////-----------------------------------------------------------------------------
//void Application::updateSelectedPlugin (const float currentTime,
//						   const float elapsedTime )
//{
//	::gluiSelectPlugin();
//
//	// opensteer demo options update
//
//	if( ( NULL == OpenSteer::Plugin::getSelectedPlugin() ) || ( elapsedTime <= 0.0f ) )
//	{
//		return;
//	}
//
//	// if no vehicle is selected, and some exist, select the first one
//	if( SimpleVehicle::getSelectedVehicle() == NULL )
//	{
//		const AVGroup& vehicles = OpenSteer::Plugin::getSelectedPlugin()->allVehicles();
//		if( vehicles.size() > 0 )
//		{
//			SimpleVehicle::setSelectedVehicle( vehicles.front() );
//		}
//	}
//
//	osScalar fSimulationFPS = this->m_fSimulationFPS;
//
//	// in case the last CPU time was higher than the incoming delta time
//	// reduce the simulation rate
//	if( this->m_fUpdateCPUTime > elapsedTime )
//	{
//		fSimulationFPS *= ( elapsedTime / this->m_fUpdateCPUTime );
//	}
//
//	if( this->m_fUpdateCPUTime > 0.0f )
//	{
//		// in case the last CPU frame rate was lower than the
//		// target frame rate reduce the target frame rate
//		osScalar fCPUFPS = ( 1.0f / this->m_fUpdateCPUTime );
//		if( fCPUFPS < fSimulationFPS )
//		{
////			blendIntoAccumulator(this->m_kUpdatePeriod.GetPeriodTime(), fCPUFPS, fSimulationFPS);
//			fSimulationFPS = fCPUFPS * 0.9f;
//		}
//	}
//
//
//	{
//		std::ostringstream status;
//		status << std::setprecision (2);
//		status << std::setiosflags (std::ios::fixed);
//		status << "Sim FPS: ";
//		status << fSimulationFPS;
//		simulationFPS->set_text( status.str().c_str() );
//	}
//
//	{
//		if( this->m_fUpdateCPUTime > 0.0f )
//		{
//			std::ostringstream status;
//			status << std::setprecision (2);
//			status << std::setiosflags (std::ios::fixed);
//			osScalar fCPUFPS = ( 1.0f / this->m_fUpdateCPUTime );
//			status << "CPU FPS: ";
//			status << fCPUFPS;
//			cpuFPS->set_text( status.str().c_str() );
//		}
//	}
//
//
//	// fixed timestep implementation
//	this->m_kUpdatePeriod.SetPeriodFrequency( fSimulationFPS );
//
//	float fCurrentAccumTime = this->m_kUpdatePeriod.GetAccumTime();
//
//	// take time factor into account
//	float fModifiedDeltaTime = elapsedTime * m_fTimeFactor;
//	size_t uiTicks = this->m_kUpdatePeriod.UpdateDeltaTime( fModifiedDeltaTime );
//
//	// only update in case at least one tick has been generated
//	if( uiTicks > 0 )
//	{
//		this->m_kUpdateClock.update();
//		const osScalar preUpdateElapsedTime = m_kUpdateClock.getElapsedRealTime();
//
//		osAbstractUpdated* pkUpdatedPlugin = dynamic_cast<osAbstractUpdated*>(OpenSteer::Plugin::getSelectedPlugin());
//		assert( NULL != pkUpdatedPlugin );
//
//		if( true == ( this->m_bFixedSimulationFPS == 1 ) )
//		{
//			OpenSteer::setAnnotationOff();
//			while( uiTicks > 0 )
//			{
//				if( uiTicks == 1 )
//				{
//					if( m_bEnableAnnotation == 1 )
//					{
//						OpenSteer::setAnnotationOn();
//					}
//				}
//				pkUpdatedPlugin->update( fCurrentAccumTime, this->m_kUpdatePeriod.GetPeriodTime() );
//				fCurrentAccumTime += this->m_kUpdatePeriod.GetPeriodTime();
//				--uiTicks;
//			}
//		}
//		else
//		{
//			if( m_bEnableAnnotation == 1 )
//			{
//				OpenSteer::setAnnotationOn();
//			}
//			else
//			{
//				OpenSteer::setAnnotationOff();
//			}
//			float fAccumDeltaTime = this->m_kUpdatePeriod.GetDeltaTime( uiTicks );
//			pkUpdatedPlugin->update( fCurrentAccumTime + fAccumDeltaTime, fAccumDeltaTime );
//		}
//
//		this->m_kUpdateClock.update();
//		const osScalar postUpdateElapsedTime = m_kUpdateClock.getElapsedRealTime();
//		const osScalar fUpdateCPUTime = postUpdateElapsedTime - preUpdateElapsedTime;
//		this->m_fUpdateCPUTime += fUpdateCPUTime;
//		this->m_fUpdateCPUTime *= 0.5f;
//
//	}
//}

//-----------------------------------------------------------------------------
bool Application::isOpenSteerProfileVisible( void ) const
{
	return ( ( 0 != this->m_bShowOpenSteerProfile ) );
}

//-----------------------------------------------------------------------------
bool Application::isProfileVisible( void ) const
{
	return ( ( 0 != this->m_bShowCPUProfile ) || ( 0 != this->m_bShowCPUProfileGraph ) );
}

////-----------------------------------------------------------------------------
//void Application::drawProfile (const float currentTime,
//				  const float elapsedTime)
//{
//#if EDUNET_HAVE_PROFILE
//	Prof_set_report_mode( static_cast<Prof_Report_Mode>(profReportMode) );
//	Prof_update( this->m_bUpdateCPUProfile );
//	const float tw = OpenSteer::drawGetWindowWidth();
//	const float th = OpenSteer::drawGetWindowHeight();
//	if( ( 0 != this->m_bShowCPUProfile ) )
//	{
//		OpenSteer::profileDraw( 10, 300, 550, 500, -16, 2, tw, th );
//	}
//	if( ( 0 != this->m_bShowCPUProfileGraph ) )
//	{
//		Profile::GraphPlot kPlot;
//		const float fYSpacing = 4.0f;
//		OpenSteer::profileDrawGraph( 10.0, 350.0, 4.0, fYSpacing, tw, th );
//		kPlot.drawGraphFrame( 10.0, 350.0, 4.0 * 128, fYSpacing * 50, false );
//	}
//#endif
//}

//-----------------------------------------------------------------------------
void Application::initialize ( void )
{

}
