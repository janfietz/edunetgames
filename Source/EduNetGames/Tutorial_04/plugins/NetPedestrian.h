#ifndef __NETPEDESTRIAN_H__
#define __NETPEDESTRIAN_H__

#include "OpenSteerExtras/SimpleNetworkVehicle.h"

//-----------------------------------------------------------------------------
class NetPedestrian : public OpenSteer::SimpleNetworkVehicle
{
	ET_DECLARE_BASE( SimpleNetworkVehicle )
public:
	// type for a group of Pedestrians
	typedef std::vector<NetPedestrian*> groupType;

	NetPedestrian();
	NetPedestrian( OpenSteer::ProximityDatabase& pd );
	virtual ~NetPedestrian();


	virtual void reset( void );
	virtual void update( const float currentTime, const float elapsedTime);
	virtual osVector3 determineCombinedSteering( const float elapsedTime );
	virtual void draw( void );

	virtual void annotatePathFollowing( const osVector3& future,
		const osVector3& onPath,
		const osVector3& target,
		const float outside );

	virtual void annotateAvoidCloseNeighbor( const AbstractVehicle& other,
		const float /*additionalDistance*/);

	virtual void annotateAvoidNeighbor (const AbstractVehicle& threat,
		const float /*steer*/,
		const osVector3& ourFuture,
		const osVector3& threatFuture );

	virtual void annotateAvoidObstacle( const float minDistanceToCollision );
	virtual void newPD( OpenSteer::ProximityDatabase& pd );

	static bool gWanderSwitch;
	static bool gUseDirectedPathFollowing;

private:

	// a pointer to this boid's interface object for the proximity database
	OpenSteer::ProximityToken* proximityToken;

	// allocate one and share amoung instances just to save memory usage
	// (change to per-instance allocation to be more MP-safe)
	static OpenSteer::AVGroup neighbors;

	// path to be followed by this pedestrian
	// XXX Ideally this should be a generic Pathway, but we use the
	// XXX getTotalPathLength and radius methods (currently defined only
	// XXX on PolylinePathway) to set random initial positions.  Could
	// XXX there be a "random position inside path" method on Pathway?
	OpenSteer::PolylineSegmentedPathwaySingleRadius* path;

	// direction for path following (upstream or downstream)
	int pathDirection;


};

OpenSteer::PolylineSegmentedPathwaySingleRadius* getTestPath (void);


#endif //  __NETPEDESTRIAN_H__
