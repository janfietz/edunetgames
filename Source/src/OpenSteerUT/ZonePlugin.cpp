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

#include "ZonePlugin.h"
#include "EduNetCommon/EduNetDraw.h"

using namespace OpenSteer;

//-----------------------------------------------------------------------------
void initPluginCamera( AbstractPlugin* pkPlugin )
{
	// camera setup
	osAbstractLocalSpace* localSpace = dynamic_cast<osAbstractLocalSpace*>(pkPlugin);
	osAbstractLocalSpace& kLocalSpace = *localSpace;
	CameraPlugin::init2dCamera( kLocalSpace );
	// Camera::accessInstance().mode = Camera::cmFixedDistanceOffset;
	Camera::accessInstance().mode = Camera::cmStraightDown;
	Camera::accessInstance().fixedTarget.set( 0, 0, 0 );
	Camera::accessInstance().fixedPosition.set( 0, 20, 0 );
	Camera::accessInstance().lookdownDistance = 15;
	// make camera jump immediately to new position
	Camera::accessInstance().doNotSmoothNextMove ();
	Camera::accessInstance().update( 0, 0, false );
}

//-----------------------------------------------------------------------------
ZonePlugin::ZonePlugin( bool bAddToRegistry ):
	BaseClass(bAddToRegistry),
	m_kZoneExtent( OS_SCALAR( 10.0 ), OS_SCALAR( 0.0 ), OS_SCALAR( 10.0 ) ),
	m_iSolid(0),
	m_zoneId(0)
{
	this->setPosition( osVector3::zero );
}

//-----------------------------------------------------------------------------
void ZonePlugin::zoneUtility( void )
{
	if( 1 == this->m_iSolid )
	{
		// colors for checkerboard
		const Color gray1(0.27f);
		const Color gray2(0.30f);
		// draw checkerboard grid
		drawXZCheckerboardGrid( this->m_kZoneExtent.x, 1, this->position(), gray1, gray2);
	}
	else
	{
		// alternate style
		drawXZLineGrid (this->m_kZoneExtent.x, 1, this->position(), gBlack);
	}
}

//-----------------------------------------------------------------------------
void ZonePlugin::prepareOpen( void ) 
{ 
	this->addSubZones();
	BaseClass::prepareOpen();
}

//-----------------------------------------------------------------------------
void ZonePlugin::open( void ) 
{ 
	AbstractPlugin* pkParent = this->getParentPlugin();
	ZonePlugin* pkParentZone = dynamic_cast<ZonePlugin*>(pkParent);
	// the root zone
	if( NULL == pkParentZone )
	{
		initPluginCamera( this );
	}

	BaseClass::open();
}

//-----------------------------------------------------------------------------
void ZonePlugin::addSubZones( void )
{
	AbstractPlugin* pkParent = this->getParentPlugin();
	ZonePlugin* pkParentZone = dynamic_cast<ZonePlugin*>(pkParent);
	// the root zone
	if( NULL == pkParentZone )
	{
		osVector3 halfExtent = this->getZoneExtent();
		halfExtent *= 0.25;
		osVector3 startOffset = this->position();
		startOffset -= halfExtent;
		osVector3 offset = startOffset; 
		for( size_t i = 0; i < 4; ++i )
		{
			if( i == 2 )
			{
				offset = startOffset;
				offset.x += halfExtent.z * 2;
			}
			ZonePlugin* subZone = ET_NEW ZonePlugin();
			subZone->m_zoneId = i;
			subZone->setZoneCenter( this->getZoneCenter() + offset );
			subZone->setZoneExtent( this->getZoneExtent() * 0.5f );
			this->addPlugin( subZone );
			this->onSubZoneAdded( subZone );

			offset.z += halfExtent.z * 2;
		}
		this->addPlugin( ET_NEW OpenSteer::CameraPlugin() );
	}
}

//-----------------------------------------------------------------------------
void ZonePlugin::reset(void)
{
	BaseClass::reset();
}

//-----------------------------------------------------------------------------
void ZonePlugin::update( const float currentTime, const float elapsedTime ) 
{
	BaseClass::update( currentTime, elapsedTime );
}

//-----------------------------------------------------------------------------
void ZonePlugin::close( void ) 
{ 
	BaseClass::close();
}

//-----------------------------------------------------------------------------
void ZonePlugin::initGui( void* pkUserdata )
{
	GLUI* glui = ::getRootGLUI();
	GLUI_Panel* pluginPanel = static_cast<GLUI_Panel*>( pkUserdata );
	glui->add_checkbox_to_panel( pluginPanel, "Solid", &this->m_iSolid);
	BaseClass::initGui( pkUserdata );
}

//-----------------------------------------------------------------------------
void ZonePlugin::redraw (const float currentTime, const float elapsedTime) 
{ 

	// right now do not call the base class
	// as we want to see the children ...
//	BaseClass::redraw( currentTime, elapsedTime );
	BaseClass::redrawChildren( currentTime, elapsedTime );
	if( false == this->isVisible() )
	{
		return;
	}
	// draw "zone area"
	this->zoneUtility(  );

	AbstractPlugin* pkParent = this->getParentPlugin();
	ZonePlugin* pkParentZone = dynamic_cast<ZonePlugin*>(pkParent);
	// the root zone
	if( NULL == pkParentZone )
	{
	}
	else
	{
		// textual annotation
		std::ostringstream annote;
		annote << std::setprecision (2) << std::setiosflags (std::ios::fixed);
		annote << "Zone: " << this->getZoneId() << std::endl << "pos(" << this->position() << ")" << std::ends;
		draw2dTextAt3dLocation (annote, this->position(), gGreen, drawGetWindowWidth(), drawGetWindowHeight());
	}

}
