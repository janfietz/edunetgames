// CameraPlugin static code:
// - code which has been extracted from OpenSteerDemo
//-----------------------------------------------------------------------------
//
//
// OpenSteer -- Steering Behaviors for Autonomous Characters
//
// Copyright (c) 2002-2005, Sony Computer Entertainment America
// Original author: Craig Reynolds <craig_reynolds@playstation.sony.com>
//
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//
//
//-----------------------------------------------------------------------------

// CameraPlugin code:
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

#include "CameraPlugin.h"
#include "EduNetCommon/EduNetDraw.h"
#include "OpenSteer/Utilities.h"

using namespace OpenSteer;

//-----------------------------------------------------------------------------
// some camera-related default constants
const float CameraPlugin::camera2dElevation = 6;
const float CameraPlugin::cameraTargetDistance = 10;
const OpenSteer::Vec3 CameraPlugin::cameraTargetOffset( Vec3::up * CameraPlugin::camera2dElevation );
//	0, CameraPlugin::camera2dElevation, 0 );


//-----------------------------------------------------------------------------
void CameraPlugin::initGui( void* pkUserdata )
{
	GLUI* glui = ::getRootGLUI();
	GLUI_Panel* pluginPanel = static_cast<GLUI_Panel*>( pkUserdata );
	GLUI_Spinner* lookdownDistanceSpinner =
		glui->add_spinner_to_panel( pluginPanel, "Look Down Distance", GLUI_SPINNER_FLOAT, &Camera::accessInstance().lookdownDistance);
	lookdownDistanceSpinner->set_float_limits(0.1f, 50.0f);
}


//-----------------------------------------------------------------------------
// set a certain initial camera state used by several plug-ins
void 
CameraPlugin::init3dCamera( AbstractLocalSpace& selected )
{
	init3dCamera( selected, cameraTargetDistance, camera2dElevation );
}

//-----------------------------------------------------------------------------
void 
CameraPlugin::init3dCamera (AbstractLocalSpace& selected,
										float distance,
										float elevation)
{
	CameraPlugin::position3dCamera (selected, distance, elevation);
	OpenSteer::Camera::accessInstance().fixedDistDistance = distance;
	OpenSteer::Camera::accessInstance().fixedDistVOffset = elevation;
	OpenSteer::Camera::accessInstance().mode = Camera::cmFixedDistanceOffset;
}

//-----------------------------------------------------------------------------
void 
CameraPlugin::init2dCamera( AbstractLocalSpace& selected )
{
	CameraPlugin::init2dCamera( selected, cameraTargetDistance, camera2dElevation );
}

//-----------------------------------------------------------------------------
void 
CameraPlugin::init2dCamera( AbstractLocalSpace& selected,
										float distance,
										float elevation )
{
	CameraPlugin::position2dCamera (selected, distance, elevation);
	OpenSteer::Camera::accessInstance().fixedDistDistance = distance;
	OpenSteer::Camera::accessInstance().fixedDistVOffset = elevation;
	OpenSteer::Camera::accessInstance().mode = Camera::cmFixedDistanceOffset;
}

//-----------------------------------------------------------------------------
void 
CameraPlugin::position3dCamera( AbstractLocalSpace& selected )
{
	CameraPlugin::position3dCamera( selected, cameraTargetDistance, camera2dElevation );
}

//-----------------------------------------------------------------------------
void 
CameraPlugin::position3dCamera( AbstractLocalSpace& selected,
											float distance,
											float /*elevation*/ )
{
	// note: the camera has no right to select a vehicle !
	// SimpleVehicle::setSelectedVehicle( &selected );
	if (&selected)
	{
		Vec3 direction = selected.position() - Camera::accessInstance().position();
		float fDirection = direction.length();
		if( fDirection > 0 )
		{
			direction /= fDirection;
			Camera::accessInstance().setForward( direction );
			Camera::accessInstance().regenerateOrthonormalBasisUF( Camera::accessInstance().forward() );
		}

		const Vec3 behind = selected.forward() * -distance;
		OpenSteer::Camera::accessInstance().setPosition (selected.position() + behind);
		OpenSteer::Camera::accessInstance().target = selected.position();
	}
	else
	{
		Vec3 forward = -Vec3::up + Vec3::forward;
		forward = forward.normalized();
		const Vec3 behind = forward * -distance;
		OpenSteer::Camera::accessInstance().setPosition( behind );
		OpenSteer::Camera::accessInstance().target = Vec3::zero;
	}

	Camera::accessInstance().doNotSmoothNextMove ();

	if (&selected)
	{
		CameraPlugin::updateCamera( 0, 0, selected );
	}
}

//-----------------------------------------------------------------------------
void 
CameraPlugin::position2dCamera( AbstractLocalSpace& selected )
{
	float distance = cameraTargetDistance;
	float elevation = camera2dElevation;
	if( &selected )
	{
		Vec3 dir = selected.position() - OpenSteer::Camera::accessInstance().position();
		if( dir.length() > 0 )
		{
			distance = ::etMin( distance, dir.length() );
		}
	}
	CameraPlugin::position2dCamera( selected, distance, elevation );
}

//-----------------------------------------------------------------------------
void 
CameraPlugin::position2dCamera( AbstractLocalSpace& selected,
											float distance,
											float elevation )
{
	// position the camera as if in 3d:
	CameraPlugin::position3dCamera (selected, distance, elevation);

	// then adjust for 3d:
	Vec3 position3d = OpenSteer::Camera::accessInstance().position();
	position3d.y += elevation;
	OpenSteer::Camera::accessInstance().setPosition (position3d);
	Camera::accessInstance().doNotSmoothNextMove ();
	if (&selected)
	{
		CameraPlugin::updateCamera( 0, 0, selected );
	}
}

//-----------------------------------------------------------------------------
// camera updating utility used by several plug-ins
void 
CameraPlugin::updateCamera (const float currentTime,
										const float elapsedTime,
										const AbstractLocalSpace& selected)
{
	OpenSteer::Camera::updateCamera( 
		currentTime, elapsedTime, selected, OpenSteer::Clock::processClock().getPausedState () );
}

// note: the camera plugin does not have an update implementation
//       the camera is updated within the redraw function

//-----------------------------------------------------------------------------
void CameraPlugin::redraw( const float currentTime, const float elapsedTime ) 
{ 
	if( false == this->isVisible() )
	{
		return;
	}
	// selected Pedestrian (user can mouse click to select another)
	const AbstractLocalSpace* selected = SimpleVehicle::getSelectedVehicle();
	if( NULL == selected )
	{
		selected = Camera::getLocalSpaceToTrack();
	}
	// TODO: determine paused state
	const bool simulationPaused = OpenSteer::Clock::processClock().getPausedState();
	// update camera
	if( NULL != selected )
	{
		Camera::updateCamera( currentTime, elapsedTime, *selected, simulationPaused );
	}
	else
	{
// 		OpenSteer::Camera::accessInstance().vehicleToTrack = NULL;
// 		Camera::accessInstance().update( currentTime, elapsedTime, simulationPaused );
	}
}
