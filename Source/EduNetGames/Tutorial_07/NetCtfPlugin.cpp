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

#include "EduNetCommon/EduNetDraw.h"
#include "EduNetConnect/NetworkPlugin.h"

#include "EduNetApplication/EduNetGames.h"
#include "OpenSteerUT/CameraPlugin.h"

using namespace OpenSteer;


//-----------------------------------------------------------------------------
namespace	{
	//-------------------------------------------------------------------------
	// globals
	//(perhaps these should be member variables of a Vehicle or Plugin class)
	const Vec3 gHomeBaseCenter(0, 0, 0);
	const float gHomeBaseRadius = 1.5;

	const float gMinStartRadius = 30;
	const float gMaxStartRadius = 40;

	const float gBrakingRate = 0.75;

	const Color evadeColor    (0.6f, 0.6f, 0.3f); // annotation
	const Color seekColor     (0.3f, 0.6f, 0.6f); // annotation
	const Color clearPathColor(0.3f, 0.6f, 0.3f); // annotation

	const float gAvoidancePredictTimeMin  = 0.9f;
	const float gAvoidancePredictTimeMax  = 2;
	float gAvoidancePredictTime = gAvoidancePredictTimeMin;

	bool enableAttackSeek  = true; // for testing(perhaps retain for UI control?)
	bool enableAttackEvade = true; // for testing(perhaps retain for UI control?)

//	NetCtfSeekerVehicle* gSeeker = NULL;


	// count the number of times the simulation has reset(e.g. for overnight runs)
	int resetCount = 0;


	//-------------------------------------------------------------------------
	//-------------------------------------------------------------------------

	// state for OpenSteerDemo Plugin
	//
	// XXX consider moving this inside CtfPlugin
	// XXX consider using STL(any advantage? consistency?)


	AbstractVehicle* ctfSeeker;
	const int ctfEnemyCount = 4;
	AbstractVehicle* ctfEnemies [ctfEnemyCount];
}

//-----------------------------------------------------------------------------
// now 1 global vehicle factory for unit tests
NetCtfVehicleFactory gOfflineNetCtfVehicleFactory;

//-----------------------------------------------------------------------------
NetCtfPlugin::NetCtfPlugin( bool bAddToRegistry ):
	BaseClass( bAddToRegistry )
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
void NetCtfPlugin::open (void)
{
	// create the seeker ("hero"/"attacker")
	ctfSeeker = this->createVehicle( ET_CID_CTF_SEEKER_VEHICLE, NULL );
	all.push_back( ctfSeeker );

	// create the specified number of enemies, 
	// storing pointers to them in an array.
	for (int i = 0; i < ctfEnemyCount; ++i)
	{
		ctfEnemies[i] = this->createVehicle( ET_CID_CTF_ENEMY_VEHICLE, NULL );
		all.push_back (ctfEnemies[i]);
	}

	// initialize camera
	CameraPlugin::init2dCamera(*ctfSeeker);
	Camera::camera.mode = Camera::cmFixedDistanceOffset;
	Camera::camera.fixedTarget.set (15, 0, 0);
	Camera::camera.fixedPosition.set (80, 60, 0);

	NetCtfBaseVehicle::initializeObstacles ();
}

//-----------------------------------------------------------------------------
void NetCtfPlugin::update (const float currentTime, const float elapsedTime)
{
	// update the seeker
	ctfSeeker->update (currentTime, elapsedTime);

	// update each enemy
	for (int i = 0; i < ctfEnemyCount; i++)
	{
		ctfEnemies[i]->update (currentTime, elapsedTime);
	}
}

//-----------------------------------------------------------------------------
void NetCtfPlugin::redraw (const float currentTime, const float elapsedTime)
{
	// selected vehicle (user can mouse click to select another)
	AbstractVehicle& selected = *SimpleVehicle::selectedVehicle;

	// vehicle nearest mouse (to be highlighted)
	AbstractVehicle& nearMouse = *OpenSteerDemo::vehicleNearestToMouse ();
#if 1
	// update camera
	CameraPlugin::updateCamera (currentTime, elapsedTime, selected);
#endif

	if( NULL != SimpleVehicle::selectedVehicle )
	{
		// draw "ground plane" centered between base and selected vehicle
		const Vec3 goalOffset = gHomeBaseCenter-Camera::camera.position();
		const Vec3 goalDirection = goalOffset.normalized ();
		const Vec3 cameraForward = Camera::camera.xxxls().forward();
		const float goalDot = cameraForward.dot (goalDirection);
		const float blend = remapIntervalClip (goalDot, 1, 0, 0.5, 0);
		const Vec3 gridCenter = interpolate (blend,
			selected.position(),
			gHomeBaseCenter);
		OpenSteerDemo::gridUtility (gridCenter);
	}
	// draw the seeker, obstacles and home base
	ctfSeeker->draw( currentTime, elapsedTime );
	drawObstacles ();

	// draw each enemy
	for (int i = 0; i < ctfEnemyCount; i++) ctfEnemies[i]->draw( currentTime, elapsedTime );

	// highlight vehicle nearest mouse
	OpenSteerDemo::highlightVehicleUtility (nearMouse);
}

//-----------------------------------------------------------------------------
void NetCtfPlugin::close (void)
{
	// delete seeker
	delete (ctfSeeker);
	ctfSeeker = NULL;

	// delete each enemy
	for (int i = 0; i < ctfEnemyCount; i++)
	{
		delete (ctfEnemies[i]);
		ctfEnemies[i] = NULL;
	}

	// clear the group of all vehicles
	all.clear();
}

//-----------------------------------------------------------------------------
void NetCtfPlugin::reset (void)
{
	// count resets
	resetCount++;

	// reset the seeker ("hero"/"attacker") and enemies
	ctfSeeker->reset ();
	for (int i = 0; i<ctfEnemyCount; i++) ctfEnemies[i]->reset ();

	// reset camera position
	CameraPlugin::position2dCamera (*ctfSeeker);

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
	OpenSteerDemo::printMessage (message);
	OpenSteerDemo::printMessage ("  F1     add one obstacle.");
	OpenSteerDemo::printMessage ("  F2     remove one obstacle.");
	OpenSteerDemo::printMessage ("");
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

NetCtfPlugin gCtfPlugin;

