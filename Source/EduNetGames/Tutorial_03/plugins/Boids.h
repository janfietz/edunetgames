#ifndef BOID_H
#define BOID_H

#include "OpenSteerExtras/SimpleNetworkVehicle.h"

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

		Boid (ProximityDatabase& pd);		 
        ~Boid ();

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

		class BoidsPlugIn* m_pkParentPlugin;

	 };

	 // ----------------------------------------------------------------------------
	// JF ++
	class BoidFactory
	{
	public:

		virtual Boid* CreateBoid( ProximityDatabase& pd )
		{
			return new Boid(pd);
		};

		virtual void DestroyBoid( const Boid* boid )
		{
			delete boid;
		};

	};
	// JF --

	// enumerate demos of various constraints on the flock
	enum EBoidConstraintType {
		EBoidConstraintType_none,
		EBoidConstraintType_insideSphere,
		EBoidConstraintType_outsideSphere,
		EBoidConstraintType_outsideSpheres,
		EBoidConstraintType_outsideSpheresNoBig,
		EBoidConstraintType_rectangle,
		EBoidConstraintType_rectangleNoBig,
		EBoidConstraintType_outsideBox,
		EBoidConstraintType_insideBox
	};

	// ----------------------------------------------------------------------------
    // PlugIn for OpenSteerDemo
	class BoidsPlugIn : public PlugIn
    {
		ET_DECLARE_BASE(PlugIn);
    public:
		BoidsPlugIn (bool bAddToRegistry = true):BaseClass(bAddToRegistry){};
		virtual ~BoidsPlugIn() {} // be more "nice" to avoid a compiler warning
		
		const char* name (void) const {return "Boids";}

        float selectionOrderSortKey (void) const {return 0.03f;}

		void open (void);
		void close (void);
		void reset (void);
		void update (const float currentTime, const float elapsedTime);
		void redraw (const float currentTime, const float elapsedTime);

		void nextPD (void);
		void handleFunctionKeys (int keyNumber);
		void printLQbinStats (void);
		void printMiniHelpForFunctionKeys (void) const;

		 // return an AVGroup containing each boid of the flock
        const AVGroup& allVehicles (void) const {return (const AVGroup&)flock;}

		const ObstacleGroup& obstacles(void) const { return m_kObstacles; }
		ObstacleGroup& obstacles(void){ return m_kObstacles; }

		// JF ++ 
		ProximityDatabase* AccessProximityDataBase( void ) const
		{
			return this->pd;
		}

		void AddBoidToFlock( Boid* pkBoid );
		void RemoveBoidFromFlock( const Boid* pkBoid );

		void SetBoidFactory( BoidFactory* pBoidFactory )
		{
			this->m_pBoidFactory = pBoidFactory;
		}

		const EBoidConstraintType GetCurrentBoundaryCondition( void ) const
		{
			return this->constraint;
		}

		const bool WasBoundaryConditionChangedLocally( void ) const
		{
			return this->bWasLocalChange;
		}

		void SetCurrentBoundaryCondition( const EBoidConstraintType  eType,
			bool bLocalChange = true);

		// JF --

	private:
		void addBoidToFlock (void);
		void removeBoidFromFlock (void);
		
		// select next "boundary condition / constraint / obstacle"
		void nextBoundaryCondition (void);

		void initObstacles (void);
		// update Boid::obstacles list when constraint changes
        void updateObstacles (void);
		void drawObstacles (void);

		class SO : public SphereObstacle
        {void draw (const bool filled, const Color& color, const Vec3& vp) const
            {drawSphereObstacle (*this, 10.0f, filled, color, vp);}};

        class RO : public RectangleObstacle
        {void draw (const bool, const Color& color, const Vec3&) const
            {tempDrawRectangle (*this, color);}};

        class BO : public BoxObstacle
        {void draw (const bool, const Color& color, const Vec3&) const
            {tempDrawBox (*this, color);}};


		static void tempDrawRectangle (const RectangleObstacle& rect, const Color& color);
		static void tempDrawBox (const BoxObstacle& box, const Color& color);

		// flock: a group (STL vector) of pointers to all boids
        Boid::groupType flock;
        typedef Boid::groupType::const_iterator iterator;

		// JF ++ 
		Boid::groupType::iterator FindBoid( const Boid* pkBoid );
		// JF --
        // pointer to database used to accelerate proximity queries
        ProximityDatabase* pd;


        // which of the various proximity databases is currently in use
        int cyclePD;

        // --------------------------------------------------------
        // the rest of this plug-in supports the various obstacles:
        // --------------------------------------------------------

        
        EBoidConstraintType constraint;
		bool bWasLocalChange;

		
        RO bigRectangle;
        BO outsideBigBox, insideBigBox;
        SO insideBigSphere, outsideSphere0, outsideSphere1, outsideSphere2,
           outsideSphere3, outsideSphere4, outsideSphere5, outsideSphere6;

		BoidFactory* m_pBoidFactory;
		ObstacleGroup m_kObstacles;
	
	};
}

#endif //BOID_H