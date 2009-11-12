 //-------------------------------------------------------------------------
// state machine test
#if 0
#include "OpenSteer/AbstractState.h"
#include <string>

std::string	kTest;


void testStateMachine( void );

using namespace OpenSteer;
typedef TState<OpenSteer::LocalSpace> AbstractLocalSpaceState;
typedef TStateMachine<OpenSteer::LocalSpace> LocalSpaceStateMachine;

class LocalSpaceState : public AbstractLocalSpaceState
{
public:	
/*	LocalSpaceState( )
	{
	}
	LocalSpaceState( const LocalSpaceState& kSource )
	{
	}
*/	virtual void enter( OpenSteer::LocalSpace* ){};
	virtual void exit( OpenSteer::LocalSpace* ){};
	
	virtual void execute( OpenSteer::LocalSpace* pkEntity, osScalar fAccumTime, osScalar fDeltaTime )
	{
		bool bTest = true;
		bTest = false;
	}
	
};


typedef TStateVector<OpenSteer::LocalSpace> LocalSpaceStateVector;


void testStateMachine( void )
{
	
	LocalSpace kLocalSpace;
	LocalSpaceStateMachine kStateMachine;
	LocalSpaceState kState;
	kStateMachine.pushState( kState );
	kStateMachine.execute( &kLocalSpace, 1.0f, 2.0f );
	
}
#endif
//-------------------------------------------------------------------------
   class AbstractVehicleUpdate : public AbstractUpdated {
	public:
		AbstractVehicleUpdate( AbstractVehicle& kVehicle ):
		m_kVehicle( kVehicle )
		{
		}
		virtual ~AbstractVehicleUpdate(){}
		
	protected:
		AbstractVehicle& m_kVehicle;
		
	};
	
	class EulerVehicleUpdate : public AbstractVehicleUpdate {
		OS_DECLARE_BASE(AbstractVehicleUpdate)
	public:
		EulerVehicleUpdate( AbstractVehicle& kVehicle ):
		BaseClass( kVehicle ),
		m_kForce(Vec3::zero),
		_smoothedAcceleration(Vec3::zero)
		{
		}
		virtual ~EulerVehicleUpdate(){}
		
		void setForce( const Vec3& kForce ){ m_kForce = kForce; }
		
		//---------------------------------------------------------------------
		virtual void update( const osScalar /*currentTime*/, const osScalar elapsedTime )
		{
			// compute acceleration and velocity
			Vec3 newAcceleration = (this->m_kForce / this->m_kVehicle.mass());
			Vec3 newVelocity = this->m_kVehicle.velocity();
			
			// damp out abrupt changes and oscillations in steering acceleration
			// (rate is proportional to time step, then clipped into useful range)
			if (elapsedTime > 0)
			{
				const float smoothRate = clip (9 * elapsedTime, 0.15f, 0.4f);
				blendIntoAccumulator (smoothRate,
									  newAcceleration,
									  _smoothedAcceleration);
			}
			
			// Euler integrate (per frame) acceleration into velocity
			newVelocity += _smoothedAcceleration * elapsedTime;
			
			// enforce speed limit
			newVelocity = newVelocity.truncateLength( this->m_kVehicle.maxSpeed () );
			
			// update Speed
			this->m_kVehicle.setSpeed (newVelocity.length());
			
			// Euler integrate (per frame) velocity into position
			this->m_kVehicle.setPosition (m_kVehicle.position() + (newVelocity * elapsedTime));
			
			bool bInfiniteRotationSpeed = false;
			if( true == bInfiniteRotationSpeed )
			{
				// regenerate local space (by default: align vehicle's forward axis with
				// new velocity, but this behavior may be overridden by derived classes.)
				this->m_kVehicle.regenerateLocalSpace (newVelocity, elapsedTime);			
			}
			else 
			{
				Vec3 newForward = this->m_kVehicle.forward();
				if( this->m_kVehicle.speed() > 0 )
				{
					newForward += newVelocity.normalize();
					newForward = newForward.normalize();
				}
				this->m_kVehicle.regenerateOrthonormalBasisUF( newForward );			
			}

		}
		
		
	private:
		Vec3 m_kForce;
		Vec3 _smoothedAcceleration;
	};

	class SteeringForceVehicleUpdate : public AbstractVehicleUpdate {
		OS_DECLARE_BASE(AbstractVehicleUpdate)
	public:
		SteeringForceVehicleUpdate( AbstractVehicle& kVehicle ):
		BaseClass( kVehicle ),
		m_kForce(Vec3::zero)
		{
		}
		virtual ~SteeringForceVehicleUpdate(){}
		
		const Vec3& getForce( void ){ return m_kForce; }
		
		//---------------------------------------------------------------------
		virtual void update( const osScalar /*currentTime*/, const osScalar elapsedTime )
		{
			const Vec3 force = this->m_kVehicle.determineCombinedSteering (elapsedTime);
			const Vec3 adjustedForce = this->m_kVehicle.adjustRawSteeringForce( force, elapsedTime );
			
			// enforce limit on magnitude of steering force
			this->m_kForce = adjustedForce.truncateLength( this->m_kVehicle.maxForce () );
		}
				
	private:
		Vec3 m_kForce;
	};
	
	
    // ----------------------------------------------------------------------------
    class Pedestrian : public SimpleVehicle
    {
		public:
        
        // type for a group of Pedestrians
        typedef std::vector<Pedestrian*> groupType;
		
		SteeringForceVehicleUpdate m_kSteeringForceUpdate;
		EulerVehicleUpdate m_kEulerUpdate;
        
        // constructor
        Pedestrian (ProximityDatabase& pd):
		m_kSteeringForceUpdate(*this),
		m_kEulerUpdate(*this)
        {
            // allocate a token for this boid in the proximity database
            proximityToken = NULL;
            newPD (pd);
            
            // reset Pedestrian state
            reset ();
        }
        
        // destructor
        virtual ~Pedestrian ()
        {
            // delete this boid's token in the proximity database
            delete proximityToken;
        }
        
        // reset all instance state
        void reset (void)
        {
            // reset the vehicle 
            SimpleVehicle::reset ();
            
            // max speed and max steering force (maneuverability)
			float fAgility = 1.0 + frandom01();
            setMaxSpeed ( 2.0 * fAgility );
            setMaxForce ( 8.0 * fAgility );
            
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
            const Vec3 randomOffset = randomVectorOnUnitRadiusXZDisk () * r;
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
		
        // per frame simulation update
        void update (const float currentTime, const float elapsedTime)
        {
            // apply steering force to our momentum
//            applySteeringForce (determineCombinedSteering (elapsedTime),
//                                elapsedTime);
			// alternative way
			this->m_kSteeringForceUpdate.update( osScalar(0), elapsedTime );
			const Vec3& kSteeringForce = this->m_kSteeringForceUpdate.getForce();
			this->m_kEulerUpdate.setForce( kSteeringForce );
			this->m_kEulerUpdate.update( osScalar(0), elapsedTime );
			
            
            // reverse direction when we reach an endpoint
            if (gUseDirectedPathFollowing)
            {
                const Color darkRed (0.7f, 0, 0);
                float const pathRadius = path->radius();
                
                if (Vec3::distance (position(), gEndpoint0) < pathRadius )
                {
                    pathDirection = +1;
                    annotationXZCircle (pathRadius, gEndpoint0, darkRed, 20);
                }
                if (Vec3::distance (position(), gEndpoint1) < pathRadius )
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
