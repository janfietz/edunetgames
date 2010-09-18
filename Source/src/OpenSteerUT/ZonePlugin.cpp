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
	m_kZoneExtent( OS_SCALAR( 0.0 ), OS_SCALAR( 0.0 ), OS_SCALAR( 0.0 ) ),
	m_iSolid(0),
	m_zoneId(0),
	m_fBorderWidth( OS_SCALAR( 0.0 ) )
{
	this->setPosition( osVector3::zero );
	this->setZoneExtent( osVector3( OS_SCALAR( 5.0 ), OS_SCALAR( 0.0 ), OS_SCALAR( 5.0 ) ) );
	this->setBorderWidth( 0.0f );
	this->setZoneColor( Color::_gBlue );
}

//-----------------------------------------------------------------------------
void ZonePlugin::zoneUtility( void )
{
	const osScalar drawExtent = this->m_kZoneExtent.x * OS_SCALAR( 2.0 ) - 0.001f;
	if( 1 == this->m_iSolid )
	{
		// colors for checkerboard
		const Color gray(0.27f);
		Color zoneGray( this->getZoneColor() );
		zoneGray.setR( zoneGray.r() * gray.r() );
		zoneGray.setG( zoneGray.g() * gray.g() );
		zoneGray.setB( zoneGray.b() * gray.b() );
//		zoneGray.setR( zoneGray.r() * gray.r() );
		// draw checkerboard grid
		drawXZCheckerboardGrid( drawExtent, 10, this->position(), zoneGray, gray);
	}
	else
	{
		// alternate style
		drawXZLineGrid( drawExtent, 1, this->position(), this->m_kZoneColor );
	}

	const osScalar borderWidth = this->getBorderWidth();
	if( borderWidth > 0 )
	{
		//this->m_kZoneAABBox.draw( this->m_kBorderColor );
		drawXZLineGrid( drawExtent + borderWidth, 1, this->position(), this->m_kBorderColor );
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
bool ZonePlugin::isVehicleInside( const OpenSteer::AbstractVehicle& kVehicle ) const
{
	return this->m_kZoneAABBox.insideXZWithRadius(kVehicle);
}

//-----------------------------------------------------------------------------
bool ZonePlugin::queryVehicleColor( const OpenSteer::AbstractVehicle& kVehicle, OpenSteer::Color& kColor ) const 
{ 
	kColor = this->getZoneColor();
	return true; 
}

//-----------------------------------------------------------------------------
void ZonePlugin::addSubZones( void )
{
	AbstractPlugin* pkParent = this->getParentPlugin();
	ZonePlugin* pkParentZone = dynamic_cast<ZonePlugin*>(pkParent);
	// the root zone
	if( NULL == pkParentZone )
	{
		// any subzones present ?
		if( this->size() == 0 )
		{
			osScalar colorBase = 0.8;
			osScalar colorWhite = 0.3;

			osColor kZoneColors[4] =
			{
				osColor( colorBase, colorWhite, colorWhite ),
				osColor( colorBase, colorBase, colorWhite ),
				osColor( colorWhite, colorBase, colorWhite ),
				osColor( colorWhite, colorWhite, colorBase ),
			};

			osVector3 halfExtent = this->getZoneExtent();
			halfExtent *= 0.5;
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
				subZone->setZoneColor( kZoneColors[i] );
				subZone->setBorderWidth( 1.0f );

				this->addPlugin( subZone );
				this->onSubZoneAdded( subZone );

				offset.z += halfExtent.z * 2;
			}
			this->addPlugin( ET_NEW OpenSteer::CameraPlugin() );
		}
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
	AbstractPlugin* pkParent = this->getParentPlugin();
	ZonePlugin* pkParentZone = dynamic_cast<ZonePlugin*>(pkParent);
	// the root zone
	if( NULL == pkParentZone )
	{
		// right now do not call the base class
		// as we want to see the children ...
		BaseClass::redrawChildren( currentTime, elapsedTime );
		if( false == this->isVisible() )
		{
			return;
		}
		// draw "zone area"
		// this->zoneUtility(  );
	}
	else
	{
		// draw "zone area"
		if( true == this->isVisible() )
		{
			this->zoneUtility(  );
		}
		BaseClass::redraw( currentTime, elapsedTime );
		if( false == this->isVisible() )
		{
			return;
		}
		// textual annotation
		AbstractPlugin* contentPlugin = this->getPlugin(0);
		size_t population = 0;
		if( NULL != contentPlugin )
		{
			osAVGroup vehicles = contentPlugin->allVehicles();
			population = vehicles.size();
		}
		std::ostringstream annote;
		annote << std::setprecision (2) << std::setiosflags (std::ios::fixed);
		annote << "Zone: " << this->getZoneId() << std::endl << "pos(" << this->position() << ")" << std::endl;
		annote << "Population: " << population << std::endl << std::ends;
		draw2dTextAt3dLocation (annote, this->position(), gGreen, drawGetWindowWidth(), drawGetWindowHeight());
	}

}
