#ifndef __SIMPLEPHYSICSVEHICLE_H__
#define __SIMPLEPHYSICSVEHICLE_H__

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

#include "EduNetCore/EduNetMacros.h"
#include "OpenSteer/SimpleVehicle.h"
#include "OpenSteerUT/AbstractVehicleUpdate.h"

//-----------------------------------------------------------------------------
namespace OpenSteer
{
	//-------------------------------------------------------------------------
	class SimplePhysicsVehicle : public SimpleVehicle
	{
		ET_DECLARE_BASE( SimpleVehicle )
	public:
		SimplePhysicsVehicle();
		virtual ~SimplePhysicsVehicle();

		OS_IMPLEMENT_CLASSNAME( SimplePhysicsVehicle )

		//---------------------------------------------------------------------------
		// extension
		void setMovesPlanar( bool bMovesPlanar )
		{
			this->_movesPlanar = bMovesPlanar;
		}

		const OpenSteer::EulerVehicleUpdate& getEulerUpdate( void ) const
		{
			return this->m_kEulerUpdate;
		}

		OpenSteer::EulerVehicleUpdate& accessEulerUpdate( void )
		{
			return this->m_kEulerUpdate;
		}

		const OpenSteer::SteeringForceVehicleUpdate& getSteeringForceUpdate( void ) const
		{
			return this->m_kSteeringForceUpdate;
		}

		OpenSteer::SteeringForceVehicleUpdate& accessSteeringForceUpdate( void )
		{
			return this->m_kSteeringForceUpdate;
		}

		virtual void draw( class AbstractRenderer*, 
					const float currentTime, const float elapsedTime ) OS_OVERRIDE;

		virtual void update (const float currentTime, const float elapsedTime);

		float getUpdateTickTime( void ) const;
		float getUpdateCurrentTime( void ) const { return this->m_fUpdateCurrentTime; };
		float getUpdateElapsedTime( void ) const { return this->m_fUpdateElapsedTime; };
	protected:
		OpenSteer::EulerVehicleUpdate m_kEulerUpdate;
		OpenSteer::SteeringForceVehicleUpdate m_kSteeringForceUpdate;
		float m_fAccumulatedElapsedTime;
	private:
		float m_fUpdateCurrentTime;
		float m_fUpdateElapsedTime;
	};

	EF_FORCEINLINE
	float SimplePhysicsVehicle::getUpdateTickTime( void ) const
	{
		// count the updateTicks a 50 hz
		const float fTickTime = 1.0f / 50.0f;
		return fTickTime;
	}

}


#endif // __SIMPLEPHYSICSVEHICLE_H__