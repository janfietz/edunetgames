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
#include "NetCtfEntityFactory.h"
#include "NetCtfGameLogic.h"

#include "EduNetCommon/EduNetDraw.h"
#include "EduNetConnect/NetworkPlugin.h"

#include "EduNetApplication/EduNetGames.h"
#include "OpenSteerUT/CameraPlugin.h"
#include "OpenSteerUT/AbstractVehicleGroup.h"
#include "OpenSteerUT/GridPlugin.h"
#include "OpenSteerUT/AbstractEntityQuery.h"

using namespace OpenSteer;


//-----------------------------------------------------------------------------
namespace	{
	//-------------------------------------------------------------------------
	// globals
	//(perhaps these should be member variables of a Vehicle or Plugin class)

	//-------------------------------------------------------------------------
	const size_t ctfEnemyCount = 4;
	const size_t initialObstacleCount = 10;

	const float gMinStartRadius = 30;
	const float gMaxStartRadius = 40;

	void initPluginCamera( osAbstractVehicle& kVehicle )
	{
		// camera setup
		CameraPlugin::init2dCamera( kVehicle );
		// Camera::camera.mode = Camera::cmFixedDistanceOffset;
		Camera::camera.mode = Camera::cmStraightDown;
		Camera::camera.fixedTarget.set( 15, 0, 0 );
		Camera::camera.fixedPosition.set( 20, 20, 20 );
		Camera::camera.lookdownDistance = 15;
		// make camera jump immediately to new position
		Camera::camera.doNotSmoothNextMove ();
	}
}

//-----------------------------------------------------------------------------
// now 1 global vehicle factory
NetCtfEntityFactory* NetCtfPlugin::m_sOfflineNetCtfEntityFactory = NULL;

//-----------------------------------------------------------------------------
void NetCtfPlugin::allocateEntityFactory( void )
{
	NetCtfPlugin::m_sOfflineNetCtfEntityFactory = ET_NEW NetCtfEntityFactory();
}

//-----------------------------------------------------------------------------
void NetCtfPlugin::destroyEntityFactory( void )
{
	ET_SAFE_DELETE( NetCtfPlugin::m_sOfflineNetCtfEntityFactory ); 
}

//-----------------------------------------------------------------------------
NetCtfPlugin::NetCtfPlugin( bool bAddToRegistry ):
BaseClass( bAddToRegistry ),resetCount(0),m_pkSeeker(NULL)
{
	this->setEntityFactory( NetCtfPlugin::m_sOfflineNetCtfEntityFactory );
}
//-----------------------------------------------------------------------------
AbstractVehicle* NetCtfPlugin::createVehicle( EntityClassId classId) const
{
	AbstractVehicle* pkVehicle = NULL;
	const AbstractEntityFactory* pkFactory = this->getEntityFactory();
	if( NULL != pkFactory )
	{
		pkVehicle = pkFactory->createVehicle( classId );
		NetCtfBaseVehicle* pkBaseVehicle = dynamic_cast<NetCtfBaseVehicle*>( pkVehicle );
		if( NULL != pkBaseVehicle )
		{
			pkBaseVehicle->m_pkObstacles = &this->allObstacles();
		}
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
		initPluginCamera( *this->m_pkSeeker );
	}
	AbstractVehicleGroup kVG( this->allVehicles() );
	kVG.addVehicleToPlugin( pkVehicle, this );
}
//-----------------------------------------------------------------------------
void NetCtfPlugin::removeVehicle ( osAbstractVehicle* pkVehicle)
{
 AbstractVehicleGroup kVG ( this->allVehicles() );
    kVG.removeVehicleFromPlugin ( pkVehicle );
}

//-----------------------------------------------------------------------------
#define testOneObstacleOverlap(radius, center)               \
{                                                            \
	float d = Vec3::distance(c, center);                    \
	float clearance = d -(r +(radius));                    \
	if(minClearance > clearance) minClearance = clearance;  \
}

//-----------------------------------------------------------------------------
void NetCtfPlugin::addOneObstacle( void )
{
	static NetCtfSeekerVehicle kPrototype;
//	if(obstacleCount < maxObstacleCount)
	{
		// pick a random center and radius,
		// loop until no overlap with other obstacles and the home base
		float r;
		Vec3 c;
		float minClearance;
		const float requiredClearance = kPrototype.radius() * 4; // 2 x diameter
		ObstacleGroup::const_iterator kIter = this->allObstacles().begin();
		ObstacleGroup::const_iterator kEnd = this->allObstacles().end();
		do
		{
			r = frandom2(1.5, 4);
			c = randomVectorOnUnitRadiusXZDisk() * gMaxStartRadius * 1.1f;
			minClearance = FLT_MAX;

			for( ObstacleGroup::const_iterator so = kIter; so != kEnd; ++so )
			{
				SphereObstacle* pkSphere = dynamic_cast<SphereObstacle*>(*so);
				testOneObstacleOverlap( pkSphere->radius,pkSphere->center );
			}

			testOneObstacleOverlap(NetCtfGameLogic::ms_fHomeBaseRadius - requiredClearance,
				NetCtfGameLogic::ms_kHomeBaseCenter);
		}
		while(minClearance < requiredClearance);

		const AbstractEntityFactory* pkFactory = this->getEntityFactory();
		if( NULL != pkFactory )
		{
			AbstractEntity* pkEntity = pkFactory->createEntity( OS_CID_SPHEREOBSTACLE );
			SphereObstacle* pkSphereObstacle = dynamic_cast<SphereObstacle*>(pkEntity);
			pkSphereObstacle->radius = r;
			pkSphereObstacle->center = c;
			// add new non-overlapping obstacle to registry
			this->allObstacles().push_back( pkSphereObstacle );
		}
	}
}

//-----------------------------------------------------------------------------
void NetCtfPlugin::removeOneObstacle( void )
{
	if( this->allObstacles().size() > 0 )
	{
		AbstractObstacle* pkObstacle = this->allObstacles()[this->allObstacles().size() - 1];
		const AbstractEntityFactory* pkFactory = this->getEntityFactory();
		if( NULL != pkFactory )
		{
			pkFactory->destroyEntity( pkObstacle );
		}
		else
		{
			ET_SAFE_DELETE( pkObstacle );
		}
		this->allObstacles().pop_back();
	}
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
		for( size_t i = 0; i < initialObstacleCount; ++i )
		{
			this->addOneObstacle();
		}

		// create the seeker ("hero"/"attacker")
		AbstractVehicle* pkVehicle = this->createVehicle( ET_CID_CTF_SEEKER_VEHICLE );
		this->addVehicle( pkVehicle );
		this->randomizeStartingPositionAndHeading( pkVehicle );  // new starting position

		// create the specified number of enemies,
		// storing pointers to them in an array.
		for (size_t i = 0; i < ctfEnemyCount; ++i)
		{
			pkVehicle = this->createVehicle( ET_CID_CTF_ENEMY_VEHICLE );
			this->addVehicle( pkVehicle );
			this->randomizeStartingPositionAndHeading( pkVehicle );  // new starting position
		}
	}

	// initialize camera
	// in case no seeker has been created
	// this might happen on the client side
	if( NULL != this->m_pkSeeker )
	{
		initPluginCamera( *this->m_pkSeeker );
	}
}

//-----------------------------------------------------------------------------
void NetCtfPlugin::update (const float currentTime, const float elapsedTime)
{
	if( false == this->isEnabled() )
	{
		return;
	}

	// any players ?
	if( ( this->allPlayers().size() > 0 ) && ( false == this->isRemoteObject() ) )
	{
		// any player without controlled entity ?
		NoControlledEntityQuery kNoControlledEntityQuery;
		AbstractPlayerQuery kPlayerQuery( this->allPlayers(), &kNoControlledEntityQuery );
		AbstractPlayer* pkPlayer = kPlayerQuery.findType();
		if( NULL != pkPlayer )
		{
			// any vehicle without player ?
			NoPlayerEntityQuery kNoPlayerEntityQuery;
			AVQuery kEntityQuery( this->allVehicles(), &kNoPlayerEntityQuery );
			osAbstractVehicle* pkVehicle = kEntityQuery.findType( );
			if( NULL != pkVehicle )
			{
				pkPlayer->play( pkVehicle );
// 				std::ostringstream message;
// 				message << "player starts playing ";
// 				message << '"' << pkVehicle->name() << '"' << std::endl;
// 				EduNet::Log::printMessage (message);
			}
		}
	}

	if( false == this->isRemoteObject() )
	{
		AbstractPlayerGroup& kAllPlayers = this->allPlayers( );
		AbstractPlayerGroup::iterator kIter = kAllPlayers.begin();
		AbstractPlayerGroup::iterator kEnd = kAllPlayers.end();
		while( kIter != kEnd )
		{
			OpenSteer::CastToAbstractUpdated( *kIter )->update( currentTime, elapsedTime );
			++kIter;
		}
	}


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
	if( false == this->isVisible() )
	{
		return;
	}

	// selected vehicle (user can mouse click to select another)
	if( NULL != SimpleVehicle::getSelectedVehicle() )
	{
		AbstractVehicle& selected = *SimpleVehicle::getSelectedVehicle();
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

	// display status in the upper left corner of the window
	std::ostringstream status;
	const float h = drawGetWindowHeight ();
	osVector3 screenLocation (10, h - 50, 0);
	Color kColor = gGray80;
	if( false == this->isRemoteObject() )
	{
		if( NULL != this->m_pkSeeker )
		{
			status << this->m_pkSeeker->getSeekerStateString() << std::endl;
		}
		status << this->allObstacles().size() << " obstacles [F1/F2]" << std::endl;
		status << this->resetCount << " restarts" << std::ends;
	}
	else
	{
		if( NULL != this->m_pkSeeker )
		{
			status << this->m_pkSeeker->getSeekerStateString() << std::endl;
		}
		status << this->allObstacles().size() << " obstacles" << std::endl;
		status << this->resetCount << " restarts" << std::ends;
		screenLocation.y -= 50.0f;
		kColor = gGray50;
	}

	draw2dTextAt2dLocation( status, screenLocation, gGray80, drawGetWindowWidth(), drawGetWindowHeight() );
}

//-----------------------------------------------------------------------------
void NetCtfPlugin::close (void)
{
	AbstractPlayerGroup& kAllPlayers = this->allPlayers( );
	AbstractPlayerGroup::iterator kPIter = kAllPlayers.begin();
	AbstractPlayerGroup::iterator kPEnd = kAllPlayers.end();
	while( kPIter != kPEnd )
	{
		( *kPIter )->play( NULL );
		++kPIter;
	}

	AbstractVehicleGroup kVG( this->allVehicles() );
	// delete all Pedestrians
	while( kVG.population() > 0 )
	{
		// save pointer to last pedestrian, then remove it from the crowd
		AbstractVehicle* pkVehicle = all.back();
		all.pop_back();

		// if it is SimpleVehicle's selected vehicle, unselect it
		if (pkVehicle == SimpleVehicle::getSelectedVehicle())
		{
			SimpleVehicle::setSelectedVehicle( NULL );
		}
		if( pkVehicle == this->m_pkSeeker )
		{
			this->m_pkSeeker = NULL;
		}

		// delete the Pedestrian
		const AbstractEntityFactory* pkFactory = this->getEntityFactory();
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

	ObstacleGroup::const_iterator kIter = this->allObstacles().begin();
	ObstacleGroup::const_iterator kEnd = this->allObstacles().end();
	while( kIter != kEnd )
	{
		AbstractObstacle* pkObstacle = *kIter;
		const AbstractEntityFactory* pkFactory = this->getEntityFactory();
		if( NULL != pkFactory )
		{
			pkFactory->destroyEntity( pkObstacle );
		}
		else
		{
			ET_SAFE_DELETE( pkObstacle );
		}
		++kIter;
	}
	this->allObstacles().clear();

}

//-----------------------------------------------------------------------------
void NetCtfPlugin::reset (void)
{
	// count the number of times the simulation has reset(e.g. for overnight runs)
	// count resets
	++this->resetCount;

	AbstractVehicleGroup kVG( this->allVehicles() );
	kVG.reset( );
	AVIterator iter = allVehicles().begin();
	AVIterator last = allVehicles().end();
	while( iter != last )
	{
		this->randomizeStartingPositionAndHeading( *iter );  // new starting position
		++iter;
	}

	// reset camera position
	// camera setup
	if( false == this->isRemoteObject() )
	{
		// reset camera position
		CameraPlugin::position2dCamera( *this->m_pkSeeker );
		// make camera jump immediately to new position
		Camera::camera.doNotSmoothNextMove ();
	}
}

//-----------------------------------------------------------------------------
void NetCtfPlugin::handleFunctionKeys (int keyNumber)
{
	if( false == this->isRemoteObject() )
	{
		switch (keyNumber)
		{
		case 1: this->addOneObstacle ();    break;
		case 2: this->removeOneObstacle (); break;
		}
	}
}

//-----------------------------------------------------------------------------
void NetCtfPlugin::printMiniHelpForFunctionKeys (void) const
{
	if( false == this->isRemoteObject() )
	{
		std::ostringstream message;
		message << "Function keys handled by ";
		message << '"' << name() << '"' << ':' << std::ends;
		EduNet::Log::printMessage (message);
		EduNet::Log::printMessage ("  F1     add one obstacle.");
		EduNet::Log::printMessage ("  F2     remove one obstacle.");
		EduNet::Log::printMessage ("");

	}
}

//-----------------------------------------------------------------------------
void NetCtfPlugin::drawObstacles (void)
{
	const Color color (0.8f, 0.6f, 0.4f);
	ObstacleGroup::const_iterator kIter = this->allObstacles().begin();
	ObstacleGroup::const_iterator kEnd = this->allObstacles().end();
	while( kIter != kEnd )
	{
		SphereObstacle* pkSphere = dynamic_cast<SphereObstacle*>(*kIter);
		drawXZCircle( pkSphere->radius, pkSphere->center, color, 40 );
		++kIter;
	}
}

void NetCtfPlugin::randomizeStartingPositionAndHeading( osAbstractVehicle* pkVehicle )
{
	// randomize position on a ring between inner and outer radii
	// centered around the home base
	const float rRadius = frandom2(gMinStartRadius, gMaxStartRadius);
	const Vec3 randomOnRing = RandomUnitVectorOnXZPlane() * rRadius;
	pkVehicle->setPosition(NetCtfGameLogic::ms_kHomeBaseCenter + randomOnRing);

	// are we are too close to an obstacle?
	if( this->minDistanceToObstacle( pkVehicle->position()) < pkVehicle->radius()*5 )
	{
		// if so, retry the randomization(this recursive call may not return
		// if there is too little free space)
		randomizeStartingPositionAndHeading( pkVehicle );
	}
	else
	{
		// otherwise, if the position is OK, randomize 2D heading
		pkVehicle->randomizeHeadingOnXZPlane();
	}

}

//-----------------------------------------------------------------------------
float NetCtfPlugin::minDistanceToObstacle( const Vec3& point ) const
{
	float r = 0;
	Vec3 c = point;
	float minClearance = FLT_MAX;

	ObstacleGroup::const_iterator kIter = this->allObstacles().begin();
	ObstacleGroup::const_iterator kEnd = this->allObstacles().end();
	while( kIter != kEnd )
	{
		SphereObstacle* pkSphere = dynamic_cast<SphereObstacle*>(*kIter);
		testOneObstacleOverlap(pkSphere->radius,pkSphere->center);
		++kIter;
	}
	return minClearance;
}


//-----------------------------------------------------------------------------
void addAnObstacle(GLUI_Control* pkControl )
{
	NetCtfPlugin* pkPlugin = (NetCtfPlugin*)pkControl->ptr_val;
	pkPlugin->addOneObstacle();
}

//-----------------------------------------------------------------------------
void removeAnObstacle(GLUI_Control* pkControl )
{
	NetCtfPlugin* pkPlugin = (NetCtfPlugin*)pkControl->ptr_val;
	pkPlugin->removeOneObstacle();
}

//-----------------------------------------------------------------------------
// implement to initialize additional gui functionality
void NetCtfPlugin::initGui( void* pkUserdata )
{
	if( false == this->isRemoteObject() )
	{

		GLUI* glui = ::getRootGLUI();
		GLUI_Panel* pluginPanel = static_cast<GLUI_Panel*>( pkUserdata );

		GLUI_Control* pkControl;
		pkControl = glui->add_button_to_panel( pluginPanel, "Add Obstacle", -1, addAnObstacle );
		pkControl->set_ptr_val( this );
		pkControl = glui->add_button_to_panel( pluginPanel, "Remove Obstacle", -1, removeAnObstacle  );
		pkControl->set_ptr_val( this );
	}
};
