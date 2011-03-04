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

#define GL_TIMER_ID 242
namespace EduNet
{
	BEGIN_EVENT_TABLE(PlayerGlCanvas, wxGLCanvas)
		EVT_TIMER(GL_TIMER_ID,  PlayerGlCanvas::onTimer)
		EVT_PAINT(PlayerGlCanvas::render)
		EVT_SIZE(PlayerGlCanvas::resized)
		EVT_KEY_DOWN(PlayerGlCanvas::onKeyDown)
		EVT_KEY_UP(PlayerGlCanvas::onKeyUp)
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
		m_pPlugin(NULL)
	{
		m_context = new wxGLContext(this);		

		this->m_Timer.Start(1);
		initGL();
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

		wxGLCanvas::SetCurrent(*this->m_context);
		wxPaintDC(this);

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

		float fRatio = 1.0f * viewPort.tw/viewPort.th;
		gluPerspective(45.0f, fRatio, 1, 1000);

		
	}

	//-----------------------------------------------------------------------------
	void PlayerGlCanvas::onKeyUp(wxKeyEvent& event)
	{
		event.Skip();
	}

	//-----------------------------------------------------------------------------
	void PlayerGlCanvas::onKeyDown(wxKeyEvent& event)
	{
		event.Skip();
	}
}

