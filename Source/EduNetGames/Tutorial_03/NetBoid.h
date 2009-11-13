#ifndef __NETBOID_H__
#define __NETBOID_H__

#include "OpenSteerUT/SimpleNetworkVehicle.h"

namespace OpenSteer{

		// Include names declared in the OpenSteer namespace into the
		// namespaces to search to find names.
		using namespace OpenSteer;
	// ----------------------------------------------------------------------------


	typedef OpenSteer::AbstractProximityDatabase<AbstractVehicle*> ProximityDatabase;
	typedef OpenSteer::AbstractTokenForProximityDatabase<AbstractVehicle*> ProximityToken;



	// ----------------------------------------------------------------------------

	class Boid : public SimpleNetworkVehicle
	{
		ET_DECLARE_BASE( SimpleNetworkVehicle )
	public:
		// type for a flock: an STL vector of Boid pointers
		typedef std::vector<Boid*> groupType;

		Boid (){};
		Boid (ProximityDatabase& pd);		 
		~Boid ();

		OS_IMPLEMENT_CLASSNAME( Boid )

		void reset (void);
		void draw (void);
		void update (const float currentTime, const float elapsedTime);
		Vec3 steerToFlock (void);
		void sphericalWrapAround (void);
		void regenerateLocalSpace (const Vec3& newVelocity,
			const float elapsedTime);
		void regenerateLocalSpaceForTerrainFollowing  (const Vec3& newVelocity,
			const float /* elapsedTime */);
		void newPD (ProximityDatabase& pd);
		void annotateAvoidObstacle (const float minDistanceToCollision);

	#ifndef NO_LQ_BIN_STATS
		static size_t minNeighbors, maxNeighbors, totalNeighbors;
	#endif // NO_LQ_BIN_STATS

		// a pointer to this boid's interface object for the proximity database
		ProximityToken* proximityToken;

		static float worldRadius;
		//private:
		// group of all obstacles to be avoided by each Boid
		//        static ObstacleGroup obstacles;
		const ObstacleGroup& obstacles(void) const;
		ObstacleGroup& obstacles(void);




		// allocate one and share amoung instances just to save memory usage
		// (change to per-instance allocation to be more MP-safe)
		//static AVGroup neighbors;
		AVGroup neighbors;

		class BoidsPlugin* m_pkParentPlugin;

	};
}

#endif // __NETBOID_H__