#ifndef OPENSTEER_ABSTRACTLOCALSPACE_H
#define OPENSTEER_ABSTRACTLOCALSPACE_H

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

#include "OpenSteer/Vec3.h"
#include "OpenSteer/AbstractUpdated.h"
#include "OpenSteer/AbstractEntity.h"

//-----------------------------------------------------------------------------
namespace OpenSteer {

	typedef struct TLocalSpaceData
	{
		Vec3 _side;     //!    side-pointing unit basis vector
		Vec3 _up;       //!  upward-pointing unit basis vector
		Vec3 _forward;  //! forward-pointing unit basis vector
		Vec3 _position; //! origin of local space
// additions for demo purpose
		Vec3 _angularVelocity; //! angular velocity of this object
		Vec3 _linearVelocity;  //! linear velocity of this object
		Vec3 _steeringForce;  //! steering force applied to this object
		float _speed;//! speed along Forward direction.  Because local space
		//! is velocity-aligned, velocity = Forward * Speed
		size_t _updateTicks; //! used for network examples
		size_t _userType; //! used to flag the record

		void resetLocalSpaceData(void)
		{
			this->_forward = Vec3::forward;
			this->_side = Vec3::side;
			this->_up = Vec3::up;
			this->_position.set (0, 0, 0);
			this->_angularVelocity =
				this->_linearVelocity = 
				this->_steeringForce = 
				Vec3::zero;
			this->_speed = 0;
			this->_updateTicks = 0;
			this->_userType = 0;
		};

	} LocalSpaceData;

	typedef std::vector<LocalSpaceData> TLocalSpaceDataArray;
	typedef TLocalSpaceDataArray::const_iterator LocalSpaceDataCIterator;
	typedef TLocalSpaceDataArray::iterator LocalSpaceDataIterator;


	//-------------------------------------------------------------------------
	class LocalSpaceDataArray : public /* TODO protected*/ TLocalSpaceDataArray
	{
	public:
		LocalSpaceDataArray( size_t uiMaxRecords = 10 ):
		  m_uiMaxRecords( uiMaxRecords )
		{
		  this->reserve( uiMaxRecords + 1 );
		  this->reset();
		}

		virtual ~LocalSpaceDataArray()
		{
		}

		void reset()
		{
			this->clear();
			this->setMaxRecords( m_uiMaxRecords );
		}
#if 0
		TLocalSpaceDataArray::const_iterator begin( void ) const
		{
			return TLocalSpaceDataArray::begin();
		}

		TLocalSpaceDataArray::const_iterator end( void ) const
		{
			return TLocalSpaceDataArray::end();
		}

		TLocalSpaceDataArray::iterator begin( void )
		{
			return TLocalSpaceDataArray::begin();
		}

		TLocalSpaceDataArray::iterator end( void )
		{
			return TLocalSpaceDataArray::end();
		}
#endif
		LocalSpaceData& addValue( const LocalSpaceData& kValue )
		{
			this->push_back( kValue );
			if( this->size() > this->m_uiMaxRecords )
			{
				this->erase( this->begin() );
			}
			return (*this)[ this->size() - 1 ];;
		}


		LocalSpaceDataArray& setMaxRecords( size_t uiValue ) 
		{ 
		  m_uiMaxRecords = uiValue; 
		  if( this->capacity() < m_uiMaxRecords )
		  {
			  this->reserve( m_uiMaxRecords );
		  }
		  return (*this);
		}

		size_t getMaxRecords( void ) const { return m_uiMaxRecords; }

	private:
		size_t m_uiMaxRecords;
	};

	//-------------------------------------------------------------------------
	class AbstractLocalSpace
    {
    public:
        virtual ~AbstractLocalSpace() { /* Nothing to do. */ }
        
		virtual const LocalSpaceData& getLocalSpaceData( void ) const OS_ABSTRACT;
		virtual LocalSpaceData& accessLocalSpaceData( void ) OS_ABSTRACT;
		virtual void setLocalSpaceData( const LocalSpaceData& kLocalSpaceData, bool bPreserveUpdateTicks ) OS_ABSTRACT;

        //! accessors (get and set) for side, up, forward and position
        virtual const Vec3& side (void) const OS_ABSTRACT;
        virtual const Vec3& setSide (const Vec3& s) OS_ABSTRACT;
        virtual const Vec3& up (void) const OS_ABSTRACT;
        virtual const Vec3& setUp (const Vec3& u) OS_ABSTRACT;
        virtual const Vec3& forward (void) const OS_ABSTRACT;
        virtual const Vec3& setForward (const Vec3& f) OS_ABSTRACT;
        virtual const Vec3& position (void) const OS_ABSTRACT;
        virtual const Vec3& setPosition (const Vec3& p) OS_ABSTRACT;
		virtual const Vec3& angularVelocity (void) const OS_ABSTRACT;
		virtual const Vec3& setAngularVelocity (const Vec3& p) OS_ABSTRACT;
		virtual const Vec3& linearVelocity (void) const OS_ABSTRACT;
		virtual const Vec3& setLinearVelocity (const Vec3& p) OS_ABSTRACT;

        //! use right-(or left-)handed coordinate space
        virtual bool rightHanded (void) const OS_ABSTRACT;

        //! reset transform to identity
        virtual void resetLocalSpace (void) OS_ABSTRACT;

        //! transform a direction in global space to its equivalent in local space
        virtual Vec3 localizeDirection (const Vec3& globalDirection) const OS_ABSTRACT;

        //! transform a point in global space to its equivalent in local space
        virtual Vec3 localizePosition (const Vec3& globalPosition) const OS_ABSTRACT;

        //! transform a point in local space to its equivalent in global space
        virtual Vec3 globalizePosition (const Vec3& localPosition) const OS_ABSTRACT;

        //! transform a direction in local space to its equivalent in global space
        virtual Vec3 globalizeDirection (const Vec3& localDirection) const OS_ABSTRACT;

        //! set "side" basis vector to normalized cross product of forward and up
        virtual void setUnitSideFromForwardAndUp (void) OS_ABSTRACT;

        //! regenerate the orthonormal basis vectors given a new forward
        //! (which is expected to have unit length)
        virtual void regenerateOrthonormalBasisUF (const Vec3& newUnitForward) OS_ABSTRACT;

        //! for when the new forward is NOT of unit length
        virtual void regenerateOrthonormalBasis (const Vec3& newForward) OS_ABSTRACT;

        //! for supplying both a new forward and and new up
        virtual void regenerateOrthonormalBasis (const Vec3& newForward,
                                                 const Vec3& newUp) OS_ABSTRACT;

        //! rotate 90 degrees in the direction implied by rightHanded()
        virtual Vec3 localRotateForwardToSide (const Vec3& v) const OS_ABSTRACT;
        virtual Vec3 globalRotateForwardToSide (const Vec3& globalForward) const=0;

		virtual void randomizeHeadingOnXZPlane (void) OS_ABSTRACT;

    };

    //-----------------------------------------------------------------------------
	class AbstractUpdatedLocalSpace : public AbstractLocalSpace, public AbstractUpdated  {
    public:
        virtual ~AbstractUpdatedLocalSpace() { /* Nothing to do. */ }

		virtual void updateCustom( AbstractUpdated* /*pkParent*/, const osScalar /*currentTime*/, const osScalar /*elapsedTime*/ )
		{
			// nothing to do here
			return;
		}

	};
	
	//-------------------------------------------------------------------------
	class AbstractEntityLocalSpace : public AbstractEntity, public AbstractLocalSpace
	{
	public:
		virtual ~AbstractEntityLocalSpace() { /* Nothing to do. */ }
	};

	//-------------------------------------------------------------------------
	class AbstractEntityUpdatedLocalSpace : public AbstractEntity, public AbstractUpdatedLocalSpace
	{
	public:
		virtual ~AbstractEntityUpdatedLocalSpace() { /* Nothing to do. */ }
	};


} //! namespace OpenSteer

//-----------------------------------------------------------------------------
#endif //! OPENSTEER_ABSTRACTLOCALSPACE_H
