#ifndef BOID_H
#define BOID_H

#include "OpenSteerUT/SimpleNetworkVehicle.h"
#include "NetBoid.h"
#include "NetBoidFactory.h"
namespace OpenSteer{

    // Include names declared in the OpenSteer namespace into the
    // namespaces to search to find names.
    using namespace OpenSteer;
	

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
    // Plugin for OpenSteerDemo
	class BoidsPlugin : public Plugin
    {
		ET_DECLARE_BASE(Plugin);
    public:
		BoidsPlugin (bool bAddToRegistry = true):
		BaseClass(bAddToRegistry)
		{
			this->setVehicleFactory( &this->m_kOfflineBoidFactory );
		};
		virtual ~BoidsPlugin() {} // be more "nice" to avoid a compiler warning
		
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
		virtual ProximityDatabase* accessProximityDataBase( void ) const
		{
			return this->pd;
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

		void initGui( void* pkUserdata );
		void addBoidToFlock (void);
		void removeBoidFromFlock (void);

		virtual osAbstractVehicle* createVehicle( osEntityClassId,
			osProximityDatabase* ) const;
		// JF --

	private:
		
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

		NetBoidFactory m_kOfflineBoidFactory;
		ObstacleGroup m_kObstacles;
	
	};
}

#endif //BOID_H