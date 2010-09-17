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

#include "PhysicsMotionState.h"
#include "EduNetCommon/EduNetMath.h"
#include "EduNetCommon/TCompressed.h"


namespace OpenSteer
{

	//-------------------------------------------------------------------------
	using namespace OpenSteer;

	//-------------------------------------------------------------------------
	bool TPhysicsMotionState::readLocalSpaceData( const LocalSpaceData& kLocalSpaceData )
	{
		if( writeLocalSpaceToTransform( kLocalSpaceData, this->m_kWorldTransform ) )
		{
			this->m_kLinearVelocity = kLocalSpaceData._linearVelocity;
			this->m_kAngularVelocity = kLocalSpaceData._angularVelocity;

			// in local space
			btMatrix3x3 kLocalBasis = this->m_kWorldTransform.getBasis().inverse();
			btVector3 kLocalLinearVelocity;
			getVector3( this->m_kLinearVelocity, kLocalLinearVelocity );
			kLocalLinearVelocity = kLocalBasis * kLocalLinearVelocity;
			btVector3 kLocalAngularVelocity;
			getVector3( this->m_kAngularVelocity, kLocalAngularVelocity );
			kLocalAngularVelocity = kLocalBasis * kLocalAngularVelocity;

			getVector3( kLocalLinearVelocity, this->m_kLocalLinearVelocity );
			getVector3( kLocalAngularVelocity, this->m_kLocalAngularVelocity );
			this->m_fLinearVelocity = this->m_kLinearVelocity.length();
			this->m_fAngularVelocity = this->m_kAngularVelocity.length();
			return true;
		}
		else
		{
			// this is a serious error what to do ?
			return false;
		}
	}

	//-------------------------------------------------------------------------
	void TPhysicsMotionState::writeLocalSpaceData( AbstractLocalSpace& kLocalSpace )
	{
		writeTransformToLocalSpace( this->m_kWorldTransform, kLocalSpace );
		kLocalSpace.setAngularVelocity( this->m_kAngularVelocity );
		kLocalSpace.setLinearVelocity( this->m_kLinearVelocity );
	}

	//-------------------------------------------------------------------------
	bool TPhysicsMotionState::updateMotionState( AbstractVehicle* pkVehicle, const osScalar currentTime, 
		const osScalar elapsedTime
		)
	{
		if( NULL == pkVehicle )
		{
			return false;
		}
		return this->updateMotionState( pkVehicle->getLocalSpaceData(), currentTime, elapsedTime );
	}

	//-------------------------------------------------------------------------
	bool TPhysicsMotionState::updateMotionState( const LocalSpaceData& kLocalSpaceData, const osScalar currentTime, 
		const osScalar elapsedTime
		)
	{
		// store new world transform
		btTransform kWorldTransform1;
		if( writeLocalSpaceToTransform( kLocalSpaceData, kWorldTransform1 ) )
		{
			this->updateMotionState( kWorldTransform1, currentTime, elapsedTime );
			// open steer is all ways treating the forward vector
			// as linear velocity direction
			// so - right now add this little correction here ???
#if 0
			this->m_kLinearVelocity = kLocalSpaceData._forward;
			this->m_kLinearVelocity *= this->m_fLinearVelocity;
			this->m_kLocalLinearVelocity = Vec3.forward;
			this->m_kLocalLinearVelocity *= this->m_fLinearVelocity;
#endif
			return true;
		}
		else
		{
			// this is a serious error what to do ?
			return false;
		}
	}

	//-------------------------------------------------------------------------
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

	//-------------------------------------------------------------------------
	void TPhysicsMotionState::integrateMotionState( TPhysicsMotionState& kTarget, const osScalar elapsedTime, size_t uiSteps ) const
	{
		PhysicsMotionState kLocalMotionState = (*this);
		for( size_t ui = 0; ui < uiSteps; ++ui )
		{
			kLocalMotionState.integrateMotionState( kTarget, elapsedTime );
			kLocalMotionState = kTarget;
		}
	}

	//-------------------------------------------------------------------------
	void TPhysicsMotionState::integrateMotionState( TPhysicsMotionState& kTarget, const osScalar elapsedTime ) const
	{
		if( elapsedTime <= 0 )
		{
			kTarget = (*this);
			return;
		}

		btVector3 kLinearVelocity, kAngularVelocity;
		getVector3( this->m_kLinearVelocity, kLinearVelocity );
		getVector3( this->m_kAngularVelocity, kAngularVelocity );
		btTransformUtil::integrateTransform(
			this->m_kWorldTransform,
			kLinearVelocity,
			kAngularVelocity,
			elapsedTime,
			kTarget.m_kWorldTransform );

		// compute local space velocities
		btMatrix3x3 kLocalBasis = this->m_kWorldTransform.getBasis().inverse();
		btVector3 kLocalLinearVelocity;
		getVector3( this->m_kLinearVelocity, kLocalLinearVelocity );
		kLocalLinearVelocity = kLocalBasis * kLocalLinearVelocity;
		btVector3 kLocalAngularVelocity;
		getVector3( this->m_kAngularVelocity, kLocalAngularVelocity );
		kLocalAngularVelocity = kLocalBasis * kLocalAngularVelocity;

		// set local velocities to integrated transform
		getVector3( kLocalLinearVelocity, kTarget.m_kLocalLinearVelocity );
		getVector3( kLocalAngularVelocity, kTarget.m_kLocalAngularVelocity );
		kTarget.m_fLinearVelocity = kTarget.m_kLinearVelocity.length();
		kTarget.m_fAngularVelocity = kTarget.m_kAngularVelocity.length();

		// transform local velocities to new world space
		kLinearVelocity = kTarget.m_kWorldTransform.getBasis() * kLocalLinearVelocity;
		kAngularVelocity = kTarget.m_kWorldTransform.getBasis() * kLocalAngularVelocity;
		getVector3( kLinearVelocity, kTarget.m_kLinearVelocity );
		getVector3( kAngularVelocity, kTarget.m_kAngularVelocity );
	}

} // end namespace OpenSteer

