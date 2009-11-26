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

#include "AbstractVehicleUpdate.h"
#include "EduNetCommon/TCompressed.h"
#include "EduNetCommon/EduNetMath.h"

//-----------------------------------------------------------------------------
using namespace OpenSteer;

//-----------------------------------------------------------------------------
void EulerVehicleUpdate::setVehicle( AbstractVehicle* pkVehicle )
{
	this->m_pkVehicle = pkVehicle;
	if( NULL != this->m_pkVehicle )
	{
		// store current world transform
		writeToMatrix( this->vehicle(), this->m_kMotionState.m_kWorldTransform );
	}
}

//-----------------------------------------------------------------------------
void EulerVehicleUpdate::integrateMotionState( 
	const osScalar currentTime, const osScalar elapsedTime )
{
	m_kSmoothedAcceleration = Vec3::zero;
	PhysicsMotionState kMotionState;
	PhysicsMotionState kExtrapolatedMotionState;
	kMotionState.readLocalSpaceData( this->vehicle().getLocalSpaceData() );
	kMotionState.integrateMotionState( 
		kExtrapolatedMotionState, elapsedTime );
	kExtrapolatedMotionState.writeLocalSpaceData( this->vehicle() );
}

//-----------------------------------------------------------------------------
void EulerVehicleUpdate::update( const osScalar currentTime, const osScalar elapsedTime )
{
	// only in case a custom has been set ?
	BaseClass::update( currentTime, elapsedTime );

	{
		ET_PROFILE( EulerVehicleUpdate );

		switch( this->getUpdateMode() )
		{
		case( EEulerUpdateMode_Accelerate ):
			{
				// compute acceleration and velocity
				const Vec3 newVelocity = this->updateLinearVelocity( currentTime, elapsedTime );

				// update Speed
				this->vehicle().setSpeed( newVelocity.length() );

				if( this->vehicle().speed() > 0 )
				{
					// Euler integrate (per frame) velocity into position
					const osVector3 kNewPosition = this->vehicle().position() + ( newVelocity * elapsedTime );
					this->vehicle().setPosition( kNewPosition );

					// regenerate local space (by default: align vehicle's forward axis with
					// new velocity, but this behavior may be overridden by derived classes.)
					Vec3 newForward = newVelocity.normalized();
					this->vehicle().regenerateLocalSpace( newForward, elapsedTime );
				}
				else
				{
					// maybe smth to turn at zero speed ?
				}
			}
			break;
		case( EEulerUpdateMode_IntegrateMotionState ):
			{
				this->integrateMotionState( currentTime, elapsedTime );
			}
			break;
		}
	}
}

//-----------------------------------------------------------------------------
Vec3 EulerVehicleUpdate::updateLinearVelocity( const osScalar currentTime, const osScalar elapsedTime )
{
	Vec3 newVelocity = this->vehicle().velocity();

	// damp out abrupt changes and oscillations in steering acceleration
	// (rate is proportional to time step, then clipped into useful range)
	if( elapsedTime > 0 )
	{
		// compute acceleration and velocity
		Vec3 newAcceleration = ( this->getForce() / this->vehicle().mass() );

		const float smoothRate = clip (9 * elapsedTime, 0.15f, 0.4f);
		blendIntoAccumulator( smoothRate,
			newAcceleration,
			m_kSmoothedAcceleration );
		// Euler integrate (per frame) acceleration into velocity
		newVelocity += m_kSmoothedAcceleration * elapsedTime;

	}
	// enforce speed limit anyways
	// as the velocity might have been set from an external call
	newVelocity = newVelocity.truncateLength( this->vehicle().maxSpeed () );

	return newVelocity;
}

//-----------------------------------------------------------------------------
void EulerVehicleUpdate::updateMotionState( const osScalar currentTime, 
					   const osScalar elapsedTime
					   )
{
	bool bSuccess = 
		this->m_kMotionState.updateMotionState( &this->vehicle(), currentTime, elapsedTime );
	if( false == bSuccess )
	{
		// this is a serious error what to do ?
	}
	else
	{
		if( this->getUpdateMode() == EEulerUpdateMode_IntegrateMotionState )
		{
			// do not do anything in this case;
//			return;
		}
		this->vehicle().setAngularVelocity( this->m_kMotionState.m_kAngularVelocity );
		this->vehicle().setLinearVelocity( this->m_kMotionState.m_kLinearVelocity );
	}
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

osScalar SteeringForceVehicleUpdate::ms_SteeringForceFPS = 30.0f;

//-----------------------------------------------------------------------------
SteeringForceVehicleUpdate::SteeringForceVehicleUpdate( AbstractVehicle* pkVehicle ):
BaseClass( pkVehicle ),
m_kForce(Vec3::zero)
{
	AbstractVehicleMath::compressFixedLengthVector( 
		m_kForce, this->vehicle().maxForce (), this->m_cForce );
}

//-----------------------------------------------------------------------------
Vec3 SteeringForceVehicleUpdate::determineCombinedSteering( const osScalar elapsedTime ) const
{
	ET_PROFILE( determineCombinedSteeringForce );

	const Vec3 force = this->vehicle().determineCombinedSteering( elapsedTime );
	Vec3 adjustedForce = this->vehicle().adjustRawSteeringForce( force, elapsedTime );

	// enforce limit on magnitude of steering force
	// for a 2d vehicle set y to zero
	if( this->vehicle().movesPlanar() )
	{
#if OPENSTEER_Z_ISUP
		float& fForceCorrection = adjustedForce.z;
#else
		float& fForceCorrection = adjustedForce.y;
#endif
		if( fForceCorrection != 0 )
		{
			const float adjust = ::fabs( fForceCorrection );
			fForceCorrection = 0;
			float adjustedForceLength = adjustedForce.length();
			if( adjustedForceLength > 0 )
			{
				adjustedForce = adjustedForce / adjustedForceLength; // normalize
				adjustedForceLength += adjust; // adjust
				adjustedForce = adjustedForce * adjustedForceLength;
			}
		}
	}
	return adjustedForce.truncateLength( this->vehicle().maxForce () );
}

//-----------------------------------------------------------------------------
void SteeringForceVehicleUpdate::setForce( const Vec3& kForce, bool bCompress )
{ 
	if( true == bCompress )
	{
		AbstractVehicleMath::compressFixedLengthVector( 
			kForce, this->vehicle().maxForce (), this->m_cForce );

		// expand again to avoid inconsistencies
		AbstractVehicleMath::expandFixedLengthVector( 
			this->m_cForce, this->vehicle().maxForce (), this->m_kForce );
		if( kForce.length() > 0.0f )
		{
			float fCompressionError = kForce.length() - this->m_kForce.length();
			if( fCompressionError > 1.0f )
			{
				bool bTest = true;
				bTest = false;
			}
			this->m_kForce = kForce;
		}
	}
	else
	{
		this->m_kForce = kForce; 
	}
}

//-----------------------------------------------------------------------------
void SteeringForceVehicleUpdate::update( const osScalar currentTime, const osScalar elapsedTime )
{
	// only in case a custom has been set ?
	BaseClass::update( currentTime, elapsedTime );
	this->m_kUpdatePeriod.SetPeriodFrequency( SteeringForceVehicleUpdate::ms_SteeringForceFPS, true );
	size_t uiTicks = this->m_kUpdatePeriod.UpdateDeltaTime( elapsedTime );
	if( uiTicks == 0 )
	{
		// just do nothing
		// meaning keeping the computed steering force for the next tick
		return;
	}

	{
		ET_PROFILE( SteeringForceVehicleUpdate );

		// this is not right as this is part of the physics engine
		// ...
		// 	if( this->vehicle().isRemoteObject() )
		// 	{
		// 		return;
		// 	}

		// in case this update is enabled
		if( this->isEnabled() )
		{
			const Vec3 force = this->determineCombinedSteering( elapsedTime );
			const bool bCompress = ( false == this->vehicle().isRemoteObject() );
			this->setForce( force, bCompress );
		}
	}
}

