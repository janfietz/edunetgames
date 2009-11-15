#include "EduNetApplication.h"

#include "EduNetCommon/EduNetDraw.h"
#include "EduNetCommon/EduNetOptions.h"
#include "EduNetGames.h"


using namespace EduNet;
using namespace OpenSteer;


//-----------------------------------------------------------------------------
namespace
{
	int pluginSelection = 0;
	int pluginIndex = 0;

	GLUI* appGlui = NULL;
	GLUI_Listbox* pluginList = NULL;

	GLUI_Panel* pluginPanel = NULL;
	GLUI_StaticText* simulationFPS = NULL;
	GLUI_StaticText* cpuFPS = NULL;
}

//-----------------------------------------------------------------------------
GLUI* getRootGLUI()
{
	return appGlui;
}

//-----------------------------------------------------------------------------
void setDefaultSettings()
{

}

//-----------------------------------------------------------------------------
void	setDefaultSettingsAndSync()
{
	setDefaultSettings();
//	glui->sync_live();
}

//-----------------------------------------------------------------------------
void gluiNextPlugin()
{
	OpenSteer::OpenSteerDemo::selectNextPlugin();
}

//-----------------------------------------------------------------------------
void gluiSelectPlugin()
{
	AbstractPlugin* currentPlugin = OpenSteer::Plugin::selectedPlugin;
	if( pluginSelection != pluginIndex )
	{
		OpenSteerDemo::selectPluginByIndex( pluginSelection );	
		pluginIndex = pluginSelection;
	}
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
Application& Application::AccessApplication( void )
{
	static Application kApplication;
	return kApplication;
}

//-----------------------------------------------------------------------------
Application::Application( void ):
m_fSimulationFPS(60.0f),
m_fTimeFactor(1.0f),
m_bFixedSimulationFPS(1),
m_bEnableAnnotation(0),
m_bShowCPUProfile(0),
m_bShowCPUProfileGraph(0),
m_bUpdateCPUProfile(0),
m_fUpdateCPUTime( 0.0f )
{
	setDefaultSettings();
	this->m_kUpdatePeriod.SetPeriodFrequency( m_fSimulationFPS );
}

//-----------------------------------------------------------------------------
Application::~Application( void )
{
	bool bTest = true;
	bTest = false;
	Application::_SDMCleanup();
}

//-----------------------------------------------------------------------------
void fnExit0 (void)
{
	Application::_SDMShutdown();
}

//-----------------------------------------------------------------------------
void Application::_SDMInit( void )
{
	atexit (fnExit0);
}

//-----------------------------------------------------------------------------
void Application::_SDMCleanup( void )
{
	static bool bCleanedup = false;
	if( true == bCleanedup )
	{
		return;
	}
	bCleanedup = true;
	if( NULL != OpenSteer::Plugin::selectedPlugin )
	{
		OpenSteer::Plugin::selectedPlugin->close();
		OpenSteer::Plugin::selectedPlugin = NULL;
	}
}


//-----------------------------------------------------------------------------
void Application::_SDMShutdown( void )
{
	printf( "shutdown ...\n");
	static bool bShutdown = false;
	if( true == bShutdown )
	{
		return;
	}
	bShutdown = true;
}


//-----------------------------------------------------------------------------
void Application::addGuiElements( GLUI* glui )
{
	appGlui = glui;

	glui->add_statictext("Plugins");
	pluginList = glui->add_listbox( "", &pluginSelection );

	int numPlugins = Plugin::getNumPlugins();
	for (int i = 0; i < numPlugins; ++i)
	{
		AbstractPlugin* pi = Plugin::getPluginAt(i);
		const char* s = pi->name();
		pluginList->add_item(i, s);
	}
	pluginIndex = pluginSelection = Plugin::getPluginIdx( OpenSteer::Plugin::selectedPlugin );
	pluginList->do_selection( pluginSelection );

	glui->add_button("Next Plugin", 0,(GLUI_Update_CB)gluiNextPlugin);

#if EDUNET_HAVE_PROFILE
	glui->add_separator();

	glui->add_checkbox("Update CPU Profile", &this->m_bUpdateCPUProfile);
	glui->add_checkbox("Show CPU Profile", &this->m_bShowCPUProfile);
	glui->add_checkbox("Show CPU Profile Graph", &this->m_bShowCPUProfileGraph);
#endif
	glui->add_separator();

	GLUI_Spinner* timefactorSpinner =
		glui->add_spinner("Timefactor", GLUI_SPINNER_FLOAT, &this->m_fTimeFactor);
	timefactorSpinner->set_float_limits(0.01f, 10.0f);

	GLUI_Spinner* simulationFPSSpinner =
		glui->add_spinner("Simulation FPS", GLUI_SPINNER_FLOAT, &this->m_fSimulationFPS);
	simulationFPSSpinner->set_float_limits(5.0f, 200.0f);
	glui->add_checkbox("Fixed Timestep", &this->m_bFixedSimulationFPS);
	simulationFPS = glui->add_statictext( "FPS" );
	cpuFPS = glui->add_statictext( "CPU" );
	glui->add_separator();
	glui->add_checkbox("Enable Annotation", &this->m_bEnableAnnotation);

	glui->add_separator();

	// initially add the specific plugin panel
	this->onPluginSelected( OpenSteer::Plugin::selectedPlugin );
}

//-----------------------------------------------------------------------------
void Application::onPluginSelected( OpenSteer::AbstractPlugin* pkPlugin )
{
	if( appGlui != NULL )
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
		if( pluginPanel == NULL )
		{
			pluginPanel = appGlui->add_panel( pkPlugin ? pkPlugin->name() : "Plugin" );
			pkPlugin->initGui( pluginPanel );
		}
	}
}

//-----------------------------------------------------------------------------
void Application::updateSelectedPlugin (const float currentTime,
						   const float elapsedTime )
{
	::gluiSelectPlugin();

	// opensteer demo options update

	if( ( NULL == OpenSteer::Plugin::selectedPlugin ) || ( elapsedTime <= 0.0f ) )
	{
		return;
	}

	osScalar fSimulationFPS = this->m_fSimulationFPS;

	// in case the last CPU time was higher than the incoming delta time
	// reduce the simulation rate
	if( this->m_fUpdateCPUTime > elapsedTime )
	{
		fSimulationFPS *= ( elapsedTime / this->m_fUpdateCPUTime );
	}

	if( this->m_fUpdateCPUTime > 0.0f )
	{
		// in case the last CPU frame rate was lower than the
		// target frame rate reduce the target frame rate
		osScalar fCPUFPS = ( 1.0f / this->m_fUpdateCPUTime );
		if( fCPUFPS < fSimulationFPS )
		{
//			blendIntoAccumulator(this->m_kUpdatePeriod.GetPeriodTime(), fCPUFPS, fSimulationFPS);
			fSimulationFPS = fCPUFPS * 0.9f;
		}
	}


	{
		std::ostringstream status;
		status << std::setprecision (2);
		status << std::setiosflags (std::ios::fixed);
		status << "Sim FPS: ";
		status << fSimulationFPS;
		simulationFPS->set_text( status.str().c_str() );
	}
	{
		if( this->m_fUpdateCPUTime > 0.0f )
		{
			std::ostringstream status;
			status << std::setprecision (2);
			status << std::setiosflags (std::ios::fixed);
			osScalar fCPUFPS = ( 1.0f / this->m_fUpdateCPUTime );
			status << "CPU FPS: ";
			status << fCPUFPS;
			cpuFPS->set_text( status.str().c_str() );
		}
	}


	// fixed timestep implementation
	this->m_kUpdatePeriod.SetPeriodFrequency( fSimulationFPS );
	
	float fCurrentAccumTime = this->m_kUpdatePeriod.GetAccumTime();

	// take time factor into account
	float fModifiedDeltaTime = elapsedTime * m_fTimeFactor;
	size_t uiTicks = this->m_kUpdatePeriod.UpdateDeltaTime( fModifiedDeltaTime );
	
	// only update in case at least one tick has been generated
	if( uiTicks > 0 )
	{
		this->m_kUpdateClock.update();
		const osScalar preUpdateElapsedTime = m_kUpdateClock.getElapsedRealTime();

		if( true == ( this->m_bFixedSimulationFPS == 1 ) )
		{
			OpenSteer::enableAnnotation = false;
			while( uiTicks > 0 )
			{
				if( uiTicks == 1 )
				{
					OpenSteer::enableAnnotation = ( m_bEnableAnnotation == 1 );
				}
				OpenSteer::Plugin::selectedPlugin->update( fCurrentAccumTime, this->m_kUpdatePeriod.GetPeriodTime() );
				fCurrentAccumTime += this->m_kUpdatePeriod.GetPeriodTime();
				--uiTicks;
			}
		}
		else
		{
			OpenSteer::enableAnnotation = ( m_bEnableAnnotation == 1 );
			float fAccumDeltaTime = this->m_kUpdatePeriod.GetDeltaTime( uiTicks );
			OpenSteer::Plugin::selectedPlugin->update( fCurrentAccumTime + fAccumDeltaTime, fAccumDeltaTime );
		}

		this->m_kUpdateClock.update();
		const osScalar postUpdateElapsedTime = m_kUpdateClock.getElapsedRealTime();
		const osScalar fUpdateCPUTime = postUpdateElapsedTime - preUpdateElapsedTime; 
		this->m_fUpdateCPUTime += fUpdateCPUTime;
		this->m_fUpdateCPUTime *= 0.5f;

	}
}

//-----------------------------------------------------------------------------
void Application::redrawSelectedPlugin (const float currentTime,
						   const float elapsedTime )
{
	if( NULL == OpenSteer::Plugin::selectedPlugin )
	{
		return;
	}
	OpenSteer::Plugin::selectedPlugin->redraw (currentTime, elapsedTime);
}

//-----------------------------------------------------------------------------
void Application::drawProfile (const float currentTime,
				  const float elapsedTime)
{
#if EDUNET_HAVE_PROFILE
	Prof_update( this->m_bUpdateCPUProfile );
//	Prof_set_report_mode(Prof_CALL_GRAPH);
	Prof_set_report_mode(Prof_HIERARCHICAL_TIME);
	const float tw = OpenSteer::drawGetWindowWidth();
	const float th = OpenSteer::drawGetWindowHeight();
	if( ( 0 != this->m_bShowCPUProfile ) )
	{
		OpenSteer::profileDraw( 10, 300, 550, 500, -16, 2, tw, th );
	}
	if( ( 0 != this->m_bShowCPUProfileGraph ) )
	{
		OpenSteer::profileDrawGraph( 10.0, 350.0, 4.0, 8.0, tw, th );
	}
#endif
}
