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

#include "NetPedestrian.h"
#include "NetPedestrianPlugin.h"
#include "OpenSteerUT/ZonePlugin.h"

using namespace OpenSteer;

namespace
{
	ObstacleGroup gObstacles;
}

#pragma warning(push)
#pragma warning(disable: 4355) // warning C4355: 'this' : used in base member initializer list
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// constructor
NetPedestrian::NetPedestrian():
path(NULL)
{
	// size of bounding sphere, for obstacle avoidance, etc.
	setRadius (0.5); // width = 0.7, add 0.3 margin, take half
	// reset Pedestrian state
	reset ();

	// for debugging
	const char* pszClassName = this->getClassName();
#ifdef ET_DEBUG
	//static TNetPedestrian kTestPedestrian;
	//OpenSteer::EntityClassId classId = kTestPedestrian.getClassId();
#endif
}

#pragma warning(pop)

//-----------------------------------------------------------------------------
NetPedestrian::~NetPedestrian()
{
}

//-----------------------------------------------------------------------------
AbstractVehicle* NetPedestrian::cloneVehicle( void ) const
{
	AbstractVehicle* pkVehicle = ET_NEW NetPedestrian();
	return pkVehicle;
}

//-----------------------------------------------------------------------------
// reset all instance state
void NetPedestrian::reset (void)
{
	float currRadius = this->radius();

	// reset the vehicle
	// but keep the radius
	SimpleVehicle::reset ();
	
	this->setRadius( currRadius );

	// max speed and max steering force (maneuverability)
// 	setMaxSpeed (2.0);
 	setMaxForce (8.0);
	setMaxSpeed (4.0f * currRadius);
//	setMaxForce (16.0f * currRadius);

	// initially stopped
	setSpeed (0);

// 	// size of bounding sphere, for obstacle avoidance, etc.
// 	setRadius (0.5); // width = 0.7, add 0.3 margin, take half

	// set the path for this Pedestrian to follow
//	path = getTestPath ();

	if( path != NULL )
	{
		// set initial position
		// (random point on path + random horizontal offset)
		const float d = path->length() * frandom01();
		const float r = path->radius();
		const osVector3 randomOffset = randomVectorOnUnitRadiusXZDisk () * r;
		setPosition( path->mapPathDistanceToPoint (d) + randomOffset );
	}

	// randomize 2D heading
	randomizeHeadingOnXZPlane ();

	// pick a random direction for path following (upstream or downstream)
	pathDirection = (frandom01() > 0.5) ? -1 : +1;

	// trail parameters: 3 seconds with 60 points along the trail
	setTrailParameters (3, 60);

	// notify proximity database that our position has changed
	if( NULL != this->m_pkProximityToken )
	{
		m_pkProximityToken->updateForNewPosition(position());
	}

	// notify the update states
	this->m_kSteeringForceUpdate.setVehicle( this );
	this->m_kEulerUpdate.setVehicle( this );
}

//-----------------------------------------------------------------------------
// per frame simulation update
void NetPedestrian::update (const float currentTime, const float elapsedTime)
{
	BaseClass::update( currentTime, elapsedTime );

	if( false == this->isRemoteObject() )	
	{
		// reverse direction when we reach an endpoint
		NetPedestrianPlugin* netPedestrianPlugin = dynamic_cast<NetPedestrianPlugin*>(this->getParentEntity());
		if( netPedestrianPlugin->m_bUseDirectedPathFollowing )
		{
			const Color darkRed (0.7f, 0, 0);
			float const pathRadius = path->radius();

			if (osVector3::distance (position(), path->getStartPoint()) < pathRadius )
			{
				pathDirection = +1;
				annotationXZCircle (pathRadius, path->getStartPoint(), darkRed, 20);
			}
			if (osVector3::distance (position(), path->getEndPoint()) < pathRadius )
			{
				pathDirection = -1;
				annotationXZCircle (pathRadius, path->getEndPoint(), darkRed, 20);
			}
		}
	}
}

//-----------------------------------------------------------------------------
// compute combined steering force: move forward, avoid obstacles
// or neighbors if needed, otherwise follow the path and wander
osVector3 NetPedestrian::determineCombinedSteering (const float elapsedTime)
{
	// note: to enable a better view on a remote vehicle we just
	//       skip computing a steering force for these guys
	if( this->isRemoteObject() )
	{
		return osVector3::zero;
	}

	// move forward
	osVector3 steeringForce = forward();

	// probability that a lower priority behavior will be given a
	// chance to "drive" even if a higher priority behavior might
	// otherwise be triggered.
//	const float leakThrough = 0.1f;

	// no random behaviour for network samples
	const float leakThrough = -1.0f;

	// determine if obstacle avoidance is required
	osVector3 obstacleAvoidance;
	if (leakThrough < frandom01())
	{
		const float oTime = 6; // minTimeToCollision = 6 seconds
		// ------------------------------------ xxxcwr11-1-04 fixing steerToAvoid
		// just for testing
		//             obstacleAvoidance = steerToAvoidObstacles (oTime, gObstacles);
		//             obstacleAvoidance = steerToAvoidObstacle (oTime, gObstacle1);
		//             obstacleAvoidance = steerToAvoidObstacle (oTime, gObstacle3);
		obstacleAvoidance = steerToAvoidObstacles (oTime, gObstacles);
		// ------------------------------------ xxxcwr11-1-04 fixing steerToAvoid
	}

	// if obstacle avoidance is needed, do it
	if (obstacleAvoidance != osVector3::zero)
	{
		steeringForce += obstacleAvoidance;
	}
	else
	{
		NetPedestrianPlugin* netPedestrianPlugin = dynamic_cast<NetPedestrianPlugin*>(this->getParentEntity());
		// otherwise consider avoiding collisions with others
		osVector3 collisionAvoidance;
		const float caLeadTime = 3;

		// find all neighbors within maxRadius using proximity database
		// (radius is largest distance between vehicles traveling head-on
		// where a collision is possible within caLeadTime seconds.)
		const float maxRadius = caLeadTime * maxSpeed() * 2;
		m_kNeighbors.clear();
		m_pkProximityToken->findNeighbors (position(), maxRadius, m_kNeighbors);

		if (leakThrough < frandom01())
			collisionAvoidance =
			steerToAvoidNeighbors (caLeadTime, m_kNeighbors) * 10;

		// if collision avoidance is needed, do it
		if (collisionAvoidance != osVector3::zero)
		{
			steeringForce += collisionAvoidance;
		}
		else
		{
			// add in wander component (according to user switch)
			if (netPedestrianPlugin->m_bWanderSwitch)
				steeringForce += steerForWander (elapsedTime);

			// do (interactively) selected type of path following
			const float pfLeadTime = 3;
			const osVector3 pathFollow =
				(netPedestrianPlugin->m_bUseDirectedPathFollowing ?
				steerToFollowPath (pathDirection, pfLeadTime, *path) :
			steerToStayOnPath (pfLeadTime, *path));

			// add in to steeringForce
			steeringForce += pathFollow * 0.5;
		}
	}
	// return steering constrained to global XZ "ground" plane
	return steeringForce.setYtoZero();
}

//-----------------------------------------------------------------------------
// draw this pedestrian into scene
void NetPedestrian::draw( const float currentTime, const float elapsedTime )
{
	BaseClass::draw( currentTime, elapsedTime );
	Color kColor;
	Vec3 kPosition = this->position();
	bool bGotParentColor = false;
	AbstractPlugin* parentPlugin = dynamic_cast<AbstractPlugin*>(this->getParentEntity());
	ZonePlugin* zonePlugin = NULL;
	if( NULL != parentPlugin )
	{
		zonePlugin = dynamic_cast<ZonePlugin*>(parentPlugin->getParentPlugin());
		bGotParentColor = parentPlugin->queryVehicleColor( *this, kColor ) ;
		if( true == bGotParentColor )
		{

		}
		else
		{
			if( true == this->isRemoteObject() )
			{
				kColor = gGreen;
				Vec3 kTempPosition = kPosition;
				kTempPosition.y += 0.05f;
				this->setPosition( kTempPosition );
			}
			else
			{
				kColor = gRed;
			}
			kColor.setA( 0.5f );
		}
	}

	OpenSteer::drawBasic2dCircularVehicle (*this, kColor);
	this->setPosition( kPosition );
	kColor.setA( 1.0f );
	OpenSteer::EAnnotationMode eMode = this->getAnnotationMode();
	this->setAnnotationMode( OpenSteer::EAnnotationMode_local );
	this->drawTrail( kColor, gWhite );
	this->setAnnotationMode( eMode );


	if( NULL != zonePlugin )
	{
		// check for zone memberships
		// textual annotation
		std::ostringstream annote;
		annote << std::setprecision (2) << std::setiosflags (std::ios::fixed);
		annote << "z[";
		for( size_t i = 0; i < 4; ++i )
		{
			if( true == this->getIsZoneMember(i) )
			{
				annote << i;
			}
			else
			{
				annote << " ";
			}
			if( i < 3 )
			{
				annote << "-";
			}
		}
		annote << "]\n";	

		//draw borders		
		annote << "b[";
		for( size_t i = 0; i < 4; ++i )
		{
			if( true == this->getIsZoneBorderMember(i) )
			{
				annote << i;
			}
			else
			{
				annote << " ";
			}
			if( i < 3 )
			{
				annote << "-";
			}
		}
		annote << "]";
		
		draw2dTextAt3dLocation (annote, this->position(), gWhite, drawGetWindowWidth(), drawGetWindowHeight());
	}
}

//-----------------------------------------------------------------------------
// called when steerToFollowPath decides steering is required
void NetPedestrian::annotatePathFollowing (const osVector3& future,
							const osVector3& onPath,
							const osVector3& target,
							const float outside)
{
	const Color yellow (1, 1, 0);
	const Color lightOrange (1.0f, 0.5f, 0.0f);
	const Color darkOrange  (0.6f, 0.3f, 0.0f);
	const Color yellowOrange (1.0f, 0.75f, 0.0f);

	// draw line from our position to our predicted future position
	annotationLine (position(), future, yellow);

	// draw line from our position to our steering target on the path
	annotationLine (position(), target, yellowOrange);

	// draw a two-toned line between the future test point and its
	// projection onto the path, the change from dark to light color
	// indicates the boundary of the tube.
	const osVector3 boundaryOffset = (onPath - future).normalized() * outside;
	const osVector3 onPathBoundary = future + boundaryOffset;
	annotationLine (onPath, onPathBoundary, darkOrange);
	annotationLine (onPathBoundary, future, lightOrange);
}

//-----------------------------------------------------------------------------
// called when steerToAvoidCloseNeighbors decides steering is required
// (parameter names commented out to prevent compiler warning from "-W")
void NetPedestrian::annotateAvoidCloseNeighbor (const AbstractVehicle& other,
								 const float /*additionalDistance*/)
{
	// draw the word "Ouch!" above colliding vehicles
	const float headOn = forward().dot(other.forward()) < 0;
	const Color green (0.4f, 0.8f, 0.1f);
	const Color red (1, 0.1f, 0);
	const Color color = headOn ? red : green;
	const char* string = headOn ? "OUCH!" : "pardon me";
	const osVector3 location = position() + osVector3 (0, 0.5f, 0);
	if (OpenSteer::annotationIsOn())
		draw2dTextAt3dLocation (string, location, color, drawGetWindowWidth(), drawGetWindowHeight());
}

//-----------------------------------------------------------------------------
// (parameter names commented out to prevent compiler warning from "-W")
void NetPedestrian::annotateAvoidNeighbor (const AbstractVehicle& threat,
							const float /*steer*/,
							const osVector3& ourFuture,
							const osVector3& threatFuture)
{
	const Color green (0.15f, 0.6f, 0.0f);

	annotationLine( position(), ourFuture, green );
	annotationLine( threat.position(), threatFuture, green );
	annotationLine( ourFuture, threatFuture, gRed );
	annotationXZCircle( radius(), ourFuture,    green, 12 );
	annotationXZCircle( radius(), threatFuture, green, 12 );
}

//-----------------------------------------------------------------------------
// xxx perhaps this should be a call to a general purpose annotation for
// xxx "local xxx axis aligned box in XZ plane" -- same code in in
// xxx CaptureTheFlag.cpp
void NetPedestrian::annotateAvoidObstacle( const float minDistanceToCollision )
{
	const osVector3 boxSide = side() * radius();
	const osVector3 boxFront = forward() * minDistanceToCollision;
	const osVector3 FR = position() + boxFront - boxSide;
	const osVector3 FL = position() + boxFront + boxSide;
	const osVector3 BR = position()            - boxSide;
	const osVector3 BL = position()            + boxSide;
	const Color white( 1,1,1 );
	annotationLine( FR, FL, white );
	annotationLine( FL, BL, white );
	annotationLine( BL, BR, white );
	annotationLine( BR, FR, white );
}

//-----------------------------------------------------------------------------
int NetPedestrian::serialize( RakNet::SerializeParameters *serializeParameters ) const
{
	int serializeOption = BaseClass::serialize( serializeParameters );
	return serializeOption;
}

//-----------------------------------------------------------------------------
void NetPedestrian::deserialize( RakNet::DeserializeParameters *deserializeParameters )
{
	BaseClass::deserialize( deserializeParameters );
}

//-----------------------------------------------------------------------------
void NetPedestrian::serializeConstruction( RakNet::BitStream *constructionBitstream )
{
	BaseClass::serializeConstruction( constructionBitstream );
}

//-----------------------------------------------------------------------------
bool NetPedestrian::deserializeConstruction( RakNet::BitStream *constructionBitstream )
{
	bool bConstruct = BaseClass::deserializeConstruction( constructionBitstream );
	return bConstruct;
}