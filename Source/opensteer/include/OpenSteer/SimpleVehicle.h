//-----------------------------------------------------------------------------
//
//
//! OpenSteer -- Steering Behaviors for Autonomous Characters
//
//! Copyright (c) 2002-2005, Sony Computer Entertainment America
//! Original author: Craig Reynolds <craig_reynolds@playstation.sony.com>
//
//! Permission is hereby granted, free of charge, to any person obtaining a
//! copy of this software and associated documentation files (the "Software"),
//! to deal in the Software without restriction, including without limitation
//! the rights to use, copy, modify, merge, publish, distribute, sublicense,
//! and/or sell copies of the Software, and to permit persons to whom the
//! Software is furnished to do so, subject to the following conditions:
//
//! The above copyright notice and this permission notice shall be included in
//! all copies or substantial portions of the Software.
//
//! THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//! IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//! FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
//! THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//! LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
//! FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
//! DEALINGS IN THE SOFTWARE.
//
//
//-----------------------------------------------------------------------------
//
//
//! SimpleVehicle
//
//! A steerable point mass with a velocity-aligned local coordinate system.
//! SimpleVehicle is useful for developing prototype vehicles in OpenSteerDemo,
//! it is the base class for vehicles in the Plugins supplied with OpenSteer.
//! Note that SimpleVehicle is intended only as sample code.  Your application
//! can use the OpenSteer library without using SimpleVehicle, as long as you
//! implement the AbstractVehicle protocol.
//
//! SimpleVehicle makes use of the "mixin" concept from OOP.  To quote
//! "Mixin-Based Programming in C++" a clear and helpful paper by Yannis
//! Smaragdakis and Don Batory (http://citeseer.nj.nec.com/503808.html):
//
//!     ...The idea is simple: we would like to specify an extension without
//!     predetermining what exactly it can extend. This is equivalent to
//!     specifying a subclass while leaving its superclass as a parameter to be
//!     determined later. The benefit is that a single class can be used to
//!     express an incremental extension, valid for a variety of classes...
//! 
//! In OpenSteer, vehicles are defined by an interface: an abstract base class
//! called AbstractVehicle.  Implementations of that interface, and related
//! functionality (like steering behaviors and vehicle physics) are provided as
//! template-based mixin classes.  The intent of this design is to allow you to
//! reuse OpenSteer code with your application's own base class.
//
//! 10-04-04 bk:  put everything into the OpenSteer namespace
//! 01-29-03 cwr: created
//
//
//-----------------------------------------------------------------------------


#ifndef OPENSTEER_SIMPLEVEHICLE_H
#define OPENSTEER_SIMPLEVEHICLE_H


#include "OpenSteer/AbstractVehicle.h"
#include "OpenSteer/SteerLibrary.h"
#include "OpenSteer/Annotation.h"


namespace OpenSteer {


    //-----------------------------------------------------------------------------


    //! SimpleVehicle_1 adds concrete LocalSpace methods to AbstractVehicle
    typedef EntityLocalSpaceMixin<AbstractVehicle> SimpleVehicle_1;


    //! SimpleVehicle_2 adds concrete annotation methods to SimpleVehicle_1
    typedef AnnotationMixin<SimpleVehicle_1> SimpleVehicle_2;


    //! SimpleVehicle_3 adds concrete steering methods to SimpleVehicle_2
    typedef SteerLibraryMixin<SimpleVehicle_2> SimpleVehicle_3;


    //! SimpleVehicle adds concrete vehicle methods to SimpleVehicle_3
    class SimpleVehicle : public SimpleVehicle_3
    {
    public:

        //! constructor
        SimpleVehicle();

        //! destructor
        ~SimpleVehicle();

		OS_IMPLEMENT_CLASSNAME( SimpleVehicle )

		virtual const char* name (void) const
		{
			//			return this->m_kInstance.name();
			return this->getClassName();
		}


       //! reset vehicle state
        virtual void reset (void)
        {
            //! reset LocalSpace state
            resetLocalSpace ();

            //! reset SteerLibraryMixin state
            //! (XXX this seems really fragile, needs to be redesigned XXX)
            SimpleVehicle_3::reset ();

            setMass( 1 );          //! mass (defaults to 1 so acceleration=force)
            setSpeed( 0 );         //! speed along Forward direction.

            setRadius( 0.5f );     //! size of bounding sphere

            setMaxForce( 0.1f );   //! steering force is clipped to this magnitude
            setMaxSpeed( 1.0f );   //! velocity is clipped to this magnitude

            //! reset bookkeeping to do running averages of these quantities
            resetSmoothedPosition();
            resetSmoothedCurvature();
            resetSmoothedAcceleration();

			clearTrailHistory(); // prevent long streaks due to teleportation

        }

        //! get/set mass
        virtual float mass (void) const {return _mass;}
        virtual float setMass (float m) {return _mass = m;}

        //! get velocity of vehicle
//		virtual const Vec3& velocity (void) const 
        virtual Vec3 velocity (void) const 
		{
			return forward() * _speed;
// TODO: later
//			return this->linearVelocity();
			/*forward() * _speed;*/ 
		}

        //! get/set speed of vehicle  (may be faster than taking mag of velocity)
        virtual float speed (void) const {return _speed;}
        virtual float setSpeed (float s) 
		{
// TODO: later
// 			if( s == 0 )
// 			{
// 				this->setLinearVelocity( Vec3::zero );
// 			}
// 			else
// 			{
// 				// get current velocity
// 				if( this->_linearVelocity.length() == 0 )
// 				{
// 					this->_linearVelocity = this->_forward;
// 				}
// 				else
// 				{
// 					this->_linearVelocity.normalize();
// 				}
// 				this->_linearVelocity *= s;
// 			}
			return _speed = s;
		}

        //! size of bounding sphere, for obstacle avoidance, etc.
        virtual float radius (void) const {return _radius;}
        virtual float setRadius (float m) {return _radius = m;}

        //! get/set maxForce
        virtual float maxForce (void) const {return _maxForce;}
        virtual float setMaxForce (float mf) {return _maxForce = mf;}

        //! get/set maxSpeed
        virtual float maxSpeed (void) const {return _maxSpeed;}
        virtual float setMaxSpeed (float ms) {return _maxSpeed = ms;}

        //! ratio of speed to max possible speed (0 slowest, 1 fastest)
        virtual float relativeSpeed (void) const {return speed () / maxSpeed ();}


        //! apply a given steering force to our momentum,
        //! adjusting our orientation to maintain velocity-alignment.
        virtual void applySteeringForce (const Vec3& force, const float deltaTime);

        virtual Vec3 determineCombinedSteering (const float elapsedTime);
		
        //! the default version: keep FORWARD parallel to velocity, change
        //! UP as little as possible.
        virtual void regenerateLocalSpace( const Vec3& newForward,
                                           const float elapsedTime);

		// local space override
		virtual const Vec3& setLinearVelocity( const Vec3& newVelocity )
		{
			this->_linearVelocity = newVelocity;
			this->_speed = this->_linearVelocity.length();
			return _linearVelocity;
		}

        //! alternate version: keep FORWARD parallel to velocity, adjust UP
        //! according to a no-basis-in-reality "banking" behavior, something
        //! like what birds and airplanes do.  (XXX experimental cwr 6-5-03)
        void regenerateLocalSpaceForBanking (const Vec3& newForward,
                                             const float elapsedTime);

        //! adjust the steering force passed to applySteeringForce.
        //! allows a specific vehicle class to redefine this adjustment.
        //! default is to disallow backward-facing steering at low speed.
        //! xxx experimental 8-20-02
        virtual Vec3 adjustRawSteeringForce (const Vec3& force,
                                             const float deltaTime) const;

        //! apply a given braking force (for a given dt) to our momentum.
        //! xxx experimental 9-6-02
        void applyBrakingForce (const float rate, const float deltaTime);

        //! predict position of this vehicle at some time in the future
        //! (assumes velocity remains constant)
        Vec3 predictFuturePosition (const float predictionTime) const;

        //! get instantaneous curvature (since last update)
        float curvature (void) const {return _curvature;}

        //! get/reset smoothedCurvature, smoothedAcceleration and smoothedPosition
        float smoothedCurvature (void) {return _smoothedCurvature;}
        float resetSmoothedCurvature (float value = 0)
        {
            _lastForward = Vec3::zero;
            _lastPosition = Vec3::zero;
            return _smoothedCurvature = _curvature = value;
        }
        Vec3 smoothedAcceleration (void) {return _smoothedAcceleration;}
        Vec3 resetSmoothedAcceleration (const Vec3& value = Vec3::zero)
        {
            return _smoothedAcceleration = value;
        }
        Vec3 smoothedPosition (void) {return _smoothedPosition;}
        Vec3 resetSmoothedPosition (const Vec3& value = Vec3::zero)
        {
            return _smoothedPosition = value;
        }


        //! draw lines from vehicle's position showing its velocity and acceleration
        void annotationVelocityAcceleration (float maxLengthA, float maxLengthV);
        void annotationVelocityAcceleration (float maxLength)
            {annotationVelocityAcceleration (maxLength, maxLength);}
        void annotationVelocityAcceleration (void)
            {annotationVelocityAcceleration (3, 3);}


		//! CP ++
		virtual void collect3DTextAnnotation( std::ostringstream& kStream );

		virtual void draw( const float /*currentTime*/, const float /*elapsedTime*/ );

		virtual void allocateProximityToken( ProximityDatabase* pkProximityDatabase );
		virtual AbstractVehicle* cloneVehicle( void ) const { return NULL; };

		virtual bool movesPlanar( void ) const { return _movesPlanar; };

		virtual void setCustomUpdated( AbstractUpdated* pkUpdated )
		{
			this->m_pkCustomUpdated = pkUpdated;
		}

		virtual AbstractUpdated* getCustomUpdated( void ) const
		{
			return this->m_pkCustomUpdated;
		}

		//-------------------------------------------------------------------
		// interface AbstractUpdated
		virtual void updateCustom( AbstractUpdated* /*pkParent*/, const osScalar /*currentTime*/, const osScalar /*elapsedTime*/ )
		{
			// nothing to do here
			return;
		}

		virtual bool isEnabled( void ) const { return this->m_bEnabled; }; 
		virtual void setEnabled( bool bEnabled ){ this->m_bEnabled = bEnabled; }; 

		//! currently selected vehicle.  Generally the one the camera follows and
		//! for which additional information may be displayed.  Clicking the mouse
		//! near a vehicle causes it to become the Selected Vehicle.
		static AbstractVehicle* selectedVehicle;
		static AbstractVehicle* nearestMouseVehicle;

		// TODO: make private !
		//! a pointer to this vehicles's interface object for the proximity database
		ProximityToken* m_pkProximityToken;

	protected:
		bool _movesPlanar;
		bool m_bEnabled;

		//! CP --
    private:

        Vec3 _lastForward;
        Vec3 _lastPosition;
        Vec3 _smoothedPosition;
		Vec3 _smoothedAcceleration;
		float _mass;       //! mass (defaults to unity so acceleration=force)

		float _radius;     //! size of bounding sphere, for obstacle avoidance, etc.

		float _maxForce;   //! the maximum steering force this vehicle can apply
		//! (steering force is clipped to this magnitude)

		float _maxSpeed;   //! the maximum speed this vehicle is allowed to move
		//! (velocity is clipped to this magnitude)

		float _curvature;
        float _smoothedCurvature;
		AbstractUpdated* m_pkCustomUpdated;



        //! measure path curvature (1/turning-radius), maintain smoothed version
        void measurePathCurvature (const float elapsedTime);
    };


} //! namespace OpenSteer
    
    
//-----------------------------------------------------------------------------
#endif //! OPENSTEER_SIMPLEVEHICLE_H
