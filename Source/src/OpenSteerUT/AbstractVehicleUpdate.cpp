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
void EulerVehicleUpdate::update( const osScalar currentTime, const osScalar elapsedTime )
{
	// only in case a custom has been set ?
	BaseClass::update( currentTime, elapsedTime );

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
	if( this->vehicle().speed() > 0 )
	{
		Vec3 newForward = newVelocity.normalized();
		this->vehicle().regenerateLocalSpace( newForward, elapsedTime );
	}
	else
	{
		// maybe smth to turn at zero speed ?
	}

	this->updateMotionState( currentTime, elapsedTime );
}

//-------------------------------------------------------------------------
void EulerVehicleUpdate::updateMotionState( const osScalar currentTime, 
					   const osScalar elapsedTime
					   )
{
	// store new world transform
	btTransform kWorldTransform1;
	if( true == writeToMatrix( this->vehicle(), kWorldTransform1 ) )
	{
		this->m_kMotionState.updateMotionState( kWorldTransform1, currentTime, elapsedTime );
	}
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

	float fForce = this->m_kForce.length();
	if( fForce > 0 )
	{
		// compression prototype code
		osVector3 kUnitForce = this->m_kForce / fForce;
		kUnitForce.x = etClamp( kUnitForce.x, -1.0f, 1.0f );
		kUnitForce.y = etClamp( kUnitForce.y, -1.0f, 1.0f );
		kUnitForce.z = etClamp( kUnitForce.z, -1.0f, 1.0f );
		float fForceFactor = fForce / this->vehicle().maxForce ();
		fForceFactor = etClamp( fForceFactor, -1.0f, 1.0f );

		this->m_cForce[0] = TCompressedFixpoint<float,char,8>::writeCompress( kUnitForce.x , -1.0f, 1.0f );
		this->m_cForce[1] = TCompressedFixpoint<float,char,8>::writeCompress( kUnitForce.y , -1.0f, 1.0f );
		this->m_cForce[2] = TCompressedFixpoint<float,char,8>::writeCompress( kUnitForce.z , -1.0f, 1.0f );
		this->m_cForce[3] = TCompressedFixpoint<float,char,8>::writeCompress( fForceFactor, -1.0f, 1.0f );

		// inflate and apply to avoid inconsistencies
		osVector3 kInflatedForce;
		kInflatedForce.x = TCompressedFixpoint<float,char,8>::readInflate( this->m_cForce[0] , -1.0f, 1.0f );
		kInflatedForce.y = TCompressedFixpoint<float,char,8>::readInflate( this->m_cForce[1] , -1.0f, 1.0f );
		kInflatedForce.z = TCompressedFixpoint<float,char,8>::readInflate( this->m_cForce[2] , -1.0f, 1.0f );
		kInflatedForce *= TCompressedFixpoint<float,char,8>::readInflate( this->m_cForce[3] , -1.0f, 1.0f );
		kInflatedForce *= this->vehicle().maxForce();

		osVector3 kCompressionError = kInflatedForce - this->m_kForce;
		float fCompressionError = kCompressionError.length();
		const size_t uiVe3Size = sizeof(osVector3);
		const size_t uiCompressedSize = sizeof(this->m_cForce);

		this->m_kForce = kInflatedForce;
	}
	else
	{
		this->m_cForce[0] = this->m_cForce[1] = this->m_cForce[2] = this->m_cForce[3];

	}

}

