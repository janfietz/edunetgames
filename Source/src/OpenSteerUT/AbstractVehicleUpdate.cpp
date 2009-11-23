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
void EulerVehicleUpdate::update( const osScalar currentTime, const osScalar elapsedTime )
{
	// only in case a custom has been set ?
	BaseClass::update( currentTime, elapsedTime );
	// store current world transform
//	writeToMatrix( this->vehicle(), this->m_kMotionState.m_kWorldTransform );

	// compute acceleration and velocity
	Vec3 newAcceleration = ( this->getForce() / this->vehicle().mass() );
	Vec3 newVelocity = this->vehicle().velocity();

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
	newVelocity = newVelocity.truncateLength( this->vehicle().maxSpeed () );

	// update Speed
	this->vehicle().setSpeed( newVelocity.length() );

	// Euler integrate (per frame) velocity into position
	osVector3 kNewPosition = vehicle().position() + (newVelocity * elapsedTime);
	this->vehicle().setPosition( kNewPosition );

	// regenerate local space (by default: align vehicle's forward axis with
	// new velocity, but this behavior may be overridden by derived classes.)
	float newSpeed = newVelocity.length();
	if( newSpeed > 0 )
	{
		//Vec3 newForward = newVelocity / newSpeed;
		Vec3 newForward = this->vehicle().forward();
		if( this->vehicle().speed() > 0 )
		{
			newForward += newVelocity.normalized();
			float fLength = newForward.length();
			if( fLength > 0 )
			{
				newForward /= fLength;
			}
			else
			{
				newForward = -this->vehicle().forward();
			}
		}
		this->vehicle().regenerateLocalSpace( newForward, elapsedTime );
	}
	else
	{
		// maybe smth to turn at zero speed ?
	}

// 	bool bInfiniteRotationSpeed = false;
// 	if( true == bInfiniteRotationSpeed )
// 	{
// 		// regenerate local space (by default: align vehicle's forward axis with
// 		// new velocity, but this behavior may be overridden by derived classes.)
// 		this->vehicle().regenerateLocalSpace( newVelocity, elapsedTime );			
// 	}
// 	else 
// 	{
// 		Vec3 newForward = this->vehicle().forward();
// 		if( this->vehicle().speed() > 0 )
// 		{
// 			newForward += newVelocity.normalized();
// 			newForward = newForward.normalized();
// 		}
// 		this->vehicle().regenerateOrthonormalBasisUF( newForward );			
// 	}
// 
	this->updateMotionState( currentTime, elapsedTime );
}

//-------------------------------------------------------------------------
void EulerVehicleUpdate::updateMotionState( const osScalar currentTime, 
					   const osScalar elapsedTime
					   )
{
	// store new world transform
	btTransform kWorldTransform1;
	writeToMatrix( this->vehicle(), kWorldTransform1 );
	this->m_kMotionState.updateMotionState( kWorldTransform1, currentTime, elapsedTime );
}

//-------------------------------------------------------------------------
void SteeringForceVehicleUpdate::update( const osScalar currentTime, const osScalar elapsedTime )
{
	// only in case a custom has been set ?
	BaseClass::update( currentTime, elapsedTime );
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
	this->m_kForce = adjustedForce.truncateLength( this->vehicle().maxForce () );

	const Vec3 c1 = this->vehicle().position();
	const Vec3 c2 = this->vehicle().position() + this->m_kForce;
	const Color color = gBlue;
	this->annotationLine (c1, c2, color);
}

