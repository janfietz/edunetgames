#ifndef __ABSTRACTVEHICLEMATH_H__
#define __ABSTRACTVEHICLEMATH_H__

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

#include "bullet/src/LinearMath/btVector3.h"
#include "bullet/src/LinearMath/btTransform.h"
#include "bullet/src/LinearMath/btTransformUtil.h"

namespace OpenSteer
{

	void localToWorldSpace( osAbstractVehicle& kVehicle, const osVector3& kSource, osVector3& kTarget );


	osVector3& getVector3( const btVector3& kSource, osVector3& kTarget );
	btVector3& getVector3( const osVector3& kSource, btVector3& kTarget );

	void readFromRotationMatrix( osLocalSpaceData& kLocalSpace, const btMatrix3x3& kWorldRotation );
	void readFromMatrix( osLocalSpaceData& kLocalSpace, const btTransform& kWorldTransform );
	void writeToRotationMatrix( const osLocalSpaceData& kLocalSpace, btMatrix3x3& kWorldRotation );
	void writeToMatrix( const osLocalSpaceData& kLocalSpace, btTransform& kWorldTransform );

	void readFromMatrix( osAbstractVehicle& kVehicle, const btTransform& kWorldTransform );
	void writeToMatrix( const AbstractVehicle& kVehicle, btTransform& kWorldTransform );

	void calculateVelocity( const btTransform& kWorldTransform0, const btTransform& kWorldTransform1,
		osScalar fDeltaTime,
		osVector3& kLinearVelocity, osVector3& kAngularVelocity );

	typedef struct TPhysicsMotionState
	{
		TPhysicsMotionState():m_kForce(Vec3::zero),
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

		void updateMotionState( AbstractVehicle* pkVehicle, const osScalar currentTime, 
			const osScalar elapsedTime
			);

		void updateMotionState( 
			const btTransform& kWorldTransform1,
			const osScalar currentTime,
			const osScalar elapsedTime
			);
	} PhysicsMotionState;

	//-------------------------------------------------------------------------
	class AbstractVehicleMath
	{
	public:
		static btQuaternion readRotation( const OpenSteer::LocalSpaceData& kLocalSpaceData );
		static void writeRotation( const btQuaternion& kRotation, OpenSteer::LocalSpaceData& kLocalSpaceData );
		
	};



}




#endif  // __ABSTRACTVEHICLEMATH_H__
