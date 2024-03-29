#ifndef OPENSTEER_LOCALSPACE_H
#define OPENSTEER_LOCALSPACE_H

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
//! LocalSpace: a local coordinate system for 3d space
//
//! Provide functionality such as transforming from local space to global
//! space and vice versa.  Also regenerates a valid space from a perturbed
//! "forward vector" which is the basis of abnstract vehicle turning.
//
//! These are comparable to a 4x4 homogeneous transformation matrix where the
//! 3x3 (R) portion is constrained to be a pure rotation (no shear or scale).
//! The rows of the 3x3 R matrix are the basis vectors of the space.  They are
//! all constrained to be mutually perpendicular and of unit length.  The top
//! ("x") row is called "side", the middle ("y") row is called "up" and the
//! bottom ("z") row is called forward.  The translation vector is called
//! "position".  Finally the "homogeneous column" is always [0 0 0 1].
//
//!     [ R R R  0 ]      [ Sx Sy Sz  0 ]
//!     [ R R R  0 ]      [ Ux Uy Uz  0 ]
//!     [ R R R  0 ]  ->  [ Fx Fy Fz  0 ]
//!     [          ]      [             ]
//!     [ T T T  1 ]      [ Tx Ty Tz  1 ]
//
//! This file defines three classes:
//!   AbstractLocalSpace:  pure virtual interface
//!   LocalSpaceMixin:     mixin to layer LocalSpace functionality on any base
//!   LocalSpace:          a concrete object (can be instantiated)
//
//! 10-04-04 bk:  put everything into the OpenSteer namespace
//! 06-05-02 cwr: created 
//
//
//-----------------------------------------------------------------------------

#include "OpenSteer/AbstractLocalSpace.h"
#include "OpenSteer/AbstractRenderable.h"
#include "OpenSteer/Entity.h"


//-----------------------------------------------------------------------------
namespace OpenSteer {

	
    //-----------------------------------------------------------------------------
    //! LocalSpaceMixin is a mixin layer, a class template with a paramterized base
    //! class.  Allows "LocalSpace-ness" to be layered on any class.
    template <class Super>
    class LocalSpaceMixin : protected LocalSpaceData, public Super
    {
        //! transformation as three orthonormal unit basis vectors and the
        //! origin of the local space.  These correspond to the "rows" of
        //! a 3x4 transformation matrix with [0 0 0 1] as the final column

    private:

    public:
// does not work
//		OS_IMPLEMENT_CLASSNAME( Super )
		OS_IMPLEMENT_CLASSNAME( LocalSpaceMixin )

		const LocalSpaceData& getLocalSpaceData( void ) const
		{
			return *this;
		}

		virtual LocalSpaceData& accessLocalSpaceData( void )
		{
			return *this;
		}

		virtual void setLocalSpaceData( const LocalSpaceData& kLocalSpaceData, bool bPreserveUpdateTicks )
		{
			const size_t currentUpdateTicks = this->_updateTicks;
			LocalSpaceData& _localSpaceData = *this;
			_localSpaceData = kLocalSpaceData;
			if( true == bPreserveUpdateTicks )
			{
				this->_updateTicks = currentUpdateTicks;
			}
		}

        //! accessors (get and set) for side, up, forward and position
        virtual const Vec3& side     (void) const {return _side;};
        virtual const Vec3& up       (void) const {return _up;};
        virtual const Vec3& forward  (void) const {return _forward;};
        virtual const Vec3& position (void) const {return _position;};
        virtual const Vec3& setSide     (const Vec3& s) {_side = s;return _side;};
        virtual const Vec3& setUp       (const Vec3& u) {_up = u;return _up;};
        virtual const Vec3& setForward  (const Vec3& f) {_forward = f;return _forward;};
        virtual const Vec3& setPosition (const Vec3& p) {_position = p;return _position;};
        virtual const Vec3& setSide     (float x, float y, float z){_side.set    (x,y,z);return _side;};
        virtual const Vec3& setUp       (float x, float y, float z){_up.set      (x,y,z);return _up;};
        virtual const Vec3& setForward  (float x, float y, float z){_forward.set (x,y,z);return _forward;};
        virtual const Vec3& setPosition (float x, float y, float z){_position.set(x,y,z);return _position;};
		virtual const Vec3& angularVelocity (void) const {return _angularVelocity;};
		virtual const Vec3& setAngularVelocity (const Vec3& p) {_angularVelocity = p;return _angularVelocity;};
		virtual const Vec3& linearVelocity (void) const {return _linearVelocity;};
		virtual const Vec3& setLinearVelocity (const Vec3& p) {_linearVelocity = p;return _linearVelocity;};

		virtual Vec3 velocity (void) const 
		{
			// note: the simple vehicle returns
			// return forward() * _speed;
			return this->linearVelocity();
			/*forward() * _speed;*/ 
		}
       //-------------------------------------------------------------------------
        //! Global compile-time switch to control handedness/chirality: should
        //! LocalSpace use a left- or right-handed coordinate system?  This can be
        //! overloaded in derived types (e.g. vehicles) to change handedness.

        bool rightHanded (void) const {return true;}

        //-------------------------------------------------------------------------
        //! constructors
        LocalSpaceMixin (void)
        {
            resetLocalSpace ();
        };

		LocalSpaceMixin (
			const Vec3& Position) 
		{
			resetLocalSpace ();
			this->_position = Position;
		}

		LocalSpaceMixin (const Vec3& Side,
                         const Vec3& Up,
                         const Vec3& Forward,
                         const Vec3& Position) 
		{
			this->_side = Side;
			this->_up = Up;
			this->_forward = Forward;
			this->_position = Position;
			this->_angularVelocity =
			this->_linearVelocity = Vec3::zero;
			this->_updateTicks = 0;
		}

        LocalSpaceMixin (const Vec3& Up,
                         const Vec3& Forward,
                         const Vec3& Position)
        {
			this->_up = Up;
			this->_forward = Forward;
			this->_position = Position;
			this->_angularVelocity =
			this->_linearVelocity = Vec3::zero;
			this->_updateTicks = 0;
			setUnitSideFromForwardAndUp ();
        }

        virtual ~LocalSpaceMixin() { /* Nothing to do. */ }
        
        //-------------------------------------------------------------------------
        //! reset transform: set local space to its identity state, equivalent to a
        //! 4x4 homogeneous transform like this:
        //
        //!     [ X 0 0 0 ]
        //!     [ 0 1 0 0 ]
        //!     [ 0 0 1 0 ]
        //!     [ 0 0 0 1 ]
        //
        //! where X is 1 for a left-handed system and -1 for a right-handed system.
        void resetLocalSpace (void)
        {
            _forward.set (0, 0, 1);
            _side = localRotateForwardToSide (_forward);
            _up.set (0, 1, 0);
            _position.set (0, 0, 0);
			this->_angularVelocity =
			this->_linearVelocity = Vec3::zero;
			this->_updateTicks = 0;
        };

        //-------------------------------------------------------------------------
        //! transform a direction in global space to its equivalent in local space
        Vec3 localizeDirection (const Vec3& globalDirection) const
        {
            //! dot offset with local basis vectors to obtain local coordiantes
            return Vec3 (globalDirection.dot (_side),
                         globalDirection.dot (_up),
                         globalDirection.dot (_forward));
        };

        //-------------------------------------------------------------------------
        //! transform a point in global space to its equivalent in local space
        Vec3 localizePosition (const Vec3& globalPosition) const
        {
            //! global offset from local origin
            Vec3 globalOffset = globalPosition - _position;

            //! dot offset with local basis vectors to obtain local coordiantes
            return localizeDirection (globalOffset);
        };

        //-------------------------------------------------------------------------
        //! transform a point in local space to its equivalent in global space
        Vec3 globalizePosition (const Vec3& localPosition) const
        {
            return _position + globalizeDirection (localPosition);
        };

        //-------------------------------------------------------------------------
        //! transform a direction in local space to its equivalent in global space
        Vec3 globalizeDirection (const Vec3& localDirection) const
        {
            return ((_side    * localDirection.x) +
                    (_up      * localDirection.y) +
                    (_forward * localDirection.z));
        };

        //-------------------------------------------------------------------------
        //! set "side" basis vector to normalized cross product of forward and up
		void setUnitSideFromForwardAndUp (void)
        {
            //! derive new unit side basis vector from forward and up
            if (rightHanded())
                _side.cross (_forward, _up);
            else
                _side.cross (_up, _forward);
            _side = _side.normalized ();
        }

        //-------------------------------------------------------------------------
        //! regenerate the orthonormal basis vectors given a new forward
        //! (which is expected to have unit length)
        void regenerateOrthonormalBasisUF (const Vec3& newUnitForward)
        {
            _forward = newUnitForward;

            //! derive new side basis vector from NEW forward and OLD up
            setUnitSideFromForwardAndUp ();

            //! derive new Up basis vector from new Side and new Forward
            //! (should have unit length since Side and Forward are
            //! perpendicular and unit length)
            if (rightHanded())
                _up.cross (_side, _forward);
            else
                _up.cross (_forward, _side);
        }

        //! for when the new forward is NOT know to have unit length
        void regenerateOrthonormalBasis (const Vec3& newForward)
        {
            regenerateOrthonormalBasisUF( newForward.normalized() );
        }


        //! for supplying both a new forward and and new up
        void regenerateOrthonormalBasis (const Vec3& newForward,
                                         const Vec3& newUp)
        {
            _up = newUp;
            regenerateOrthonormalBasis (newForward.normalized());
        }

        //-------------------------------------------------------------------------
        //! rotate, in the canonical direction, a vector pointing in the
        //! "forward" (+Z) direction to the "side" (+/-X) direction
        Vec3 localRotateForwardToSide (const Vec3& v) const
        {
            return Vec3 (rightHanded () ? -v.z : +v.z,
                         v.y,
                         v.x);
        }

        //! not currently used, just added for completeness
        Vec3 globalRotateForwardToSide (const Vec3& globalForward) const
        {
            const Vec3 localForward = localizeDirection (globalForward);
            const Vec3 localSide = localRotateForwardToSide (localForward);
            return globalizeDirection (localSide);
        }

		//! set a random "2D" heading: set local Up to global Y, then effectively
		//! rotate about it by a random angle (pick random forward, derive side).
		virtual void randomizeHeadingOnXZPlane (void)
		{
			setUp (Vec3::up);
			setForward (RandomUnitVectorOnXZPlane ());
			setSide (localRotateForwardToSide (forward()));
		}

		// predict position of this local space at some time in the future
		// extension idea - predictFutureLocalSpace ?
		Vec3 predictFuturePosition (const float predictionTime) const
		{
			return position() + (velocity() * predictionTime);
		}
    };

	//-------------------------------------------------------------------------
	template <class Super>
	class EntityLocalSpaceMixin : public LocalSpaceMixin<Super>
	{
	private:
		Entity m_kEntity;

	public:
		//---------------------------------------------------------------------
		//! constructors
		EntityLocalSpaceMixin (void):LocalSpaceMixin<Super>(),m_pkPossessor(NULL),
			m_pkPossessed(NULL)
		{
		};

		EntityLocalSpaceMixin (
			const Vec3& Position):LocalSpaceMixin<Super>( Position ),m_pkPossessor(NULL),
			m_pkPossessed(NULL)
		{
		};

		EntityLocalSpaceMixin (const Vec3& Side,
			const Vec3& Up,
			const Vec3& Forward,
			const Vec3& Position):LocalSpaceMixin<Super>( Side, Up, Forward, Position ),m_pkPossessor(NULL),
			m_pkPossessed(NULL)
		{
		};

		EntityLocalSpaceMixin (const Vec3& Up,
			const Vec3& Forward,
			const Vec3& Position):LocalSpaceMixin<Super>( Up, Forward, Position ),m_pkPossessor(NULL),
			m_pkPossessed(NULL)
		{
		};

		virtual ~EntityLocalSpaceMixin() 
		{ 
			if( NULL != this->getControlledEntity() )
			{
				this->play( NULL );
			}
			AbstractPlayer* pkPlayer = this->getPlayer();
			if( NULL != pkPlayer )
			{
				pkPlayer->play( NULL );
			}
		}

// does not work
//		OS_IMPLEMENT_CLASSNAME( Super )
		OS_IMPLEMENT_CLASSNAME( EntityLocalSpaceMixin )

		// AbstractEntity interface
		virtual AbstractEntity* cloneEntity( void ) const
		{
			return NULL;
		}

		virtual EntityClassId getClassId( void ) const
		{
			return this->m_kEntity.getClassId( );
		}

		virtual InstanceTracker::Id getEntityId( void ) const
		{
			return this->m_kEntity.getEntityId();
		}

		virtual NetworkId getNetworkId( void ) const
		{
			return this->m_kEntity.getNetworkId();
		}

		virtual void setNetworkId( NetworkId id )
		{
			this->m_kEntity.setNetworkId( id );
		}

		virtual void setIsRemoteObject( bool bIsRemote )
		{
			this->m_kEntity.setIsRemoteObject( bIsRemote );
		}

		virtual bool isRemoteObject( void ) const
		{
			return this->m_kEntity.isRemoteObject();
		}

		virtual const char* name (void) const
		{
			return this->m_kEntity.name();
		}

		//---------------------------------------------------------------------
		// AbstractEntity interface
		virtual void play( AbstractEntity* pkEntity )
		{
			if( false == this->isPlayer() )
			{
				// not a player
				return;
			}
			AbstractEntity* pkThisEntity = dynamic_cast<AbstractEntity*>( this );
			AbstractPlayer* pkThis = CastToAbstractPlayer( pkThisEntity );
			if( NULL != pkEntity )
			{
				pkEntity->possessBy( pkThisEntity );
				AbstractPlayer* pkPlayer = pkEntity->getPlayer();
				if( pkPlayer == pkThis )
				{
					// play succeeded
					this->m_pkPossessed = pkEntity;
				}
			}
			else
			{
				if( NULL != this->m_pkPossessed )
				{
					this->m_pkPossessed->setPossessor( NULL );
				}
				this->m_pkPossessed = NULL;
			}
		};

		virtual void setPossessor( AbstractEntity* pkEntity )
		{
			this->m_pkPossessor = pkEntity;
		}

		virtual void possessBy( AbstractEntity* pkEntity )
		{
			if( NULL != this->m_pkPossessor )
			{
				this->m_pkPossessor->play( NULL );
			}
			this->setPossessor( pkEntity );
		}

		virtual AbstractPlayer* getPlayer( void ) const
		{
			return CastToAbstractPlayer( this->m_pkPossessor );
		}

		virtual AbstractEntity* getControlledEntity( void ) const
		{
			return this->m_pkPossessed;
		}

		virtual bool isPossessed( void ) const 
		{
			return (NULL != this->getPlayer());
		}

		virtual bool isPlayer( void ) const
		{
			EntityLocalSpaceMixin* pkThis = (EntityLocalSpaceMixin*)this;
			AbstractEntity* pkThisEntity = dynamic_cast<AbstractEntity*>( pkThis );
			AbstractPlayer* pkThisPlayer = OpenSteer::CastToAbstractPlayer( pkThisEntity );
			return ( NULL != pkThisPlayer );
		}

		//---------------------------------------------------------------------
		virtual void setParentEntity( AbstractEntity* pkParentEntity )
		{
			this->m_kEntity.setParentEntity( pkParentEntity );
		}

		//! return the parent of this entity
		virtual AbstractEntity* getParentEntity( void ) const
		{
			return this->m_kEntity.getParentEntity();
		}

		virtual AbstractPlugin* getHostPlugin( void ) const
		{
			return OpenSteer::CastToAbstractPlugin( this->getParentEntity() );
		}

		//---------------------------------------------------------------------
		virtual void setVisible( bool bValue )
		{
			this->m_kEntity.setVisible( bValue );
		}

		virtual bool isVisible( void ) const
		{
			return this->m_kEntity.isVisible();
		}

	private:
		AbstractEntity* m_pkPossessor;
		AbstractEntity* m_pkPossessed;
	};

	//-------------------------------------------------------------------------
	//! Concrete LocalSpace class, and a global constant for the identity transform
	typedef LocalSpaceMixin<AbstractLocalSpace> LocalSpace;
	const LocalSpace gGlobalSpace;

	typedef AbstractRenderMixin<LocalSpace> RenderableLocalSpace;

	//-------------------------------------------------------------------------
	//! Concrete EntityLocalSpace class, and a global constant for the identity transform
	typedef AbstractUpdatedMixin<AbstractEntityUpdatedLocalSpace> EntityUpdatedLocalSpace;
	typedef EntityLocalSpaceMixin<EntityUpdatedLocalSpace> EntityLocalSpace;
//	typedef EntityLocalSpaceMixin<AbstractEntityLocalSpace> EntityLocalSpace;
	const EntityLocalSpace gGlobalEntitySpace;

} //! namespace OpenSteer

//-----------------------------------------------------------------------------
#endif //! OPENSTEER_LOCALSPACE_H
