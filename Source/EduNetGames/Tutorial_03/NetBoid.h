#ifndef __NETBOID_H__
#define __NETBOID_H__

#include "OpenSteerUT/AbstractVehicleUtilities.h"
#include "OpenSteerUT/VehicleClassIds.h"
#include "EduNetConnect/SimpleNetworkVehicle.h"

namespace OpenSteer{

		// Include names declared in the OpenSteer namespace into the
		// namespaces to search to find names.
		using namespace OpenSteer;
	//-------------------------------------------------------------------------


	typedef OpenSteer::AbstractProximityDatabase<AbstractVehicle*> ProximityDatabase;
	typedef OpenSteer::AbstractTokenForProximityDatabase<AbstractVehicle*> ProximityToken;



	//-------------------------------------------------------------------------

	class Boid : public SimpleNetworkVehicle
	{
		ET_DECLARE_BASE( SimpleNetworkVehicle )
	public:
		// type for a flock: an STL vector of Boid pointers
		typedef std::vector<Boid*> groupType;

		Boid ();
		~Boid ();

		OS_IMPLEMENT_CLASSNAME( Boid )

		void reset (void);
		void draw (void);
		void update (const float currentTime, const float elapsedTime);
		Vec3 steerToFlock (void);
		void sphericalWrapAround (void);
		void regenerateLocalSpace( const Vec3& newForward,
			const float elapsedTime );
		void regenerateLocalSpaceForTerrainFollowing (const Vec3& newForward,
			const float /* elapsedTime */);
		void annotateAvoidObstacle (const float minDistanceToCollision);

		virtual AbstractVehicle* cloneVehicle( 
			ProximityDatabase* ) const;

	

	#ifndef NO_LQ_BIN_STATS
		static size_t minNeighbors, maxNeighbors, totalNeighbors;
	#endif // NO_LQ_BIN_STATS


		static float worldRadius;
		void setParentPlugin( class BoidsPlugin* pPlugin);		
	private:
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
	typedef OpenSteer::VehicleClassIdMixin<Boid, ET_CID_NETBOID> TNetBoid;
}

#endif // __NETBOID_H__