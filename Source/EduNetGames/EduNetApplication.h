#ifndef __EDUNETAPPLICATION_H__
#define	__EDUNETAPPLICATION_H__


//-----------------------------------------------------------------------------
#include "EduNetCommon/EduNetCommon.h"
#include "EduNetCommon/TUpdatePeriod.h"



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
	static void _SDMCleanup( void );
	static void _SDMShutdown( void );


	float m_fSimulationFPS;
	float m_fTimeFactor;
	int m_bFixedSimulationFPS;
	int m_bEnableAnnotation;

private:
	Application( void );
	virtual ~Application( void );

	TUpdatePeriod<osScalar, FloatMathLimits> m_kUpdatePeriod;
	OpenSteer::Clock m_kUpdateClock;
	osScalar m_fUpdateCPUTime;

};

}

#endif // __EDUNETAPPLICATION_H__