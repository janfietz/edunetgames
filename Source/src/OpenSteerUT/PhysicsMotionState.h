#ifndef __PHYSICSMOTIONSTATE_H__
#define __PHYSICSMOTIONSTATE_H__

//-----------------------------------------------------------------------------
// Copyright (c) 2011, Jan Fietz, Cyrus Preuss
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

#include "OpenSteerUT/AbstractVehicleMath.h"


namespace OpenSteer
{
	typedef struct PhysicsMotionState_t PhysicsMotionState;

	typedef struct PhysicsMotionState_t
	{
		PhysicsMotionState_t():m_kForce(Vec3::zero),
			m_kLinearVelocity(Vec3::zero),
			m_kLocalLinearVelocity(Vec3::zero),
			m_kAngularVelocity(Vec3::zero),
			m_kLocalAngularVelocity(Vec3::zero),
			m_fLinearVelocity(0),
			m_fAngularVelocity(0),
			m_fLastUpdateTime(0)
		{
			m_kWorldTransform.setIdentity();
		}
		btTransform m_kWorldTransform;
		Vec3 m_kForce;
		Vec3 m_kLinearVelocity;
		Vec3 m_kLocalLinearVelocity;
		Vec3 m_kAngularVelocity;
		Vec3 m_kLocalAngularVelocity;
		float m_fLinearVelocity;
		float m_fAngularVelocity;
		osScalar m_fLastUpdateTime;

		void reset( void )
		{
			m_kWorldTransform.setIdentity();
			m_kForce =
			m_kLinearVelocity =
			m_kLocalLinearVelocity = 
			m_kAngularVelocity =
			m_kLocalAngularVelocity = Vec3::zero;
			m_fLinearVelocity =
			m_fAngularVelocity =
			m_fLastUpdateTime = 0;
		}

		bool readLocalSpaceData( const LocalSpaceData& kLocalSpaceData );
		void writeLocalSpaceData( AbstractLocalSpace& kLocalSpace );

		bool updateMotionState( AbstractVehicle* pkVehicle, const osScalar currentTime, 
			const osScalar elapsedTime
			);

		bool updateMotionState( const LocalSpaceData& kLocalSpaceData, const osScalar currentTime, 
			const osScalar elapsedTime
			);

		void updateMotionState( 
			const btTransform& kWorldTransform1,
			const osScalar currentTime,
			const osScalar elapsedTime
			);

		void integrateMotionState( PhysicsMotionState& kTarget, const osScalar elapsedTime ) const;
		void integrateMotionState( PhysicsMotionState& kTarget, const osScalar elapsedTime, size_t uiSteps ) const;

	} PhysicsMotionState;




}




#endif  // __PHYSICSMOTIONSTATE_H__
