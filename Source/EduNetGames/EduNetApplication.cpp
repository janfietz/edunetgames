#include "EduNetApplication.h"

#include "EduNetOptions.h"
#include "EduNetGames.h"


using namespace EduNet;
using namespace OpenSteer;


//-----------------------------------------------------------------------------
namespace
{
	int pluginSelection=0;
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
	OpenSteer::OpenSteerDemo::selectNextPlugIn();
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
	GLUI_Listbox* testList =
		glui->add_listbox("", &pluginSelection);

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
void Application::updateSelectedPlugIn (const float currentTime,
						   const float elapsedTime )
{
	// opensteer demo options update
	OpenSteer::enableAnnotation = false;

	AbstractPlugIn* selectedPlugIn = OpenSteerDemo::selectedPlugIn;
	if( NULL == selectedPlugIn )
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
			selectedPlugIn->update( fCurrentAccumTime, this->m_kUpdatePeriod.GetPeriodTime() );
			fCurrentAccumTime += this->m_kUpdatePeriod.GetPeriodTime();
			--uiTicks;
		}
	}
	else
	{
		OpenSteer::enableAnnotation = ( m_bEnableAnnotation == 1 );
		float fAccumDeltaTime = this->m_kUpdatePeriod.GetDeltaTime( uiTicks );
		selectedPlugIn->update( fCurrentAccumTime + fAccumDeltaTime, fAccumDeltaTime );
	}
}

//-----------------------------------------------------------------------------
void Application::redrawSelectedPlugIn (const float currentTime,
						   const float elapsedTime )
{
	AbstractPlugIn* selectedPlugIn = OpenSteerDemo::selectedPlugIn;
	if( NULL == selectedPlugIn )
	{
		return;
	}
	selectedPlugIn->redraw (currentTime, elapsedTime);
}

