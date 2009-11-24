//-----------------------------------------------------------------------------
// Copyright (c) 2009, Jan Fietz, Cyrus Preuss
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without modification, 
// are permitted provided that the following conditions are met:
// 
// * Redistributions of source code must retain the above copyright notice, 
//   this list of conditions and the following disclaimer.
// * Redistributions in binary form must reproduce the above copyright notice, 
//   this list of conditions and the following disclaimer in the documentation 
//   and/or other materials provided with the distribution.
// * Neither the name of EduNetGames nor the names of its contributors
//   may be used to endorse or promote products derived from this software
//   without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
// IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES 
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON 
// ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, 
// EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//-----------------------------------------------------------------------------

#include "AbstractVehicleMath.h"


namespace OpenSteer
{

	//-----------------------------------------------------------------------------
	using namespace OpenSteer;

	//-------------------------------------------------------------------------
	void TPhysicsMotionState::updateMotionState( AbstractVehicle* pkVehicle, const osScalar currentTime, 
		const osScalar elapsedTime
		)
	{
		if( NULL == pkVehicle )
		{
			return;
		}
		// store new world transform
		btTransform kWorldTransform1;
		writeToMatrix( *pkVehicle, kWorldTransform1 );
		this->updateMotionState( kWorldTransform1, currentTime, elapsedTime );
	}


	//-----------------------------------------------------------------------------
	void TPhysicsMotionState::updateMotionState( 
		const class btTransform& kWorldTransform1,
		const osScalar currentTime,
		const osScalar elapsedTime
		)
	{
		// do not do this twice
		if( this->m_fLastUpdateTime == currentTime )
		{
			return;
		}
		this->m_fLastUpdateTime = currentTime;

		const class btTransform& kWorldTransform0 = this->m_kWorldTransform;
		// compute motion state data
		// in world space
		PhysicsMotionState& kState = *this;
		bool bDoCalculation = true;
		if( elapsedTime == 0.0f )
		{
			// do nothing in this case
			bDoCalculation = false;
		}
		else if( elapsedTime < 0.001f )
		{
			// clamp very very small timeframes ?
			// elapsedTime = 0.001f;
			bDoCalculation = false;
		}
		if( true == bDoCalculation )
		{
			calculateVelocity( kWorldTransform0, kWorldTransform1, elapsedTime,
				kState.m_kLinearVelocity, kState.m_kAngularVelocity );

			// in local space
			btMatrix3x3 kLocalBasis = kWorldTransform1.getBasis().inverse();
			btVector3 kLocalLinearVelocity;
			getVector3( kState.m_kLinearVelocity, kLocalLinearVelocity );
			kLocalLinearVelocity = kLocalBasis * kLocalLinearVelocity;
			btVector3 kLocalAngularVelocity;
			getVector3( kState.m_kAngularVelocity, kLocalAngularVelocity );
			kLocalAngularVelocity = kLocalBasis * kLocalAngularVelocity;

			getVector3( kLocalLinearVelocity, kState.m_kLocalLinearVelocity );
			getVector3( kLocalAngularVelocity, kState.m_kLocalAngularVelocity );
			kState.m_fLinearVelocity = kState.m_kLinearVelocity.length();
			kState.m_fAngularVelocity = kState.m_kAngularVelocity.length();

		}

		kState.m_kWorldTransform = kWorldTransform1;
	}

	//-----------------------------------------------------------------------------
	osVector3& getVector3( const btVector3& kSource, osVector3& kTarget )
	{
		kTarget.x = kSource.getX();
		kTarget.y = kSource.getY();
		kTarget.z = kSource.getZ();
		return kTarget;
	}

	//-----------------------------------------------------------------------------
	btVector3& getVector3( const osVector3& kSource, btVector3& kTarget )
	{
		kTarget.setX( kSource.x );
		kTarget.setY( kSource.y );
		kTarget.setZ( kSource.z );
		return kTarget;
	}

	// test conversion
	// 	EduNet::EmptyVehicle v1, v2;
	// 	btTransform kWorldTransform;
	// 	btTransform kTargetWorldTransform;
	// 	kWorldTransform.setIdentity();
	// 	readFromMatrix( v2, kWorldTransform );
	// 	writeToMatrix( v2, kTargetWorldTransform );
	void readFromRotationMatrix( osLocalSpaceData& kLocalSpace, const btMatrix3x3& kWorldRotation )
	{
		// calculate local coordinate system
		const btVector3& kRow0 = kWorldRotation.getRow(0);
		const btVector3& kRow1 = kWorldRotation.getRow(1);
		const btVector3& kRow2 = kWorldRotation.getRow(2);
		btVector3 kAxis[3];
		kAxis[0].setX( kRow0.getX() );
		kAxis[0].setY( kRow1.getX() );
		kAxis[0].setZ( kRow2.getX() );

		kAxis[1].setX( kRow0.getY() );
		kAxis[1].setY( kRow1.getY() );
		kAxis[1].setZ( kRow2.getY() );

		kAxis[2].setX( kRow0.getZ() );
		kAxis[2].setY( kRow1.getZ() );
		kAxis[2].setZ( kRow2.getZ() );


#if OS_HAS_BULLET
		kLocalSpace._side = kAxis[0];
		kLocalSpace._forward = kAxis[1];
		kLocalSpace._up = kAxis[2];
#else
		osVector3 kTemp;
#if OPENSTEER_Z_ISUP
		kLocalSpace._side = getVector3( kAxis[0], kTemp );
		kLocalSpace._forward = getVector3( kAxis[1], kTemp );
		kLocalSpace._up = getVector3( kAxis[2], kTemp );
#else
		kLocalSpace._side = -getVector3( kAxis[0], kTemp );
		kLocalSpace._forward = getVector3( kAxis[2], kTemp );
		kLocalSpace._up = getVector3( kAxis[1], kTemp );
#endif
#endif
	}

	//-------------------------------------------------------------------------
	void readFromMatrix( osLocalSpaceData& kLocalSpace, const btTransform& kWorldTransform )
	{
		//	const btTransform& kWorldTransform = pkRigidBody->getWorldTransform();
		const btMatrix3x3& kWorldRotation = kWorldTransform.getBasis();
		OpenSteer::readFromRotationMatrix( kLocalSpace, kWorldRotation );
#if OS_HAS_BULLET
		kLocalSpace._position = kWorldTransform.getOrigin();
#else
		osVector3 kTemp;
		kLocalSpace._position = getVector3( kWorldTransform.getOrigin(), kTemp );
#endif
	}

	//-------------------------------------------------------------------------
	void writeToRotationMatrix( const osLocalSpaceData& kLocalSpace, btMatrix3x3& kWorldRotation )
	{
#if OS_HAS_BULLET
		const btVector3 kTargetRow[3] =
		{
			kVehicle.side(),
			kVehicle.forward(),
			kVehicle.up(),
		};
#else
		btVector3 kTemp;
		const btVector3 kTargetRow[3] =
		{
#if OPENSTEER_Z_ISUP
			getVector3( kLocalSpace._side, kTemp ),
			getVector3( kLocalSpace._forward, kTemp ),
			getVector3( kLocalSpace._up, kTemp ),
#else
			getVector3( -kLocalSpace._side, kTemp ),
			getVector3( kLocalSpace._up, kTemp ),
			getVector3( kLocalSpace._forward, kTemp ),
#endif
		};
#endif
		kWorldRotation.setValue( 
			kTargetRow[0].x(), kTargetRow[1].x(), kTargetRow[2].x(),
			kTargetRow[0].y(), kTargetRow[1].y(), kTargetRow[2].y(),
			kTargetRow[0].z(), kTargetRow[1].z(), kTargetRow[2].z());
	}

	//-------------------------------------------------------------------------
	void writeToMatrix( const osLocalSpaceData& kLocalSpace, btTransform& kWorldTransform )
	{
#if OS_HAS_BULLET
		kWorldTransform.setOrigin( kLocalSpace._position );
#else
		btVector3 kTemp;
		kWorldTransform.setOrigin( getVector3( kLocalSpace._position, kTemp ) );
#endif
		btMatrix3x3& kWorldRotation = kWorldTransform.getBasis();
		OpenSteer::writeToRotationMatrix( kLocalSpace, kWorldRotation );
	}

	//-----------------------------------------------------------------------------
	void readFromMatrix( AbstractVehicle& kVehicle, const btTransform& kWorldTransform )
	{
		OpenSteer::readFromMatrix( kVehicle.accessLocalSpaceData(), kWorldTransform );
	}

	//-----------------------------------------------------------------------------
	void writeToMatrix( const AbstractVehicle& kVehicle, btTransform& kWorldTransform )
	{
		OpenSteer::writeToMatrix( kVehicle.getLocalSpaceData(), kWorldTransform );
	}

	//-----------------------------------------------------------------------------
	void calculateVelocity( const btTransform& kWorldTransform0, const btTransform& kWorldTransform1,
		osScalar fDeltaTime,
		osVector3& kLinearVelocity, osVector3& kAngularVelocity )
	{
		btVector3 _LinearVelocity, _AngularVelocity;
		btTransformUtil::calculateVelocity( 
			kWorldTransform0, kWorldTransform1, fDeltaTime, 
			_LinearVelocity, _AngularVelocity );
		getVector3( _LinearVelocity, kLinearVelocity );
		getVector3( _AngularVelocity, kAngularVelocity );
	}

	//-----------------------------------------------------------------------------
	void localToWorldSpace( osAbstractVehicle& kVehicle, const osVector3& kSource, osVector3& kTarget )
	{
		btTransform kWorldTransform;
		writeToMatrix( kVehicle, kWorldTransform );
		btVector3 _kSource, _kWorld;
		getVector3( kSource, _kSource );
		_kWorld = kWorldTransform.getBasis() * _kSource;
		getVector3( _kWorld, kTarget );
	}


	//-----------------------------------------------------------------------------
	btQuaternion AbstractVehicleMath::readRotation( const OpenSteer::LocalSpaceData& kLocalSpaceData )
	{
		btMatrix3x3 kWorldRotation;
		OpenSteer::writeToRotationMatrix( kLocalSpaceData, kWorldRotation );
		btQuaternion kRotation;
		kWorldRotation.getRotation( kRotation );
		return kRotation;
	}

	//-----------------------------------------------------------------------------
	void AbstractVehicleMath::writeRotation( const btQuaternion& kRotation, OpenSteer::LocalSpaceData& kLocalSpaceData )
	{
		btMatrix3x3 kWorldRotation;
		kWorldRotation.setRotation( kRotation );
		OpenSteer::readFromRotationMatrix( kLocalSpaceData, kWorldRotation );
	}

} // end namespace OpenSteer

