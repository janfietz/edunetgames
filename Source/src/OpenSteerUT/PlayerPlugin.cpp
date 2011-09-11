//-----------------------------------------------------------------------------
// Copyright (c) 2011, Jan Fietz, Cyrus Preuss
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

#include "PlayerPlugin.h"

#include "EduNetCommon/EduNetDraw.h"
#include "EduNetProfile/GraphPlot.h"
#include "OpenSteerUT/CameraPlugin.h"
#include "OpenSteerUT/GridPlugin.h"

#include <math.h>

using namespace EduNet;
//-----------------------------------------------------------------------------
void PlayerPlugin::initGui( void* pkUserdata )
{
	GLUI* glui = ::getRootGLUI();
	GLUI_Panel* pluginPanel = static_cast<GLUI_Panel*>( pkUserdata );
//	glui->add_statictext_to_panel( pluginPanel, "no options" );
	glui->add_checkbox_to_panel( pluginPanel, "Show Motionstate", &this->m_bShowMotionStatePlot);
}


//-----------------------------------------------------------------------------
void PlayerPlugin::open (void)
{
	m_kVehicle.reset();
	SimpleVehicle::setSelectedVehicle( &m_kVehicle );
	m_kVehicles.push_back( &m_kVehicle );

	// initialize camera
	CameraPlugin::init2dCamera( *SimpleVehicle::getSelectedVehicle() );
	Camera::accessInstance().setPosition (
		10,
		CameraPlugin::camera2dElevation,
		10);
	Camera::accessInstance().fixedPosition.set( 40, 40, 40 );
}

//-----------------------------------------------------------------------------
void PlayerPlugin::update (const float currentTime, const float elapsedTime)
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
	if( OpenSteer::SimpleVehicle::getSelectedVehicle() != NULL )
	{
		// update motion state plot
		this->m_kMotionStateProfile.recordUpdate( OpenSteer::SimpleVehicle::getSelectedVehicle(), currentTime, elapsedTime );
	}
}

//-----------------------------------------------------------------------------
void PlayerPlugin::redraw (const float currentTime, const float elapsedTime)
{
	if( false == this->isVisible() )
	{
		return;
	}
	if( NULL != SimpleVehicle::getSelectedVehicle() )
	{
		// update camera, tracking test vehicle
		CameraPlugin::updateCamera (currentTime, elapsedTime, *SimpleVehicle::getSelectedVehicle() );
		// draw "ground plane"
		GridPlugin::gridUtility( SimpleVehicle::getSelectedVehicle()->position() );
	}

	AbstractVehicleGroup kVehicles( m_kVehicles );
	kVehicles.redraw( currentTime, elapsedTime );

	if( 0 != this->m_bShowMotionStatePlot )
	{
		// draw motion state plot
		if( NULL != SimpleVehicle::getSelectedVehicle() )
		{
			this->m_kMotionStateProfile.draw( currentTime );
		}
	}

}

