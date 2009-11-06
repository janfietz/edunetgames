#ifndef __EDUNETAPPLICATION_H__
#define	__EDUNETAPPLICATION_H__


//-----------------------------------------------------------------------------
#include "EduNet/common/EduNetCommon.h"
#include "EduNet/templates/TUpdatePeriod.h"

#include "glui/GL/glui.h"

namespace EduNet	{

//-----------------------------------------------------------------------------
class Application
{
public:
	Application( void );
	virtual ~Application( void );

	void addGuiElements( GLUI *glui );

	// do a simulation update for the currently selected plug-in
	void updateSelectedPlugin (const float currentTime,
		const float elapsedTime);

	// redraw graphics for the currently selected plug-in
	void redrawSelectedPlugin (const float currentTime,
		const float elapsedTime);

	float m_fSimulationFPS;
	float m_fTimeFactor;
	int m_bFixedSimulationFPS;
	int m_bEnableAnnotation;

private:

	EduNet::UpdatePeriodFloat m_kUpdatePeriod;

};

}

#endif // __EDUNETAPPLICATION_H__