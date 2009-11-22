//-----------------------------------------------------------------------------
// Copyright(c) 2009, Jan Fietz, Cyrus Preuss
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
//(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON 
// ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
//(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, 
// EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//-----------------------------------------------------------------------------

#include "NetCtfVehicles.h"
#include "NetCtfGameLogic.h"

using namespace OpenSteer;

TNetCtfBaseVehicle gNetCtfBaseVehicle;
TNetCtfSeekerVehicle gNetCtfSeekerVehicle;
TNetCtfEnemyVehicle gNetCtfEnemyVehicle;

//-----------------------------------------------------------------------------
namespace	{
	//-------------------------------------------------------------------------
	// globals
	//(perhaps these should be member variables of a Vehicle or Plugin class)

	const float gMinStartRadius = 30;
	const float gMaxStartRadius = 40;

	const float gBrakingRate = 0.75;

	const Color evadeColor    (0.6f, 0.6f, 0.3f); // annotation
	const Color seekColor     (0.3f, 0.6f, 0.6f); // annotation
	const Color clearPathColor(0.3f, 0.6f, 0.3f); // annotation

	const float gAvoidancePredictTimeMin  = 0.9f;
	const float gAvoidancePredictTimeMax  = 2;
	float gAvoidancePredictTime = gAvoidancePredictTimeMin;
}

const int NetCtfBaseVehicle::maxObstacleCount = 100;

#pragma warning(push)
#pragma warning(disable: 4355) // warning C4355: 'this' : used in base member initializer list

//-----------------------------------------------------------------------------
NetCtfBaseVehicle::NetCtfBaseVehicle()
{
	reset();
}

//-----------------------------------------------------------------------------
NetCtfBaseVehicle::NetCtfBaseVehicle( OpenSteer::ProximityDatabase* pd )
{

}

#pragma warning(pop)

//-----------------------------------------------------------------------------
NetCtfBaseVehicle::~NetCtfBaseVehicle()
{

}

//-----------------------------------------------------------------------------
AbstractVehicle* NetCtfBaseVehicle::cloneVehicle( ProximityDatabase* pkProximityDatabase ) const
{
	AbstractVehicle* pkVehicle = ET_NEW NetCtfBaseVehicle();
	pkVehicle->allocateProximityToken( pkProximityDatabase );
	return pkVehicle;
}

//-----------------------------------------------------------------------------
void NetCtfBaseVehicle::reset( void )
{
	BaseClass::reset();  // reset the vehicle 

	setSpeed(3);             // speed along Forward direction.
	setMaxForce(3.0);        // steering force is clipped to this magnitude
	setMaxSpeed(3.0);        // velocity is clipped to this magnitude

	avoiding = false;         // not actively avoiding

	randomizeStartingPositionAndHeading();  // new starting position
}

//-----------------------------------------------------------------------------
// draw this character/vehicle into the scene
void NetCtfBaseVehicle::draw( const float currentTime, const float elapsedTime )
{
	BaseClass::draw( currentTime, elapsedTime );
	drawBasic2dCircularVehicle(*this, bodyColor);
	drawTrail();
}

//-----------------------------------------------------------------------------
// update
void  NetCtfBaseVehicle::update( const float currentTime, const float elapsedTime )
{
	BaseClass::update( currentTime,  elapsedTime );
}

//-----------------------------------------------------------------------------
void NetCtfBaseVehicle::randomizeStartingPositionAndHeading( void )
{
	// randomize position on a ring between inner and outer radii
	// centered around the home base
	const float rRadius = frandom2(gMinStartRadius, gMaxStartRadius);
	const Vec3 randomOnRing = RandomUnitVectorOnXZPlane() * rRadius;
	setPosition(NetCtfGameLogic::ms_kHomeBaseCenter + randomOnRing);

	// are we are too close to an obstacle?
	if(minDistanceToObstacle(position()) < radius()*5)
	{
		// if so, retry the randomization(this recursive call may not return
		// if there is too little free space)
		randomizeStartingPositionAndHeading();
	}
	else
	{
		// otherwise, if the position is OK, randomize 2D heading
		randomizeHeadingOnXZPlane();
	}
}

//-----------------------------------------------------------------------------
// dynamic obstacle registry
//
// xxx need to combine guts of addOneObstacle and minDistanceToObstacle,
// xxx perhaps by having the former call the latter, or change the latter to
// xxx be "nearestObstacle": give it a position, it finds the nearest obstacle
// xxx(but remember: obstacles a not necessarilty spheres!)


int NetCtfBaseVehicle::obstacleCount = -1; // this value means "uninitialized"
SOG NetCtfBaseVehicle::allObstacles;


#define testOneObstacleOverlap(radius, center)               \
{                                                            \
	float d = Vec3::distance(c, center);                    \
	float clearance = d -(r +(radius));                    \
	if(minClearance > clearance) minClearance = clearance;  \
}


//-----------------------------------------------------------------------------
void NetCtfBaseVehicle::initializeObstacles( void )
{
	// start with 40% of possible obstacles
	if(obstacleCount == -1)
	{
		obstacleCount = 0;
		for(int i = 0; i <(maxObstacleCount * 0.4); i++) addOneObstacle();
	}
}

//-----------------------------------------------------------------------------
void NetCtfBaseVehicle::addOneObstacle( void )
{
	if(obstacleCount < maxObstacleCount)
	{
		// pick a random center and radius,
		// loop until no overlap with other obstacles and the home base
		float r;
		Vec3 c;
		float minClearance;
		const float requiredClearance = gNetCtfSeekerVehicle.radius() * 4; // 2 x diameter
		do
		{
			r = frandom2(1.5, 4);
			c = randomVectorOnUnitRadiusXZDisk() * gMaxStartRadius * 1.1f;
			minClearance = FLT_MAX;

			for(SOI so = allObstacles.begin(); so != allObstacles.end(); so++)
			{
				testOneObstacleOverlap((**so).radius,(**so).center);
			}

			testOneObstacleOverlap(NetCtfGameLogic::ms_fHomeBaseRadius - requiredClearance,
				NetCtfGameLogic::ms_kHomeBaseCenter);
		}
		while(minClearance < requiredClearance);

		// add new non-overlapping obstacle to registry
		allObstacles.push_back(new SphereObstacle(r, c));
		obstacleCount++;
	}
}

//-----------------------------------------------------------------------------
float NetCtfBaseVehicle::minDistanceToObstacle(const Vec3& point)
{
	float r = 0;
	Vec3 c = point;
	float minClearance = FLT_MAX;
	for(SOI so = allObstacles.begin(); so != allObstacles.end(); so++)
	{
		testOneObstacleOverlap((**so).radius,(**so).center);
	}
	return minClearance;
}

//-----------------------------------------------------------------------------
void NetCtfBaseVehicle::removeOneObstacle( void )
{
	if(obstacleCount > 0)
	{
		obstacleCount--;
		allObstacles.pop_back();
	}
}

//-----------------------------------------------------------------------------
// xxx perhaps this should be a call to a general purpose annotation
// xxx for "local xxx axis aligned box in XZ plane" -- same code in in
// xxx Pedestrian.cpp
void NetCtfBaseVehicle::annotateAvoidObstacle(const float minDistanceToCollision)
{
	const Vec3 boxSide = side() * radius();
	const Vec3 boxFront = forward() * minDistanceToCollision;
	const Vec3 FR = position() + boxFront - boxSide;
	const Vec3 FL = position() + boxFront + boxSide;
	const Vec3 BR = position()            - boxSide;
	const Vec3 BL = position()            + boxSide;
	const Color white(1,1,1);
	annotationLine(FR, FL, white);
	annotationLine(FL, BL, white);
	annotationLine(BL, BR, white);
	annotationLine(BR, FR, white);
}

//-----------------------------------------------------------------------------
AbstractVehicle* NetCtfSeekerVehicle::cloneVehicle( ProximityDatabase* pkProximityDatabase ) const
{
	AbstractVehicle* pkVehicle = ET_NEW NetCtfSeekerVehicle();
	pkVehicle->allocateProximityToken( pkProximityDatabase );
	return pkVehicle;
}

//-----------------------------------------------------------------------------
void NetCtfSeekerVehicle::reset( void )
{
	BaseClass::reset();
	bodyColor.set(0.4f, 0.4f, 0.6f); // blueish
	state = running;
	evading = false;
}

//-----------------------------------------------------------------------------
// are there any enemies along the corridor between us and the goal?
bool NetCtfSeekerVehicle::clearPathToGoal( void )
{
	const float sideThreshold = radius() * 8.0f;
	const float behindThreshold = radius() * 2.0f;

	const Vec3 goalOffset = NetCtfGameLogic::ms_kHomeBaseCenter - position();
	const float goalDistance = goalOffset.length();
	const Vec3 goalDirection = goalOffset / goalDistance;

	const bool goalIsAside = isAside(NetCtfGameLogic::ms_kHomeBaseCenter, 0.5);

	// for annotation: loop over all and save result, instead of early return 
	bool xxxReturn = true;

	size_t uiEnemyCount = this->m_kEnemies.size();

	// loop over enemies
	for(size_t i = 0; i < uiEnemyCount; ++i)
	{
		// short name for this enemy
		const AbstractVehicle& e = *this->m_kEnemies[i];
		const float eDistance = Vec3::distance(position(), e.position());
		const float timeEstimate = 0.3f * eDistance / e.speed(); //xxx
		const Vec3 eFuture = e.predictFuturePosition(timeEstimate);
		const Vec3 eOffset = eFuture - position();
		const float alongCorridor = goalDirection.dot(eOffset);
		const bool inCorridor =((alongCorridor > -behindThreshold) && 
			(alongCorridor < goalDistance));
		const float eForwardDistance = forward().dot(eOffset);

		// xxx temp move this up before the conditionals
		annotationXZCircle(e.radius(), eFuture, clearPathColor, 20); //xxx

		// consider as potential blocker if within the corridor
		if(inCorridor)
		{
			const Vec3 perp = eOffset -(goalDirection * alongCorridor);
			const float acrossCorridor = perp.length();
			if(acrossCorridor < sideThreshold)
			{
				// not a blocker if behind us and we are perp to corridor
				const float eFront = eForwardDistance + e.radius();

				//annotationLine(position, forward*eFront, gGreen); // xxx
				//annotationLine(e.position, forward*eFront, gGreen); // xxx

				// xxx
				// std::ostringstream message;
				// message << "eFront = " << std::setprecision(2)
				//         << std::setiosflags(std::ios::fixed) << eFront << std::ends;
				// draw2dTextAt3dLocation(*message.str(), eFuture, gWhite);

				const bool eIsBehind = eFront < -behindThreshold;
				const bool eIsWayBehind = eFront <(-2 * behindThreshold);
				const bool safeToTurnTowardsGoal =
					((eIsBehind && goalIsAside) || eIsWayBehind);

				if(! safeToTurnTowardsGoal)
				{
					// this enemy blocks the path to the goal, so return false
					annotationLine(position(), e.position(), clearPathColor);
					// return false;
					xxxReturn = false;
				}
			}
		}
	}

	// no enemies found along path, return true to indicate path is clear
	// clearPathAnnotation(sideThreshold, behindThreshold, goalDirection);
	// return true;
	//if(xxxReturn)
	clearPathAnnotation(sideThreshold, behindThreshold, goalDirection);
	return xxxReturn;
}

//-----------------------------------------------------------------------------
void NetCtfSeekerVehicle::clearPathAnnotation(const float sideThreshold,
									 const float behindThreshold,
									 const Vec3& goalDirection)
{
	const Vec3 behindSide = side() * sideThreshold;
	const Vec3 behindBack = forward() * -behindThreshold;
	const Vec3 pbb = position() + behindBack;
	const Vec3 gun = localRotateForwardToSide(goalDirection);
	const Vec3 gn = gun * sideThreshold;
	const Vec3 hbc = NetCtfGameLogic::ms_kHomeBaseCenter;
	annotationLine(pbb + gn,         hbc + gn,         clearPathColor);
	annotationLine(pbb - gn,         hbc - gn,         clearPathColor);
	annotationLine(hbc - gn,         hbc + gn,         clearPathColor);
	annotationLine(pbb - behindSide, pbb + behindSide, clearPathColor);
}

//-----------------------------------------------------------------------------
Vec3 NetCtfSeekerVehicle::steerToEvadeAllDefenders( void )
{
	Vec3 evade(0, 0, 0);
	const float goalDistance = Vec3::distance(NetCtfGameLogic::ms_kHomeBaseCenter, position());

	size_t uiEnemyCount = this->m_kEnemies.size();
	// sum up weighted evasion
	// loop over enemies
	for(size_t i = 0; i < uiEnemyCount; ++i)
	{
		const AbstractVehicle& e = *this->m_kEnemies[i];
		const Vec3 eOffset = e.position() - position();
		const float eDistance = eOffset.length();

		const float eForwardDistance = forward().dot(eOffset);
		const float behindThreshold = radius() * 2;
		const bool behind = eForwardDistance < behindThreshold;
		if((!behind) ||(eDistance < 5))
		{
			if(eDistance <(goalDistance * 1.2)) //xxx
			{
				// const float timeEstimate = 0.5f * eDistance / e.speed;//xxx
				const float timeEstimate = 0.15f * eDistance / e.speed();//xxx
				const Vec3 future =
					e.predictFuturePosition(timeEstimate);

				annotationXZCircle(e.radius(), future, evadeColor, 20); // xxx

				const Vec3 offset = future - position();
				const Vec3 lateral = offset.perpendicularComponent(forward());
				const float d = lateral.length();
				const float weight = -1000 /(d * d);
				evade +=(lateral / d) * weight;
			}
		}
	}
	return evade;
}

//-----------------------------------------------------------------------------
Vec3 NetCtfSeekerVehicle::XXXsteerToEvadeAllDefenders( void )
{
	// sum up weighted evasion
	Vec3 evade(0, 0, 0);
	size_t uiEnemyCount = this->m_kEnemies.size();

	// loop over enemies
	for(size_t i = 0; i < uiEnemyCount; ++i)
	{
		// short name for this enemy
		const AbstractVehicle& e = *this->m_kEnemies[i];
		const Vec3 eOffset = e.position() - position();
		const float eDistance = eOffset.length();

		// xxx maybe this should take into account e's heading? xxx
		const float timeEstimate = 0.5f * eDistance / e.speed(); //xxx
		const Vec3 eFuture = e.predictFuturePosition(timeEstimate);

		// annotation
		annotationXZCircle(e.radius(), eFuture, evadeColor, 20);

		// steering to flee from eFuture(enemy's future position)
		const Vec3 flee = xxxsteerForFlee(eFuture);

		const float eForwardDistance = forward().dot(eOffset);
		const float behindThreshold = radius() * -2;

		const float distanceWeight = 4 / eDistance;
		const float forwardWeight =((eForwardDistance > behindThreshold) ?
			1.0f : 0.5f);

		const Vec3 adjustedFlee = flee * distanceWeight * forwardWeight;

		evade += adjustedFlee;
	}
	return evade;
}

//-----------------------------------------------------------------------------
Vec3 NetCtfSeekerVehicle::steeringForSeeker( void )
{
	// determine if obstacle avodiance is needed
	const bool clearPath = clearPathToGoal();
	adjustObstacleAvoidanceLookAhead(clearPath);
	const Vec3 obstacleAvoidance =
		steerToAvoidObstacles(gAvoidancePredictTime,
		(ObstacleGroup&) allObstacles);

	// saved for annotation
	avoiding =(obstacleAvoidance != Vec3::zero);

	if(avoiding)
	{
		// use pure obstacle avoidance if needed
		return obstacleAvoidance;
	}
	else
	{
		// otherwise seek home base and perhaps evade defenders
		const Vec3 seek = xxxsteerForSeek(NetCtfGameLogic::ms_kHomeBaseCenter);
		if(clearPath)
		{
			// we have a clear path(defender-free corridor), use pure seek

			// xxx experiment 9-16-02
			Vec3 s = limitMaxDeviationAngle(seek, 0.707f, forward());

			annotationLine(position(), position() +(s * 0.2f), seekColor);
			return s;
		}
		else
		{
			if(0) // xxx testing new evade code xxx
			{
				// combine seek and(forward facing portion of) evasion
				const Vec3 evade = steerToEvadeAllDefenders();
				const Vec3 steer = 
					seek + limitMaxDeviationAngle(evade, 0.5f, forward());

				// annotation: show evasion steering force
				annotationLine(position(),position()+(steer*0.2f),evadeColor);
				return steer;
			}
			else

			{
				const Vec3 evade = XXXsteerToEvadeAllDefenders();
				const Vec3 steer = limitMaxDeviationAngle(seek + evade,
					0.707f, forward());

				annotationLine(position(),position()+seek, gRed);
				annotationLine(position(),position()+evade, gGreen);

				// annotation: show evasion steering force
				annotationLine(position(),position()+(steer*0.2f),evadeColor);
				return steer;
			}
		}
	}
}

//-----------------------------------------------------------------------------
// adjust obstacle avoidance look ahead time: make it large when we are far
// from the goal and heading directly towards it, make it small otherwise.
void NetCtfSeekerVehicle::adjustObstacleAvoidanceLookAhead(const bool clearPath)
{
	if(clearPath)
	{
		evading = false;
		const float goalDistance = Vec3::distance(NetCtfGameLogic::ms_kHomeBaseCenter,position());
		const bool headingTowardGoal = isAhead(NetCtfGameLogic::ms_kHomeBaseCenter, 0.98f);
		const bool isNear =(goalDistance/speed()) < gAvoidancePredictTimeMax;
		const bool useMax = headingTowardGoal && !isNear;
		gAvoidancePredictTime =
			(useMax ? gAvoidancePredictTimeMax : gAvoidancePredictTimeMin);
	}
	else
	{
		evading = true;
		gAvoidancePredictTime = gAvoidancePredictTimeMin;
	}
}

//-----------------------------------------------------------------------------
void NetCtfSeekerVehicle::updateState(const float currentTime)
{
	// if we reach the goal before being tagged, switch to atGoal state
	if(state == running)
	{
		const float baseDistance = Vec3::distance(position(),NetCtfGameLogic::ms_kHomeBaseCenter);
		if(baseDistance <(radius() + NetCtfGameLogic::ms_fHomeBaseRadius)) state = atGoal;
	}

	// update lastRunningTime(holds off reset time)
	if(state == running)
	{
		lastRunningTime = currentTime;
	}
	else
	{
#if ET_ACTIVE_CTF_HACK
		const float resetDelay = 4;
		const float resetTime = lastRunningTime + resetDelay;
		if( currentTime > resetTime ) 
		{
			// xxx a royal hack(should do this internal to CTF):
			OpenSteerDemo::queueDelayedResetPluginXXX();
		}
#endif
	}
}

//-----------------------------------------------------------------------------
const char* NetCtfSeekerVehicle::getSeekerStateString( void ) const
{
	// select string describing current seeker state
	const char* seekerStateString = "";
	switch(state)
	{
	case running:
		if(avoiding)
			seekerStateString = "avoid obstacle";
		else if(evading)
			seekerStateString = "seek and evade";
		else
			seekerStateString = "seek goal";
		break;
	case tagged: 
		seekerStateString = "tagged"; 
		break;
	case atGoal: 
		seekerStateString = "reached goal"; 
		break;
	}
	return seekerStateString;
}

//-----------------------------------------------------------------------------
void NetCtfSeekerVehicle::draw( const float currentTime, const float elapsedTime )
{
	// first call the draw method in the base class
	BaseClass::draw( currentTime, elapsedTime );

	// annote seeker with its state as text
	const Vec3 textOrigin = position() + Vec3(0, 0.25, 0);
	std::ostringstream annote;
	annote << this->getSeekerStateString() << std::endl;
	annote << std::setprecision(2) << std::setiosflags(std::ios::fixed)
		<< speed() << std::ends;
	draw2dTextAt3dLocation( annote, textOrigin, gWhite, drawGetWindowWidth(), drawGetWindowHeight());

	this->drawHomeBase();
}

//-----------------------------------------------------------------------------
void NetCtfSeekerVehicle::drawHomeBase( void ) const
{
	const Vec3 up (0, 0.01f, 0);
	const Color atColor (0.3f, 0.3f, 0.5f);
	const Color noColor = gGray50;
	// TODO move to seeker himself
	const bool reached = this->state == NetCtfSeekerVehicle::atGoal;
	const Color baseColor = (reached ? atColor : noColor);
	drawXZDisk( NetCtfGameLogic::ms_fHomeBaseRadius,    NetCtfGameLogic::ms_kHomeBaseCenter, baseColor, 40 );
	drawXZDisk( NetCtfGameLogic::ms_fHomeBaseRadius/15, NetCtfGameLogic::ms_kHomeBaseCenter + up, gBlack, 20 );
}

//-----------------------------------------------------------------------------
// compute combined steering force: move forward, avoid obstacles
// or neighbors if needed, otherwise follow the path and wander
osVector3 NetCtfSeekerVehicle::determineCombinedSteering (const float elapsedTime)
{
	if( this->isRemoteObject() )
	{
		return this->lastSteeringForce();
	}

	// determine and apply steering/braking forces
	osVector3 steeringForce(0, 0, 0);;
	if(state == running)
	{
		steeringForce = steeringForSeeker();
	}
	else
	{
		applyBrakingForce( gBrakingRate, elapsedTime );
	}

	this->setLastSteeringForce( steeringForce.setYtoZero () );

	// return steering constrained to global XZ "ground" plane
	return this->lastSteeringForce();
}

//-----------------------------------------------------------------------------
// update method for goal seeker
void NetCtfSeekerVehicle::update(const float currentTime, const float elapsedTime)
{
	// do behavioral state transitions, as needed
	this->updateState(currentTime);
	BaseClass::update( currentTime, elapsedTime );
}

//-----------------------------------------------------------------------------
AbstractVehicle* NetCtfEnemyVehicle::cloneVehicle( ProximityDatabase* pkProximityDatabase ) const
{
	AbstractVehicle* pkVehicle = ET_NEW NetCtfEnemyVehicle();
	pkVehicle->allocateProximityToken( pkProximityDatabase );
	return pkVehicle;
}

//-----------------------------------------------------------------------------
void NetCtfEnemyVehicle::reset( void )
{
	BaseClass::reset();
	bodyColor.set(0.6f, 0.4f, 0.4f); // redish
	this->m_pkSeeker = NULL;
}

//-----------------------------------------------------------------------------
// compute combined steering force: move forward, avoid obstacles
// or neighbors if needed, otherwise follow the path and wander
osVector3 NetCtfEnemyVehicle::determineCombinedSteering (const float elapsedTime)
{
	if( this->isRemoteObject() )
	{
		return this->lastSteeringForce();
	}
	osVector3 steeringForce(0, 0, 0);

	if( NULL == this->m_pkSeeker )
	{
		this->applyBrakingForce( gBrakingRate, elapsedTime );
	}
	else
	{
		// determine upper bound for pursuit prediction time
		const float seekerToGoalDist = Vec3::distance( NetCtfGameLogic::ms_kHomeBaseCenter,
			this->m_pkSeeker->position() );
		const float adjustedDistance = seekerToGoalDist - radius()-NetCtfGameLogic::ms_fHomeBaseRadius;
		const float seekerToGoalTime =((adjustedDistance < 0 ) ?
			0 :
		(adjustedDistance/this->m_pkSeeker->speed()));
		const float maxPredictionTime = seekerToGoalTime * 0.9f;

		// determine steering(pursuit, obstacle avoidance, or braking)
		if(this->m_pkSeeker->state == running)
		{
			const Vec3 avoidance =
				steerToAvoidObstacles(gAvoidancePredictTimeMin,
				(ObstacleGroup&) allObstacles);

			// saved for annotation
			avoiding =(avoidance == Vec3::zero);

			if(avoiding)
				steeringForce = steerForPursuit(*this->m_pkSeeker, maxPredictionTime);
			else
				steeringForce = avoidance;
		}
		else
		{
			applyBrakingForce( gBrakingRate, elapsedTime );
		}
	}

	this->setLastSteeringForce( steeringForce.setYtoZero () );

	// return steering constrained to global XZ "ground" plane
	return this->lastSteeringForce();
}

//-----------------------------------------------------------------------------
void NetCtfEnemyVehicle::update( const float currentTime, const float elapsedTime )
{
	BaseClass::update( currentTime, elapsedTime );
	// detect and record interceptions("tags") of seeker
	if( NULL != this->m_pkSeeker )
	{
		const float seekerToMeDist = Vec3::distance(position(), 
			this->m_pkSeeker->position());
		const float sumOfRadii = radius() + this->m_pkSeeker->radius();
		if(seekerToMeDist < sumOfRadii)
		{
			if(this->m_pkSeeker->state == running) this->m_pkSeeker->state = tagged;

			// annotation:
			if(this->m_pkSeeker->state == tagged)
			{
				const Color color(0.8f, 0.5f, 0.5f);
				annotationXZDisk(sumOfRadii,
					(position() + this->m_pkSeeker->position()) / 2,
					color,
					20);
			}
		}
	}
}
