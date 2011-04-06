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

#include "EmptyPlugin.h"

#include "EduNetCommon/EduNetDraw.h"
#include "EduNetProfile/GraphPlot.h"

#include <math.h>

#include "wx/wx.h"
#include "wx/statbox.h"
#include "wx/panel.h"

OpenSteer::InstanceTracker EduNet::EmptyVehicle::ms_kInstanceCount;


using namespace EduNet;
//-----------------------------------------------------------------------------
void EmptyPlugin::initGui( void* pkUserdata )
{
	GLUI* glui = ::getRootGLUI();
	GLUI_Panel* pluginPanel = static_cast<GLUI_Panel*>( pkUserdata );
//	glui->add_statictext_to_panel( pluginPanel, "no options" );
	glui->add_checkbox_to_panel( pluginPanel, "Show Motionstate", &this->m_bShowMotionStatePlot);
	glui->add_checkbox_to_panel( pluginPanel, "Show SamplePlot", &this->m_bShowSamplePlot);
}
//-----------------------------------------------------------------------------
wxWindow* EmptyPlugin::prepareGui( wxWindow* parent )
{
	wxWindow* window = new wxWindow(parent, wxID_ANY);
	wxSizer* sizer = new wxStaticBoxSizer( wxVERTICAL, window, this->pluginName() );
	window->SetSizer(sizer);

	wxWindow* subwindow = BaseClass::prepareGui( window );
	return window;
}
//-----------------------------------------------------------------------------
void EmptyPlugin::open (void)
{
	m_kVehicle.reset();
	SimpleVehicle::setSelectedVehicle( &m_kVehicle );
	m_kVehicles.push_back( &m_kVehicle );

	m_pCamera = ET_NEW OpenSteer::CameraPlugin();
	addPlugin( m_pCamera );
	
	m_pGrid = ET_NEW OpenSteer::GridPlugin();
	addPlugin( m_pGrid );

	// initialize camera
	m_pCamera->init2dCamera( m_kVehicle );
	
	Camera& cam = m_pCamera->accessCamera();
	cam.setPosition (
		10,
		CameraPlugin::camera2dElevation,
		10);
	cam.fixedPosition.set( 40, 40, 40 );
}
//-----------------------------------------------------------------------------
void EmptyPlugin::close (void)
{
	m_pCamera = NULL;
	m_pGrid = NULL;
	removeAllPlugins();	
}

//-----------------------------------------------------------------------------
void EmptyPlugin::update (const float currentTime, const float elapsedTime)
{
	if( false == this->isEnabled() )
	{
		return;
	}
	AbstractVehicleGroup kVehicles( m_kVehicles );
	kVehicles.update( currentTime, elapsedTime );
	if( 0 == m_bShowMotionStatePlot )
	{
		return;
	}

	AbstractVehicle* pVehicle = SimpleVehicle::getSelectedVehicle();
	if( pVehicle != NULL )
	{
		// update motion state plot
		this->m_kMotionStateProfile.recordUpdate( pVehicle, currentTime, elapsedTime );

		m_pCamera->setCameraTarget( pVehicle );
		m_pGrid->setGridCenter( pVehicle->position() );
	}

	BaseClass::update( currentTime, elapsedTime);
}

//-----------------------------------------------------------------------------
void EmptyPlugin::redraw (OpenSteer::AbstractRenderer* pRenderer, 
	const float currentTime, const float elapsedTime)
{
	if( false == this->isVisible() )
	{
		return;
	}
	BaseClass::redraw(pRenderer, currentTime, elapsedTime);	

	AbstractVehicleGroup kVehicles( m_kVehicles );
	kVehicles.redraw(pRenderer, currentTime, elapsedTime );

	if( 0 != this->m_bShowSamplePlot )
	{
		const float fGraphHeight = 175;
		// draw a test graph
		{
			Profile::GraphValues kValues;
			const size_t uiMaxRecords = kValues.getMaxRecords();
			float fX(0), fY(0);
			for( size_t i = 0; i < uiMaxRecords; ++i )
			{
				fY = fX * fX;
				fY = sinf( fX );
				//		fY = sinf( fX ) * ( ( 0.5f * fX ) * ( 0.5f * fX ) );
				kValues.addValue( fX, fY );
				fX += 0.10f;
			}
			Profile::GraphPlot kPlot;
			kPlot.draw( pRenderer, kValues, 50, 175, 300, fGraphHeight );
		}

		// draw a test graph
		{
			Profile::GraphValuesArray kValuesArray;

			for( size_t i = 0; i < 3; ++i )
			{
				Profile::GraphValues& kValues = kValuesArray.accessValues(i);
				const size_t uiMaxRecords = kValues.getMaxRecords();
				float fX(0), fY(0);
				for( size_t j = 0; j < uiMaxRecords; ++j )
				{
					fY = fX * fX;
					switch( i )
					{
					case(0):
						fY = sinf( fX ) * fX;
						break;
					case(1):
						fY = cos( fX ) * fX;
						break;
					case(2):
						fY = ( sinf( fX ) * ( ( 0.5f * fX ) * ( 0.5f * fX ) ) );
						break;
					}
					kValues.addValue( fX, fY );
					fX += 0.10f;
				}
			}
			Profile::GraphPlot kPlot;
			kPlot.draw( pRenderer, kValuesArray, 50, 175 + fGraphHeight + 20, 300, fGraphHeight );
		}
	}
	if( 0 != this->m_bShowMotionStatePlot )
	{
		// draw motion state plot
		if( NULL != SimpleVehicle::getSelectedVehicle() )
		{
			this->m_kMotionStateProfile.draw( pRenderer, currentTime );
		}
	}
	
}


