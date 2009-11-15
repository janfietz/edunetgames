#include "NetBoid.h"
#include "EduNetApplication/EduNetGames.h"
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
Boid::Boid (ProximityDatabase& pd)
{
	// allocate a token for this boid in the proximity database
	proximityToken = NULL;
	newPD (pd);

	// reset all boid state
	reset ();	
}
//-----------------------------------------------------------------------------
Boid::~Boid ()
{
	// delete this boid's token in the proximity database
	delete proximityToken;
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
	proximityToken->updateForNewPosition (position());
}

//-----------------------------------------------------------------------------
// draw this boid into the scene
void Boid::draw (void)
{
	drawBasic3dSphericalVehicle (*this,
		(this->isRemoteObject()) ? gOrange : gGray70);
	// drawTrail ();
}

//-----------------------------------------------------------------------------
// per frame simulation update
void Boid::update (const float currentTime, const float elapsedTime)
{
	OPENSTEER_UNUSED_PARAMETER(currentTime);

	if( false == this->isRemoteObject() )
	{
		// steer to flock and avoid obstacles if any
		applySteeringForce (steerToFlock (), elapsedTime);

		// wrap around to contrain boid within the spherical boundary
		sphericalWrapAround ();
	}

	// notify proximity database that our position has changed
	proximityToken->updateForNewPosition (position());
}

//-----------------------------------------------------------------------------
// basic flocking
Vec3 Boid::steerToFlock (void)
{
	// avoid obstacles if needed
	// XXX this should probably be moved elsewhere
	const Vec3 avoidance = steerToAvoidObstacles (1.0f, obstacles());
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
	proximityToken->findNeighbors (position(), maxRadius, neighbors);

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

	return separationW + alignmentW + cohesionW;
}

//-----------------------------------------------------------------------------
// constrain this boid to stay within sphereical boundary.
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
			OpenSteerDemo::position3dCamera
				(*SimpleVehicle::selectedVehicle); 
			Camera::camera.doNotSmoothNextMove ();
		}
	}
}


// ---------------------------------------------- xxxcwr111704_terrain_following
// control orientation for this boid
void Boid::regenerateLocalSpace (const Vec3& newVelocity,
								 const float elapsedTime)
{
	// 3d flight with banking
	regenerateLocalSpaceForBanking (newVelocity, elapsedTime);

	// // follow terrain surface
	// regenerateLocalSpaceForTerrainFollowing (newVelocity, elapsedTime);
}

//-----------------------------------------------------------------------------
// XXX experiment:
// XXX   herd with terrain following
// XXX   special case terrain: a sphere at the origin, radius 40
void Boid::regenerateLocalSpaceForTerrainFollowing  (const Vec3& newVelocity,
													 const float /* elapsedTime */)
{

	// XXX this is special case code, these should be derived from arguments //
	const Vec3 surfaceNormal = position().normalize();                       //
	const Vec3 surfacePoint = surfaceNormal * 40.0f;                         //
	// XXX this is special case code, these should be derived from arguments //

	const Vec3 newUp = surfaceNormal;
	const Vec3 newPos = surfacePoint;
	const Vec3 newVel = newVelocity.perpendicularComponent(newUp);
	const float newSpeed = newVel.length();
	const Vec3 newFor = newVel / newSpeed;

	setSpeed (newSpeed);
	setPosition (newPos);
	setUp (newUp);
	setForward (newFor);
	setUnitSideFromForwardAndUp ();
}
// ---------------------------------------------- xxxcwr111704_terrain_following

// switch to new proximity database -- just for demo purposes
void Boid::newPD (ProximityDatabase& pd)
{
	// delete this boid's token in the old proximity database
	delete proximityToken;

	// allocate a token for this boid in the proximity database
	proximityToken = pd.allocateToken (this);
}       

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


const ObstacleGroup& Boid::obstacles(void) const { return m_pkParentPlugin->obstacles(); }
ObstacleGroup& Boid::obstacles(void) { return m_pkParentPlugin->obstacles(); }

//-----------------------------------------------------------------------------
AbstractVehicle* Boid::cloneVehicle(
	ProximityDatabase* pkProximityDatabase ) const
{
	Boid* pkNewBoid = (NULL == pkProximityDatabase) ? new Boid() : new Boid( *pkProximityDatabase );
	pkNewBoid->setParentPlugin( this->m_pkParentPlugin );

	 return pkNewBoid;
}