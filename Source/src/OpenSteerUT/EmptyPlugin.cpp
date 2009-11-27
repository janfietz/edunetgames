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
#include "EduNetApplication/EduNetGames.h"
#include "OpenSteerUT/CameraPlugin.h"
#include "OpenSteerUT/GridPlugin.h"

#include <math.h>


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
void EmptyPlugin::open (void)
{
	m_kVehicle.reset();
	SimpleVehicle::selectedVehicle = &m_kVehicle;
	m_kVehicles.push_back( &m_kVehicle );

	// initialize camera
	CameraPlugin::init2dCamera( *SimpleVehicle::selectedVehicle );
	Camera::camera.setPosition (
		10,
		CameraPlugin::camera2dElevation,
		10);
	Camera::camera.fixedPosition.set( 40, 40, 40 );
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
	if( OpenSteer::SimpleVehicle::selectedVehicle != NULL )
	{
		// update motion state plot
		this->m_kMotionStateProfile.recordUpdate( OpenSteer::SimpleVehicle::selectedVehicle, currentTime, elapsedTime );
	}
}

//-----------------------------------------------------------------------------
void EmptyPlugin::redraw (const float currentTime, const float elapsedTime)
{
	if( false == this->isVisible() )
	{
		return;
	}
	if( NULL != SimpleVehicle::selectedVehicle )
	{
		// update camera, tracking test vehicle
		CameraPlugin::updateCamera (currentTime, elapsedTime, *SimpleVehicle::selectedVehicle );
		// draw "ground plane"
		GridPlugin::gridUtility( SimpleVehicle::selectedVehicle->position() );
	}

	AbstractVehicleGroup kVehicles( m_kVehicles );
	kVehicles.redraw( currentTime, elapsedTime );

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
			kPlot.draw( kValues, 50, 175, 300, fGraphHeight );
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
			kPlot.draw( kValuesArray, 50, 175 + fGraphHeight + 20, 300, fGraphHeight );
		}
	}
	if( 0 != this->m_bShowMotionStatePlot )
	{
		// draw motion state plot
		if( NULL != SimpleVehicle::selectedVehicle )
		{
			this->m_kMotionStateProfile.draw( currentTime );
		}
	}

}

