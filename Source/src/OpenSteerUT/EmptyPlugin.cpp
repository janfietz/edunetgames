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
#include "EduNetApplication/EduNetGames.h"


OpenSteer::InstanceTracker EduNet::EmptyVehicle::ms_kInstanceCount;


using namespace EduNet;
//-----------------------------------------------------------------------------
void EmptyPlugin::initGui( void* pkUserdata )
{
	GLUI* glui = ::getRootGLUI();
	GLUI_Panel* pluginPanel = static_cast<GLUI_Panel*>( pkUserdata );
	glui->add_statictext_to_panel( pluginPanel, "no options" );
}


//-----------------------------------------------------------------------------
void EmptyPlugin::open (void)
{
	m_kVehicle.reset();
	SimpleVehicle::selectedVehicle = &m_kVehicle;
	m_kVehicles.push_back( &m_kVehicle );

	// initialize camera
	OpenSteerDemo::init2dCamera( *SimpleVehicle::selectedVehicle );
	OpenSteerDemo::camera.setPosition (
		10,
		OpenSteerDemo::camera2dElevation,
		10);
	OpenSteerDemo::camera.fixedPosition.set( 40, 40, 40 );
}

//-----------------------------------------------------------------------------
void EmptyPlugin::redraw (const float currentTime, const float elapsedTime)
{
	AbstractVehicleGroup kVehicles( m_kVehicles );
	kVehicles.redraw( currentTime, elapsedTime );
	if( NULL != SimpleVehicle::selectedVehicle )
	{
		// update camera, tracking test vehicle
		OpenSteerDemo::updateCamera (currentTime, elapsedTime, *SimpleVehicle::selectedVehicle );
		// draw "ground plane"
		OpenSteerDemo::gridUtility( SimpleVehicle::selectedVehicle->position() );
	}
}