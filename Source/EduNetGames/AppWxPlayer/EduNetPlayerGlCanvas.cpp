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

#include "EduNetPlayerGlCanvas.h"
#include "EduNetApplication/EduNetGames.h"

#include <GL/glut.h>

#include "OpenSteer/AbstractPlugin.h"
#include "OpenSteerUT/OpensteerUT.h"

#include "OpenSteer/SimpleVehicle.h"
#include "OpenSteer/Draw.h"
#include "mgf/common/KeyCodes.h"

#define GL_TIMER_ID 242
namespace EduNet
{
	BEGIN_EVENT_TABLE(PlayerGlCanvas, wxGLCanvas)
		EVT_TIMER(GL_TIMER_ID,  PlayerGlCanvas::onTimer)
		EVT_PAINT(PlayerGlCanvas::render)
		EVT_SIZE(PlayerGlCanvas::resized)
		EVT_KEY_DOWN(PlayerGlCanvas::onKeyDown)
		EVT_KEY_UP(PlayerGlCanvas::onKeyUp)
		EVT_CHAR(PlayerGlCanvas::onCharInput)
	END_EVENT_TABLE()
//-----------------------------------------------------------------------------
PlayerGlCanvas::PlayerGlCanvas(wxWindow *parent,
							   wxWindowID id,
							   const wxPoint &pos,
							   const wxSize &size,
							   long style,
							   const wxString& name ):
	wxGLCanvas(parent, id, 0, pos, size,style ),
		m_Timer(this, GL_TIMER_ID),
		m_iTicks(0),
		m_pPlugin(NULL),
		m_context(NULL)
	{
		this->m_Timer.Start(1);
		
	}

	PlayerGlCanvas::~PlayerGlCanvas()
	{
		delete this->m_context;
	}

//-----------------------------------------------------------------------------
	void PlayerGlCanvas::onTimer(wxTimerEvent& evt)
	{
		++this->m_iTicks;
		this->Refresh();
		this->Update();
	}
	//-----------------------------------------------------------------------------
	void PlayerGlCanvas::resized(wxSizeEvent& evt)
	{
		initGL();
		Refresh();
	}

	//-----------------------------------------------------------------------------
	void PlayerGlCanvas::render(wxPaintEvent& evt)
	{
		if(false == IsShown())
		{
			return;
		}

		
		wxGLContext* pContext = this->accessContext();
		wxPaintDC dc(this);

		glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity ();
		// set default camera position
		gluLookAt (0, 15, 0,
			0,  0,  0,
			0,0,1);

		OpenSteer::OpenGLRenderer kRenderer;

		if (m_pPlugin != NULL)
		{
			m_Clock.update ();
						
			updatePlugin( m_pPlugin, m_Clock.getTotalSimulationTime (),
				m_Clock.getElapsedSimulationTime () );

			// nearest mouse (to be highlighted)
			//SimpleVehicle::setNearestMouseVehicle( GameDemo::vehicleNearestToMouse() );

			
			kRenderer.SetPlugin( m_pPlugin );
			m_pPlugin->redraw( &kRenderer, m_Clock.getTotalSimulationTime (),
				m_Clock.getElapsedSimulationTime () );
		}	

		glFlush(); 
		SwapBuffers();
		// check for errors in drawing module, if so report and exit
		kRenderer.checkForDrawError ( "GameDemo::updateSimulationAndRedraw" );
	}

	//-----------------------------------------------------------------------------
	void PlayerGlCanvas::updatePlugin(OpenSteer::AbstractPlugin* pkPlugin, float totalSimTime, float elapsedSimTime)
	{		
		// if no vehicle is selected, and some exist, select the first one
		if ( OpenSteer::SimpleVehicle::getSelectedVehicle() == NULL)
		{
			const OpenSteer::AVGroup& vehicles = m_pPlugin->allVehicles();
			if (vehicles.size() > 0) 
			{
				OpenSteer::SimpleVehicle::setSelectedVehicle( vehicles.front() );
			}
		}

		osAbstractUpdated* pkUpdatedPlugin = dynamic_cast<osAbstractUpdated*>(m_pPlugin);
		assert( NULL != pkUpdatedPlugin );
		pkUpdatedPlugin->update( totalSimTime,elapsedSimTime );
	}

	

	void PlayerGlCanvas::setPlugin( OpenSteer::AbstractPlugin* pPlugin )
	{
		this->m_pPlugin = pPlugin;
	}

	// initialize GL mode settings
	void PlayerGlCanvas::initGL ( void )
	{
		// background = dark gray
		glClearColor ( 0.3f, 0.3f, 0.3f, 0 );
	
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

		ViewPort& viewPort = OpenSteerUTData::g_openSteerUTDataPtr->viewPort;
		viewPort.tx = 0;
		viewPort.ty = 0;
		viewPort.tw = GetSize().x;
		viewPort.th = GetSize().y;
		glViewport(viewPort.tx, viewPort.ty, (GLint)viewPort.tw, (GLint)viewPort.th); 


		glMatrixMode(GL_PROJECTION);	
		glLoadIdentity();

		float fRatio = (float)viewPort.tw/(float)viewPort.th;
		gluPerspective(45.0f, fRatio, 1, 1000);

		
	}


	// mapping of wx key codes to mgf codes
	struct KeyCodePairT
	{
		wxKeyCode wxKC;
		mgf::EKey keyCode;
	};


	static const KeyCodePairT KeyCodes[]=
	{
		{ WXK_BACK,             mgf::KEY_BACKSPACE },
		{ WXK_TAB,              mgf::KEY_TAB },
		{ WXK_RETURN,           mgf::KEY_RETURN },
		{ WXK_ESCAPE,           mgf::KEY_ESCAPE },
		{ WXK_SPACE,            mgf::KEY_SPACE },
		{ (wxKeyCode)48,        mgf::KEY_0 },
		{ (wxKeyCode)49,        mgf::KEY_1 },
		{ (wxKeyCode)50,        mgf::KEY_2 },
		{ (wxKeyCode)51,        mgf::KEY_3 },
		{ (wxKeyCode)52,        mgf::KEY_4 },
		{ (wxKeyCode)53,        mgf::KEY_5 },
		{ (wxKeyCode)54,        mgf::KEY_6 },
		{ (wxKeyCode)55,        mgf::KEY_7 },
		{ (wxKeyCode)56,        mgf::KEY_8 },
		{ (wxKeyCode)57,        mgf::KEY_9 },
		{ (wxKeyCode)65,        mgf::KEY_A },
		{ (wxKeyCode)66,        mgf::KEY_B },
		{ (wxKeyCode)67,        mgf::KEY_C },
		{ (wxKeyCode)68,        mgf::KEY_D },
		{ (wxKeyCode)69,        mgf::KEY_E },
		{ (wxKeyCode)70,        mgf::KEY_F },
		{ (wxKeyCode)71,        mgf::KEY_G },
		{ (wxKeyCode)72,        mgf::KEY_H },
		{ (wxKeyCode)73,        mgf::KEY_I },
		{ (wxKeyCode)74,        mgf::KEY_J },
		{ (wxKeyCode)75,        mgf::KEY_K },
		{ (wxKeyCode)76,        mgf::KEY_L },
		{ (wxKeyCode)77,        mgf::KEY_M },
		{ (wxKeyCode)78,        mgf::KEY_N },
		{ (wxKeyCode)79,        mgf::KEY_O },
		{ (wxKeyCode)80,        mgf::KEY_P },
		{ (wxKeyCode)81,        mgf::KEY_Q },
		{ (wxKeyCode)82,        mgf::KEY_R },
		{ (wxKeyCode)83,        mgf::KEY_S },
		{ (wxKeyCode)84,        mgf::KEY_T },
		{ (wxKeyCode)85,        mgf::KEY_U },
		{ (wxKeyCode)86,        mgf::KEY_V },
		{ (wxKeyCode)87,        mgf::KEY_W },
		{ (wxKeyCode)88,        mgf::KEY_X },
		{ (wxKeyCode)89,        mgf::KEY_Y },
		{ (wxKeyCode)90,        mgf::KEY_Z },
		{ WXK_DELETE,           mgf::KEY_DELETE },
		//{ WXK_START,           mgf:: },
		//{ WXK_LBUTTON,         mgf:: },
		//{ WXK_RBUTTON,         mgf:: },
		//{ WXK_CANCEL,          mgf:: },
		//{ WXK_MBUTTON,         mgf:: },
		//{ WXK_CLEAR,           mgf:: },
		{ WXK_SHIFT,            mgf::KEY_LSHIFT },
		{ WXK_ALT,              mgf::KEY_LMENU },
		{ WXK_CONTROL,          mgf::KEY_LCONTROL },
		//{ WXK_MENU,            mgf:: },
		{ WXK_PAUSE,            mgf::KEY_PAUSE },
		{ WXK_CAPITAL,          mgf::KEY_CAPITAL },
		{ WXK_END,              mgf::KEY_END },
		{ WXK_HOME,             mgf::KEY_HOME },
		{ WXK_LEFT,             mgf::KEY_LEFT },
		{ WXK_UP,               mgf::KEY_UP },
		{ WXK_RIGHT,            mgf::KEY_RIGHT },
		{ WXK_DOWN,             mgf::KEY_DOWN },
		//{ WXK_SELECT,          mgf:: },
		//{ WXK_PRINT,           mgf:: },
		//{ WXK_EXECUTE,         mgf:: },
		//{ WXK_SNAPSHOT,        mgf:: },
		{ WXK_INSERT,           mgf::KEY_INSERT },
		//{ WXK_HELP,            mgf:: },
		{ WXK_NUMPAD0,          mgf::KEY_NUMPAD0 },
		{ WXK_NUMPAD1,          mgf::KEY_NUMPAD1 },
		{ WXK_NUMPAD2,          mgf::KEY_NUMPAD2 },
		{ WXK_NUMPAD3,          mgf::KEY_NUMPAD3 },
		{ WXK_NUMPAD4,          mgf::KEY_NUMPAD4 },
		{ WXK_NUMPAD5,          mgf::KEY_NUMPAD5 },
		{ WXK_NUMPAD6,          mgf::KEY_NUMPAD6 },
		{ WXK_NUMPAD7,          mgf::KEY_NUMPAD7 },
		{ WXK_NUMPAD8,          mgf::KEY_NUMPAD8 },
		{ WXK_NUMPAD9,          mgf::KEY_NUMPAD9 },
		//{ WXK_MULTIPLY,        mgf:: },
		//{ WXK_ADD,             mgf:: },
		{ WXK_SEPARATOR,        mgf::KEY_COMMA },
		{ WXK_SUBTRACT,         mgf::KEY_MINUS },
		{ WXK_DECIMAL,          mgf::KEY_PERIOD },
		{ WXK_DIVIDE,           mgf::KEY_SLASH },
		{ WXK_F1,               mgf::KEY_F1 },
		{ WXK_F2,               mgf::KEY_F2 },
		{ WXK_F3,               mgf::KEY_F3 },
		{ WXK_F4,               mgf::KEY_F4 },
		{ WXK_F5,               mgf::KEY_F5 },
		{ WXK_F6,               mgf::KEY_F6 },
		{ WXK_F7,               mgf::KEY_F7 },
		{ WXK_F8,               mgf::KEY_F8 },
		{ WXK_F9,               mgf::KEY_F9 },
		{ WXK_F10,              mgf::KEY_F10 },
		{ WXK_F11,              mgf::KEY_F11 },
		{ WXK_F12,              mgf::KEY_F12 },
		{ WXK_F13,              mgf::KEY_F13 },
		{ WXK_F14,              mgf::KEY_F14 },
		{ WXK_F15,              mgf::KEY_F15 },
		{ WXK_NUMLOCK,          mgf::KEY_NUMLOCK },
		{ WXK_SCROLL,           mgf::KEY_SCROLL },
		{ WXK_PAGEUP,           mgf::KEY_PGUP },
		{ WXK_PAGEDOWN,         mgf::KEY_PGDN },
		//{ WXK_NUMPAD_SPACE,    mgf:: },
		//{ WXK_NUMPAD_TAB,      mgf:: },
		{ WXK_NUMPAD_ENTER,     mgf::KEY_NUMPADENTER },
		//{ WXK_NUMPAD_F1,       mgf:: },
		//{ WXK_NUMPAD_F2,       mgf:: },
		//{ WXK_NUMPAD_F3,       mgf:: },
		//{ WXK_NUMPAD_F4,       mgf:: },
		//{ WXK_NUMPAD_HOME,     mgf:: },
		//{ WXK_NUMPAD_LEFT,     mgf:: },
		//{ WXK_NUMPAD_UP,       mgf:: },
		//{ WXK_NUMPAD_RIGHT,    mgf:: },
		//{ WXK_NUMPAD_DOWN,     mgf:: },
		//{ WXK_NUMPAD_PAGEUP,   mgf:: },
		//{ WXK_NUMPAD_PAGEDOWN, mgf:: },
		//{ WXK_NUMPAD_END,      mgf:: },
		//{ WXK_NUMPAD_BEGIN,    mgf:: },
		//{ WXK_NUMPAD_INSERT,   mgf:: },
		//{ WXK_NUMPAD_DELETE,   mgf:: },
		//{ WXK_NUMPAD_EQUAL,    mgf:: },
		{ WXK_NUMPAD_MULTIPLY,  mgf::KEY_MULTIPLY },
		{ WXK_NUMPAD_ADD,       mgf::KEY_ADD },
		{ WXK_NUMPAD_SEPARATOR, mgf::KEY_NUMPADCOMMA },
		{ WXK_NUMPAD_SUBTRACT,  mgf::KEY_SUBTRACT },
		{ WXK_NUMPAD_DECIMAL,   mgf::KEY_DECIMAL },
		{ WXK_NUMPAD_DIVIDE,    mgf::KEY_DIVIDE },
		{ WXK_WINDOWS_LEFT,     mgf::KEY_LWIN },
		{ WXK_WINDOWS_RIGHT,    mgf::KEY_RWIN },
		//{ WXK_WINDOWS_MENU ,   mgf:: },
		//{ WXK_COMMAND,         mgf:: },
		//{ (wxKeyCode)0, (mgf::EKey)0 }
	};

	//-----------------------------------------------------------------------------
	void PlayerGlCanvas::onKeyUp(wxKeyEvent& event)
	{
		for (int KeyCodeNr=0; KeyCodes[KeyCodeNr].wxKC!=0; KeyCodeNr++)
		{
			if (KeyCodes[KeyCodeNr].wxKC==event.GetKeyCode())
			{				
				if (m_pPlugin != NULL)
				{
					m_pPlugin->handleFunctionKeys( KeyCodes[KeyCodeNr].keyCode );
				}				
				break;
			}
		}
		event.Skip();
	}

	//-----------------------------------------------------------------------------
	void PlayerGlCanvas::onKeyDown(wxKeyEvent& event)
	{
		for (int KeyCodeNr=0; KeyCodes[KeyCodeNr].wxKC!=0; KeyCodeNr++)
		{
			if (KeyCodes[KeyCodeNr].wxKC==event.GetKeyCode())
			{				
				if (m_pPlugin != NULL)
				{
					m_pPlugin->handleFunctionKeys( KeyCodes[KeyCodeNr].keyCode );
				}				
				break;
			}
		}
		event.Skip();
	}

	wxGLContext* PlayerGlCanvas::accessContext()
	{
		if (NULL == m_context)
		{
			m_context = new wxGLContext(this);
			m_context->SetCurrent(*this);
			initGL();
		}
		return m_context;
	}

	void PlayerGlCanvas::onCharInput( wxKeyEvent& event )
	{
		if (m_pPlugin != NULL)
		{
			m_pPlugin->handleFunctionKeys( event.GetKeyCode() );
		}
		event.Skip();
	}
}

