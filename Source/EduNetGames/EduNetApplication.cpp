#include "EduNetApplication.h"

#include "EduNet/options/EduNetOptions.h"
#include "EduNetGames.h"


using namespace EduNet;
using namespace OpenSteer;


//-----------------------------------------------------------------------------
namespace
{
	int pluginSelection=0;
	int pluginIndex=0;

	GLUI_Listbox* pluginList = NULL;
}



void setDefaultSettings()
{

}

void	setDefaultSettingsAndSync()
{
	setDefaultSettings();
//	glui->sync_live();
}

void gluiNextPlugin()
{
	OpenSteer::OpenSteerDemo::selectNextPlugin();
}

void gluiSelectPlugin()
{
	// update the index first
	AbstractPlugin* pi = Plugin::getPluginAt( pluginList->orig_value );
	if( OpenSteerDemo::selectedPlugin != pi )
	{
		int numPlugins = Plugin::getNumPlugins();
		for (int i = 0; i < numPlugins; i++)
		{
			AbstractPlugin* piList = Plugin::getPluginAt(i);
			if( piList == OpenSteerDemo::selectedPlugin )
			{
				pluginSelection = i;
				pluginList->do_selection( i );
				break;
			}
		}

	}
	

	if( pluginSelection != pluginIndex )
	{
		OpenSteerDemo::selectPluginByIndex( pluginSelection );	
		pluginIndex = pluginSelection;
	}
}

//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
Application::Application( void ):
m_fSimulationFPS(60.0f),
m_fTimeFactor(1.0f),
m_bFixedSimulationFPS(1),
m_bEnableAnnotation(0)
{
	setDefaultSettings();
	this->m_kUpdatePeriod.SetPeriodFrequency( m_fSimulationFPS );
}

//-----------------------------------------------------------------------------
Application::~Application( void )
{

}

//-----------------------------------------------------------------------------
void Application::addGuiElements( GLUI* glui )
{
	glui->add_statictext("Plugins");
	pluginList =
		glui->add_listbox("", &pluginSelection);//, -1, gluiSelectPlugin);

	int numPlugins = Plugin::getNumPlugins();
	for (int i = 0; i < numPlugins; i++)
	{
		AbstractPlugin* pi = Plugin::getPluginAt(i);
		const char* s = pi->name();
		pluginList->add_item(i, s);
	}


	glui->add_button("Next Plugin", 0,(GLUI_Update_CB)gluiNextPlugin);

	glui->add_separator();

	GLUI_Spinner* timefactorSpinner =
		glui->add_spinner("Timefactor", GLUI_SPINNER_FLOAT, &this->m_fTimeFactor);
	timefactorSpinner->set_float_limits(0.001f, 10.0f);

	GLUI_Spinner* simulationFPSSpinner =
		glui->add_spinner("Simulation FPS", GLUI_SPINNER_FLOAT, &this->m_fSimulationFPS);
	simulationFPSSpinner->set_float_limits(5.0f, 200.0f);
	glui->add_checkbox("Fixed Timestep", &this->m_bFixedSimulationFPS);
	glui->add_separator();
	glui->add_checkbox("Enable Annotation", &this->m_bEnableAnnotation);
}

//-----------------------------------------------------------------------------
void Application::updateSelectedPlugin (const float currentTime,
						   const float elapsedTime )
{
	gluiSelectPlugin();


	// opensteer demo options update
	OpenSteer::enableAnnotation = false;

	AbstractPlugin* selectedPlugin = OpenSteerDemo::selectedPlugin;
	if( NULL == selectedPlugin )
	{
		return;
	}

	// fixed timestep implementation
	this->m_kUpdatePeriod.SetPeriodFrequency( this->m_fSimulationFPS );
	float fCurrentAccumTime = this->m_kUpdatePeriod.GetAccumTime();
	float fModifiedDeltaTime = elapsedTime * m_fTimeFactor;
	size_t uiTicks = this->m_kUpdatePeriod.UpdateDeltaTime( fModifiedDeltaTime );
	if( true == ( this->m_bFixedSimulationFPS == 1 ) )
	{
		while( uiTicks > 0 )
		{
			if( uiTicks == 1 )
			{
				OpenSteer::enableAnnotation = ( m_bEnableAnnotation == 1 );
			}
			selectedPlugin->update( fCurrentAccumTime, this->m_kUpdatePeriod.GetPeriodTime() );
			fCurrentAccumTime += this->m_kUpdatePeriod.GetPeriodTime();
			--uiTicks;
		}
	}
	else
	{
		OpenSteer::enableAnnotation = ( m_bEnableAnnotation == 1 );
		float fAccumDeltaTime = this->m_kUpdatePeriod.GetDeltaTime( uiTicks );
		selectedPlugin->update( fCurrentAccumTime + fAccumDeltaTime, fAccumDeltaTime );
	}
}

//-----------------------------------------------------------------------------
void Application::redrawSelectedPlugin (const float currentTime,
						   const float elapsedTime )
{
	AbstractPlugin* selectedPlugin = OpenSteerDemo::selectedPlugin;
	if( NULL == selectedPlugin )
	{
		return;
	}
	selectedPlugin->redraw (currentTime, elapsedTime);
}

