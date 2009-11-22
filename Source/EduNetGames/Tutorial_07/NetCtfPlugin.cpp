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

#include "NetCtfPlugin.h"
#include "NetCtfVehicles.h"
#include "NetCtfVehicleFactory.h"
#include "NetCtfGameLogic.h"

#include "EduNetCommon/EduNetDraw.h"
#include "EduNetConnect/NetworkPlugin.h"

#include "EduNetApplication/EduNetGames.h"
#include "OpenSteerUT/CameraPlugin.h"
#include "OpenSteerUT/AbstractVehicleGroup.h"
#include "OpenSteerUT/GridPlugin.h"

using namespace OpenSteer;


//-----------------------------------------------------------------------------
namespace	{
	//-------------------------------------------------------------------------
	// globals
	//(perhaps these should be member variables of a Vehicle or Plugin class)

	//-------------------------------------------------------------------------
	const int ctfEnemyCount = 4;
}

//-----------------------------------------------------------------------------
// now 1 global vehicle factory
NetCtfVehicleFactory gOfflineNetCtfVehicleFactory;

//-----------------------------------------------------------------------------
NetCtfPlugin::NetCtfPlugin( bool bAddToRegistry ):
BaseClass( bAddToRegistry ),resetCount(0),m_pkSeeker(NULL)
{
	this->setVehicleFactory( &gOfflineNetCtfVehicleFactory );
}
//-----------------------------------------------------------------------------
AbstractVehicle* NetCtfPlugin::createVehicle( EntityClassId classId, ProximityDatabase* pd ) const
{
	AbstractVehicle* pkVehicle = NULL;
	const AbstractVehicleFactory* pkFactory = this->getVehicleFactory();
	if( NULL != pkFactory )
	{
		pkVehicle = pkFactory->createVehicle( classId, pd );
	}
	return pkVehicle;
}

//-----------------------------------------------------------------------------
void NetCtfPlugin::addVehicle( AbstractVehicle* pkVehicle )
{
	if( NULL == pkVehicle )
	{
		return;
	}
	NetCtfSeekerVehicle* pkSeeker = dynamic_cast<NetCtfSeekerVehicle*>( pkVehicle );
	if( NULL != pkSeeker )
	{
		this->m_pkSeeker = pkSeeker;
		// initialize camera
		CameraPlugin::init2dCamera( *this->m_pkSeeker );
		Camera::camera.mode = Camera::cmFixedDistanceOffset;
		Camera::camera.fixedTarget.set(15, 0, 0);
		Camera::camera.fixedPosition.set(80, 60, 0);
	}
	AbstractVehicleGroup kVG( this->allVehicles() );
	kVG.addVehicle( pkVehicle );
}

//-----------------------------------------------------------------------------
void NetCtfPlugin::open (void)
{
	if( this->isRemoteObject() )
	{
		// client side initialize
		// a client does not do anything here
	}
	else
	{
		// create the seeker ("hero"/"attacker")
		this->addVehicle( this->createVehicle( ET_CID_CTF_SEEKER_VEHICLE, NULL ) );
		// create the specified number of enemies, 
		// storing pointers to them in an array.
		for (int i = 0; i < ctfEnemyCount; ++i)
		{
			this->addVehicle( this->createVehicle( ET_CID_CTF_ENEMY_VEHICLE, NULL ) );
		}

		NetCtfBaseVehicle::initializeObstacles();
	}

	// initialize camera
	// in case no seeker has been created
	// this might happen on the client side
	if( NULL == this->m_pkSeeker )
	{
		CameraPlugin::init2dCamera( *this->m_pkSeeker );
		Camera::camera.mode = Camera::cmFixedDistanceOffset;
		Camera::camera.fixedTarget.set(15, 0, 0);
		Camera::camera.fixedPosition.set(80, 60, 0);
	}

}

//-----------------------------------------------------------------------------
void NetCtfPlugin::update (const float currentTime, const float elapsedTime)
{
	NetCtfGameLogic kGameLogic;
	kGameLogic.setPlugin( this );
	kGameLogic.update( currentTime, elapsedTime );
	// update the seeker
	// update each enemy
	AbstractVehicleGroup kVG( this->allVehicles() );
	kVG.setCustomUpdated( &kGameLogic );
	kVG.update( currentTime, elapsedTime );

	if( true == kGameLogic.isGameOver() )
	{
		this->reset();
	}
}

//-----------------------------------------------------------------------------
void NetCtfPlugin::redraw (const float currentTime, const float elapsedTime)
{
	// selected vehicle (user can mouse click to select another)
	AbstractVehicle& selected = *SimpleVehicle::selectedVehicle;

	// vehicle nearest mouse (to be highlighted)
	AbstractVehicle& nearMouse = *OpenSteerDemo::vehicleNearestToMouse ();
#if 0
	// update camera
	CameraPlugin::updateCamera (currentTime, elapsedTime, selected);
#endif

	if( NULL != SimpleVehicle::selectedVehicle )
	{
		// draw "ground plane" centered between base and selected vehicle
		const Vec3 goalOffset = NetCtfGameLogic::ms_kHomeBaseCenter - Camera::camera.position();
		const Vec3 goalDirection = goalOffset.normalized ();
		const Vec3 cameraForward = Camera::camera.xxxls().forward();
		const float goalDot = cameraForward.dot (goalDirection);
		const float blend = remapIntervalClip (goalDot, 1, 0, 0.5, 0);
		const Vec3 gridCenter = interpolate (blend,
			selected.position(),
			NetCtfGameLogic::ms_kHomeBaseCenter);
		GridPlugin::setGridCenter( gridCenter );
	}
	// draw the seeker, obstacles and home base
	drawObstacles ();

	AbstractVehicleGroup kVG( this->allVehicles() );
	kVG.redraw( currentTime, elapsedTime );

	// highlight vehicle nearest mouse
	VehicleUtilities::highlightVehicleUtility (nearMouse);

	// display status in the upper left corner of the window
	std::ostringstream status;
	if( NULL != this->m_pkSeeker )
	{
		status << this->m_pkSeeker->getSeekerStateString() << std::endl;
	}
	status << NetCtfBaseVehicle::allObstacles.size() << " obstacles [F1/F2]" << std::endl;
	status << this->resetCount << " restarts" << std::ends;
	const float h = drawGetWindowHeight();
	const Vec3 screenLocation(10, h-50, 0);
	draw2dTextAt2dLocation( status, screenLocation, gGray80, drawGetWindowWidth(), drawGetWindowHeight() );

}

//-----------------------------------------------------------------------------
void NetCtfPlugin::close (void)
{
	AbstractVehicleGroup kVG( this->allVehicles() );
	// delete all Pedestrians
	while( kVG.population() > 0 )
	{
		// save pointer to last pedestrian, then remove it from the crowd
		AbstractVehicle* pkVehicle = all.back();
		all.pop_back();

		// if it is SimpleVehicle's selected vehicle, unselect it
		if (pkVehicle == SimpleVehicle::selectedVehicle)
		{
			SimpleVehicle::selectedVehicle = NULL;
		}
		if( pkVehicle == this->m_pkSeeker )
		{
			this->m_pkSeeker = NULL;
		}

		// delete the Pedestrian
		const AbstractVehicleFactory* pkFactory = this->getVehicleFactory();
		if( NULL != pkFactory )
		{
			pkFactory->destroyVehicle( pkVehicle );
		}
		else
		{
			ET_SAFE_DELETE( pkVehicle );
		}
		pkVehicle = NULL;
	}
}

//-----------------------------------------------------------------------------
void NetCtfPlugin::reset (void)
{
	// count the number of times the simulation has reset(e.g. for overnight runs)
	// count resets
	++this->resetCount;

	AbstractVehicleGroup kVG( this->allVehicles() );
	kVG.reset( );

	// reset camera position
	CameraPlugin::position2dCamera( *SimpleVehicle::selectedVehicle );

	// make camera jump immediately to new position
	Camera::camera.doNotSmoothNextMove ();
}

//-----------------------------------------------------------------------------
void NetCtfPlugin::handleFunctionKeys (int keyNumber)
{
	switch (keyNumber)
	{
	case 1: NetCtfBaseVehicle::addOneObstacle ();    break;
	case 2: NetCtfBaseVehicle::removeOneObstacle (); break;
	}
}

//-----------------------------------------------------------------------------
void NetCtfPlugin::printMiniHelpForFunctionKeys (void) const
{
	std::ostringstream message;
	message << "Function keys handled by ";
	message << '"' << name() << '"' << ':' << std::ends;
	EduNet::Log::printMessage (message);
	EduNet::Log::printMessage ("  F1     add one obstacle.");
	EduNet::Log::printMessage ("  F2     remove one obstacle.");
	EduNet::Log::printMessage ("");
}

//-----------------------------------------------------------------------------
void NetCtfPlugin::drawObstacles (void)
{
	const Color color (0.8f, 0.6f, 0.4f);
	const SOG& allSO = NetCtfBaseVehicle::allObstacles;
	for (SOI so = allSO.begin(); so != allSO.end(); so++)
	{
		drawXZCircle ((**so).radius, (**so).center, color, 40);
	}
}


