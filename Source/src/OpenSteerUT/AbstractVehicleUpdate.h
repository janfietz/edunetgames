#ifndef __ABSTRACTVEHICLEUPDATE_H__
#define __ABSTRACTVEHICLEUPDATE_H__

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

#include "EduNetCommon/EduNetCommon.h"

namespace OpenSteer
{

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

	//-------------------------------------------------------------------------
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

	//-------------------------------------------------------------------------
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

}




#endif  // __ABSTRACTVEHICLEUPDATE_H__
