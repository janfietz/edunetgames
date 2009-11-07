#ifndef __EDUNETAPPLICATION_H__
#define	__EDUNETAPPLICATION_H__


//-----------------------------------------------------------------------------
#include "EduNet/common/EduNetCommon.h"
#include "EduNet/templates/TUpdatePeriod.h"

// glui class forwarding
class GLUI;
class GLUI_Control;
class GLUI_Listbox;
class GLUI_StaticText;
class GLUI_EditText;
class GLUI_Panel;
class GLUI_Spinner;
class GLUI_RadioButton;
class GLUI_RadioGroup;
class GLUI_Glut_Window;
class GLUI_TreePanel;
class GLUI_Scrollbar;
class GLUI_List;

class Arcball;


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

	void onPluginSelected( OpenSteer::AbstractPlugin* pkPlugin );

	GLUI* getAppGui( void ) const;
	GLUI_Panel* getPluginPanel( void ) const;

	float m_fSimulationFPS;
	float m_fTimeFactor;
	int m_bFixedSimulationFPS;
	int m_bEnableAnnotation;

private:

	EduNet::UpdatePeriodFloat m_kUpdatePeriod;

};

}

#endif // __EDUNETAPPLICATION_H__