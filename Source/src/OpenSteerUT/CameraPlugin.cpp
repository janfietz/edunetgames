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
	//GLUI* glui = ::getRootGLUI();
	//GLUI_Panel* pluginPanel = static_cast<GLUI_Panel*>( pkUserdata );
	//GLUI_Spinner* lookdownDistanceSpinner =
	//	glui->add_spinner_to_panel( pluginPanel, "Look Down Distance", GLUI_SPINNER_FLOAT, &m_Camera.lookdownDistance);
	//lookdownDistanceSpinner->set_float_limits(0.1f, 50.0f);
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
	position3dCamera (selected, distance, elevation);
	m_Camera.fixedDistDistance = distance;
	m_Camera.fixedDistVOffset = elevation;
	m_Camera.mode = Camera::cmFixedDistanceOffset;
}

//-----------------------------------------------------------------------------
void 
CameraPlugin::init2dCamera( AbstractLocalSpace& selected )
{
	init2dCamera( selected, cameraTargetDistance, camera2dElevation );
}

//-----------------------------------------------------------------------------
void 
CameraPlugin::init2dCamera( AbstractLocalSpace& selected,
										float distance,
										float elevation )
{
	position2dCamera (selected, distance, elevation);
	m_Camera.fixedDistDistance = distance;
	m_Camera.fixedDistVOffset = elevation;
	m_Camera.mode = Camera::cmFixedDistanceOffset;
}

//-----------------------------------------------------------------------------
void 
CameraPlugin::position3dCamera( AbstractLocalSpace& selected )
{
	position3dCamera( selected, cameraTargetDistance, camera2dElevation );
}

//-----------------------------------------------------------------------------
void 
CameraPlugin::position3dCamera( AbstractLocalSpace& selected,
											float distance,
											float /*elevation*/ )
{
	if (&selected)
	{
		Vec3 direction = selected.position() - m_Camera.position();
		float fDirection = direction.length();
		if( fDirection > 0 )
		{
			direction /= fDirection;
			m_Camera.setForward( direction );
			m_Camera.regenerateOrthonormalBasisUF( m_Camera.forward() );
		}

		const Vec3 behind = selected.forward() * -distance;
		m_Camera.setPosition (selected.position() + behind);
		m_Camera.target = selected.position();
	}
	else
	{
		Vec3 forward = -Vec3::up + Vec3::forward;
		forward = forward.normalized();
		const Vec3 behind = forward * -distance;
		m_Camera.setPosition( behind );
		m_Camera.target = Vec3::zero;
	}

	m_Camera.doNotSmoothNextMove ();	
}

//-----------------------------------------------------------------------------
void 
CameraPlugin::position2dCamera( AbstractLocalSpace& selected )
{
	float distance = cameraTargetDistance;
	float elevation = camera2dElevation;
	if( &selected )
	{
		Vec3 dir = selected.position() - m_Camera.position();
		if( dir.length() > 0 )
		{
			distance = ::etMin( distance, dir.length() );
		}
	}
	position2dCamera( selected, distance, elevation );
}

//-----------------------------------------------------------------------------
void 
CameraPlugin::position2dCamera( AbstractLocalSpace& selected,
											float distance,
											float elevation )
{
	// position the camera as if in 3d:
	position3dCamera (selected, distance, elevation);

	// then adjust for 3d:
	Vec3 position3d = m_Camera.position();
	position3d.y += elevation;
	m_Camera.setPosition (position3d);
	m_Camera.doNotSmoothNextMove ();	
}

//-----------------------------------------------------------------------------
void CameraPlugin::setCameraTarget ( const AbstractLocalSpace* selected )
{	
	m_Camera.setLocalSpaceToTrack( selected );
}

//-----------------------------------------------------------------------------
void CameraPlugin::redraw( OpenSteer::AbstractRenderer* pRenderer,
	const float currentTime, 
	const float elapsedTime ) 
{ 
	pRenderer->SetCamera( &m_Camera );
	if( false == this->isVisible() )
	{
		return;
	}
	m_Camera.draw( pRenderer, currentTime, elapsedTime);
}

void OpenSteer::CameraPlugin::update( const float currentTime, const float elapsedTime )
{		
	m_Camera.update( currentTime, elapsedTime );
}

void OpenSteer::CameraPlugin::open( void )
{
	m_Camera.reset();
}