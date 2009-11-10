#ifndef __EDUNETAPPLICATION_H__
#define	__EDUNETAPPLICATION_H__


//-----------------------------------------------------------------------------
#include "EduNet/common/EduNetCommon.h"
#include "EduNet/templates/TUpdatePeriod.h"



namespace EduNet	{

//-----------------------------------------------------------------------------
class Application
{
public:

	void addGuiElements( GLUI *glui );

	// do a simulation update for the currently selected plug-in
	void updateSelectedPlugin (const float currentTime,
		const float elapsedTime);

	// redraw graphics for the currently selected plug-in
	void redrawSelectedPlugin (const float currentTime,
		const float elapsedTime);

	void onPluginSelected( OpenSteer::AbstractPlugin* pkPlugin );

	static Application& AccessApplication( void );

	static void _SDMInit( void );
	static void _SDMShutdown( void );


	float m_fSimulationFPS;
	float m_fTimeFactor;
	int m_bFixedSimulationFPS;
	int m_bEnableAnnotation;

private:
	Application( void );
	virtual ~Application( void );

	EduNet::UpdatePeriodFloat m_kUpdatePeriod;

};

}

#endif // __EDUNETAPPLICATION_H__