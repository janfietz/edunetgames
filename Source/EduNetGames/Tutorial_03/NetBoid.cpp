#include "NetBoid.h"
#include "EduNetApplication/EduNetGames.h"
#include "OpenSteerUT/CameraPlugin.h"
#include "OpenSteer/UnusedParameter.h"
#include "BoidsPlugin.h"

// Include names declared in the OpenSteer namespace into the
// namespaces to search to find names.
using namespace OpenSteer;

//AVGroup Boid::neighbors;
float Boid::worldRadius = 50.0f;
//ObstacleGroup Boid::obstacles;

size_t Boid::minNeighbors = 0;
size_t Boid::maxNeighbors = 0;
size_t Boid::totalNeighbors = 0;

//-----------------------------------------------------------------------------
Boid::Boid()
{
	_movesPlanar = false;

	// reset all boid state
	reset ();	
}
//-----------------------------------------------------------------------------
Boid::~Boid ()
{
}

void Boid::setParentPlugin( BoidsPlugin* pPlugin)
{
	this->m_pkParentPlugin = pPlugin;
}

//-----------------------------------------------------------------------------
// reset state
void Boid::reset (void)
{
	// reset the vehicle
	BaseClass::reset ();

	// steering force is clipped to this magnitude
	setMaxForce (27);

	// velocity is clipped to this magnitude
	setMaxSpeed (9);

	// initial slow speed
	setSpeed (maxSpeed() * 0.3f);

	// randomize initial orientation
	regenerateOrthonormalBasisUF (RandomUnitVector ());

	// randomize initial position
	setPosition (RandomVectorInUnitRadiusSphere () * 20);

	// notify proximity database that our position has changed
	if( NULL != this->m_pkProximityToken )
	{
		m_pkProximityToken->updateForNewPosition(position());
	}
}

//-----------------------------------------------------------------------------
// draw this boid into the scene
void Boid::draw( const float currentTime, const float elapsedTime )
{
	BaseClass::draw( currentTime, elapsedTime );
	drawBasic3dSphericalVehicle (*this,
		(this->isRemoteObject()) ? gOrange : gGray70);
	// drawTrail ();
}

//-----------------------------------------------------------------------------
// per frame simulation update
void Boid::update (const float currentTime, const float elapsedTime)
{
	if( false == this->isRemoteObject() )
	{
		// wrap around to contrain boid within the spherical boundary
		sphericalWrapAround ();
	}
	BaseClass::update( currentTime, elapsedTime );

	// notify proximity database that our position has changed
	if( NULL != this->m_pkProximityToken )
	{
		m_pkProximityToken->updateForNewPosition(position());
	}
}

//-----------------------------------------------------------------------------
osVector3 Boid::determineCombinedSteering (const float elapsedTime)
{
	// steer to flock and avoid obstacles if any
	const Vec3 kSummedForce = steerToFlock();
	return kSummedForce;
}

//-----------------------------------------------------------------------------
// basic flocking
Vec3 Boid::steerToFlock (void)
{
	// avoid obstacles if needed
	// XXX this should probably be moved elsewhere
	const Vec3 avoidance = steerToAvoidObstacles (1.0f, allObstacles());
	if (avoidance != Vec3::zero) return avoidance;

	const float separationRadius =  5.0f;
	const float separationAngle  = -0.707f;
	const float separationWeight =  12.0f;

	const float alignmentRadius = 7.5f;
	const float alignmentAngle  = 0.7f;
	const float alignmentWeight = 8.0f;

	const float cohesionRadius = 9.0f;
	const float cohesionAngle  = -0.15f;
	const float cohesionWeight = 8.0f;

	const float maxRadius = maxXXX (separationRadius,
		maxXXX (alignmentRadius,
		cohesionRadius));

	// find all flockmates within maxRadius using proximity database
	neighbors.clear();
	m_pkProximityToken->findNeighbors (position(), maxRadius, neighbors);

#ifndef NO_LQ_BIN_STATS
	// maintain stats on max/min/ave neighbors per boids
	size_t count = neighbors.size();
	if (maxNeighbors < count) maxNeighbors = count;
	if (minNeighbors > count) minNeighbors = count;
	totalNeighbors += count;
#endif // NO_LQ_BIN_STATS

	// determine each of the three component behaviors of flocking
	const Vec3 separation = steerForSeparation (separationRadius,
		separationAngle,
		neighbors);
	const Vec3 alignment  = steerForAlignment  (alignmentRadius,
		alignmentAngle,
		neighbors);
	const Vec3 cohesion   = steerForCohesion   (cohesionRadius,
		cohesionAngle,
		neighbors);

	// apply weights to components (save in variables for annotation)
	const Vec3 separationW = separation * separationWeight;
	const Vec3 alignmentW = alignment * alignmentWeight;
	const Vec3 cohesionW = cohesion * cohesionWeight;

	// annotation
	// const float s = 0.1;
	// annotationLine (position, position + (separationW * s), gRed);
	// annotationLine (position, position + (alignmentW  * s), gOrange);
	// annotationLine (position, position + (cohesionW   * s), gYellow);

	const Vec3 kSummedForce = separationW + alignmentW + cohesionW;
	return kSummedForce;
}

//-----------------------------------------------------------------------------
// constrain this boid to stay within spherical boundary.
void Boid::sphericalWrapAround (void)
{
	// when outside the sphere
	if (position().length() > worldRadius)
	{
		// wrap around (teleport)
		setPosition (position().sphericalWrapAround (Vec3::zero,
			worldRadius));
		if (this == SimpleVehicle::selectedVehicle)
		{
			CameraPlugin::position3dCamera
				(*SimpleVehicle::selectedVehicle); 
			Camera::camera.doNotSmoothNextMove ();
		}
	}
}


// ---------------------------------------------- xxxcwr111704_terrain_following
// control orientation for this boid
void Boid::regenerateLocalSpace( const Vec3& newForward,
								 const float elapsedTime )
{
	// 3d flight with banking
	regenerateLocalSpaceForBanking( newForward, elapsedTime );

	// // follow terrain surface
	// regenerateLocalSpaceForTerrainFollowing( newForward, elapsedTime );
}

//-----------------------------------------------------------------------------
// XXX experiment:
// XXX   herd with terrain following
// XXX   special case terrain: a sphere at the origin, radius 40
void Boid::regenerateLocalSpaceForTerrainFollowing  (const Vec3& newForward,
													 const float /* elapsedTime */)
{

	// XXX this is special case code, these should be derived from arguments //
	const Vec3 surfaceNormal = position().normalized();                       //
	const Vec3 surfacePoint = surfaceNormal * 40.0f;                         //
	// XXX this is special case code, these should be derived from arguments //

	const Vec3 newUp = surfaceNormal;
	const Vec3 newPos = surfacePoint;
	const Vec3 newFor = newForward.perpendicularComponent(newUp);
// 	const float newSpeed = newVel.length();
// 	const Vec3 newFor = newVel / newSpeed;
// 
// 	setSpeed (newSpeed);
	setPosition( newPos );
	setUp( newUp );
	setForward( newFor );
	setUnitSideFromForwardAndUp();
}
// ---------------------------------------------- xxxcwr111704_terrain_following

//-----------------------------------------------------------------------------
// xxx perhaps this should be a call to a general purpose annotation for
// xxx "local xxx axis aligned box in XZ plane" -- same code in in
// xxx CaptureTheFlag.cpp
void Boid::annotateAvoidObstacle (const float minDistanceToCollision)
{
	const Vec3 boxSide = side() * radius();
	const Vec3 boxFront = forward() * minDistanceToCollision;
	const Vec3 FR = position() + boxFront - boxSide;
	const Vec3 FL = position() + boxFront + boxSide;
	const Vec3 BR = position()            - boxSide;
	const Vec3 BL = position()            + boxSide;
	const Color white (1,1,1);
	annotationLine (FR, FL, white);
	annotationLine (FL, BL, white);
	annotationLine (BL, BR, white);
	annotationLine (BR, FR, white);
}


const ObstacleGroup& Boid::allObstacles(void) const { return m_pkParentPlugin->allObstacles(); }
ObstacleGroup& Boid::allObstacles(void) { return m_pkParentPlugin->allObstacles(); }

//-----------------------------------------------------------------------------
AbstractVehicle* Boid::cloneVehicle( void ) const
{
	Boid* pkVehicle = ET_NEW Boid();
	pkVehicle->setParentPlugin( this->m_pkParentPlugin );
	return pkVehicle;
}