// namespace OpenSteer
// {
// 
// }
#include "NetPedestrian.h"

using namespace OpenSteer;

namespace
{
	// ----------------------------------------------------------------------------


	// creates a path for the Plugin
	PolylineSegmentedPathwaySingleRadius* gTestPath = NULL;
	ObstacleGroup gObstacles;
	osVector3 gEndpoint0;
	osVector3 gEndpoint1;



}



// ----------------------------------------------------------------------------
/**
* Creates a path of the form of an eight. Data provided by Nick Porcino.
*/
PolylineSegmentedPathwaySingleRadius* getTestPath (void)
{
	if (gTestPath == NULL)
	{
		const float pathRadius = 2;

		const PolylineSegmentedPathwaySingleRadius::size_type pathPointCount = 16;
		// const float size = 30;
		const osVector3 pathPoints[pathPointCount] = {
			osVector3( -12.678730011f, 0.0144290002063f, 0.523285984993f ),
			osVector3( -10.447640419f, 0.0149269998074f, -3.44138407707f ),
			osVector3( -5.88988399506f, 0.0128290001303f, -4.1717581749f ),
			osVector3( 0.941263973713f, 0.00330199999735f, 0.350513994694f ),
			osVector3( 5.83484792709f, -0.00407700007781f, 6.56243610382f ),
			osVector3( 11.0144147873f, -0.0111180003732f, 10.175157547f ),
			osVector3( 15.9621419907f, -0.0129949999973f, 8.82364273071f ),
			osVector3( 18.697883606f, -0.0102310003713f, 2.42084693909f ),
			osVector3( 16.0552558899f, -0.00506500015035f, -3.57153511047f ),
			osVector3( 10.5450153351f, 0.00284500000998f, -9.92683887482f ),
			osVector3( 5.88374519348f, 0.00683500012383f, -8.51393127441f ),
			osVector3( 3.17790007591f, 0.00419700006023f, -2.33129906654f ),
			osVector3( 1.94371795654f, 0.00101799995173f, 2.78656601906f ),
			osVector3( -1.04967498779f, 0.000867999973707f, 5.57114219666f ),
			osVector3( -7.58111476898f, 0.00634300010279f, 6.13661909103f ),
			osVector3( -12.4111375809f, 0.0108559997752f, 3.5670940876f )
		};




		// ------------------------------------ xxxcwr11-1-04 fixing steerToAvoid

		gEndpoint0 = pathPoints[0];
		gEndpoint1 = pathPoints[pathPointCount-1];

		gTestPath = new PolylineSegmentedPathwaySingleRadius (pathPointCount,
			pathPoints,
			pathRadius,
			false);
	}
	return gTestPath;
}




AVGroup NetPedestrian::neighbors;
bool NetPedestrian::gWanderSwitch = true;
bool NetPedestrian::gUseDirectedPathFollowing = true;

//-----------------------------------------------------------------------------
NetPedestrian::NetPedestrian()
{

}


//-----------------------------------------------------------------------------
NetPedestrian::~NetPedestrian()
{
	// delete this boid's token in the proximity database
	delete proximityToken;
}



//-----------------------------------------------------------------------------
// constructor
NetPedestrian::NetPedestrian( ProximityDatabase& pd )
{
	// allocate a token for this boid in the proximity database
	proximityToken = NULL;
	newPD (pd);

	// reset Pedestrian state
	reset ();

	const char* pszClassName = this->getClassName();
}


//-----------------------------------------------------------------------------
// reset all instance state
void NetPedestrian::reset (void)
{
	// reset the vehicle 
	SimpleVehicle::reset ();

	// max speed and max steering force (maneuverability) 
	setMaxSpeed (2.0);
	setMaxForce (8.0);

	// initially stopped
	setSpeed (0);

	// size of bounding sphere, for obstacle avoidance, etc.
	setRadius (0.5); // width = 0.7, add 0.3 margin, take half

	// set the path for this Pedestrian to follow
	path = getTestPath ();

	// set initial position
	// (random point on path + random horizontal offset)
	const float d = path->length() * frandom01();
	const float r = path->radius();
	const osVector3 randomOffset = randomVectorOnUnitRadiusXZDisk () * r;
	setPosition (path->mapPathDistanceToPoint (d) + randomOffset);


	// randomize 2D heading
	randomizeHeadingOnXZPlane ();

	// pick a random direction for path following (upstream or downstream)
	pathDirection = (frandom01() > 0.5) ? -1 : +1;

	// trail parameters: 3 seconds with 60 points along the trail
	setTrailParameters (3, 60);

	// notify proximity database that our position has changed
	proximityToken->updateForNewPosition (position());
}

//-----------------------------------------------------------------------------
// per frame simulation update
void NetPedestrian::update (const float currentTime, const float elapsedTime)
{
	// apply steering force to our momentum
	applySteeringForce (determineCombinedSteering (elapsedTime),
		elapsedTime);

	// reverse direction when we reach an endpoint
	if (gUseDirectedPathFollowing)
	{
		const Color darkRed (0.7f, 0, 0);
		float const pathRadius = path->radius();

		if (osVector3::distance (position(), gEndpoint0) < pathRadius )
		{
			pathDirection = +1;
			annotationXZCircle (pathRadius, gEndpoint0, darkRed, 20);
		}
		if (osVector3::distance (position(), gEndpoint1) < pathRadius )
		{
			pathDirection = -1;
			annotationXZCircle (pathRadius, gEndpoint1, darkRed, 20);
		}
	}

	// annotation
	annotationVelocityAcceleration (5, 0);
	recordTrailVertex (currentTime, position());

	// notify proximity database that our position has changed
	proximityToken->updateForNewPosition (position());
}

//-----------------------------------------------------------------------------
// compute combined steering force: move forward, avoid obstacles
// or neighbors if needed, otherwise follow the path and wander
osVector3 NetPedestrian::determineCombinedSteering (const float elapsedTime)
{
	// move forward
	osVector3 steeringForce = forward();

	// probability that a lower priority behavior will be given a
	// chance to "drive" even if a higher priority behavior might
	// otherwise be triggered.
	const float leakThrough = 0.1f;

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
		// otherwise consider avoiding collisions with others
		osVector3 collisionAvoidance;
		const float caLeadTime = 3;

		// find all neighbors within maxRadius using proximity database
		// (radius is largest distance between vehicles traveling head-on
		// where a collision is possible within caLeadTime seconds.)
		const float maxRadius = caLeadTime * maxSpeed() * 2;
		neighbors.clear();
		proximityToken->findNeighbors (position(), maxRadius, neighbors);

		if (leakThrough < frandom01())
			collisionAvoidance =
			steerToAvoidNeighbors (caLeadTime, neighbors) * 10;

		// if collision avoidance is needed, do it
		if (collisionAvoidance != osVector3::zero)
		{
			steeringForce += collisionAvoidance;
		}
		else
		{
			// add in wander component (according to user switch)
			if (gWanderSwitch)
				steeringForce += steerForWander (elapsedTime);

			// do (interactively) selected type of path following
			const float pfLeadTime = 3;
			const osVector3 pathFollow =
				(gUseDirectedPathFollowing ?
				steerToFollowPath (pathDirection, pfLeadTime, *path) :
			steerToStayOnPath (pfLeadTime, *path));

			// add in to steeringForce
			steeringForce += pathFollow * 0.5;
		}
	}

	// return steering constrained to global XZ "ground" plane
	return steeringForce.setYtoZero ();
}


//-----------------------------------------------------------------------------
// draw this pedestrian into scene
void NetPedestrian::draw( const float currentTime, const float elapsedTime )
{
	drawBasic2dCircularVehicle (*this, gGray50);
	drawTrail ();
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
	const osVector3 boundaryOffset = (onPath - future).normalize() * outside;
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
		draw2dTextAt3dLocation (*string, location, color, drawGetWindowWidth(), drawGetWindowHeight());
}


//-----------------------------------------------------------------------------
// (parameter names commented out to prevent compiler warning from "-W")
void NetPedestrian::annotateAvoidNeighbor (const AbstractVehicle& threat,
							const float /*steer*/,
							const osVector3& ourFuture,
							const osVector3& threatFuture)
{
	const Color green (0.15f, 0.6f, 0.0f);

	annotationLine (position(), ourFuture, green);
	annotationLine (threat.position(), threatFuture, green);
	annotationLine (ourFuture, threatFuture, gRed);
	annotationXZCircle (radius(), ourFuture,    green, 12);
	annotationXZCircle (radius(), threatFuture, green, 12);
}

//-----------------------------------------------------------------------------
// xxx perhaps this should be a call to a general purpose annotation for
// xxx "local xxx axis aligned box in XZ plane" -- same code in in
// xxx CaptureTheFlag.cpp
void NetPedestrian::annotateAvoidObstacle (const float minDistanceToCollision)
{
	const osVector3 boxSide = side() * radius();
	const osVector3 boxFront = forward() * minDistanceToCollision;
	const osVector3 FR = position() + boxFront - boxSide;
	const osVector3 FL = position() + boxFront + boxSide;
	const osVector3 BR = position()            - boxSide;
	const osVector3 BL = position()            + boxSide;
	const Color white (1,1,1);
	annotationLine (FR, FL, white);
	annotationLine (FL, BL, white);
	annotationLine (BL, BR, white);
	annotationLine (BR, FR, white);
}

//-----------------------------------------------------------------------------
// switch to new proximity database -- just for demo purposes
void NetPedestrian::newPD (ProximityDatabase& pd)
{
	// delete this boid's token in the old proximity database
	delete proximityToken;

	// allocate a token for this boid in the proximity database
	proximityToken = pd.allocateToken (this);
}

