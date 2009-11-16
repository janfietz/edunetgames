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
		AbstractVehicleUpdate( AbstractVehicle* pkVehicle ):
		  m_pkVehicle( pkVehicle ), m_pkCustomUpdated(NULL)
		  {
		  }
		  virtual ~AbstractVehicleUpdate(){}

		  //-------------------------------------------------------------------
		  // interface AbstractUpdated
		  virtual void setCustomUpdated( AbstractUpdated* pkUpdated )
		  {
			  this->m_pkCustomUpdated = pkUpdated;
		  }
		  virtual AbstractUpdated* getCustomUpdated( void ) const
		  {
			  return this->m_pkCustomUpdated;
		  }

		  //-------------------------------------------------------------------
		  bool isVehicleUpdate( void ) const
		  {
			  return (NULL != this->m_pkVehicle);
		  }

		  void setVehicle( AbstractVehicle* pkVehicle )
		  {
			  this->m_pkVehicle = pkVehicle;
		  }

		  AbstractVehicle& vehicle( void ) 
		  {
			  assert( NULL != this->m_pkVehicle );
			  return *this->m_pkVehicle;
		  }

	protected:
		AbstractVehicle* m_pkVehicle;
		AbstractUpdated* m_pkCustomUpdated;

	};

	//-------------------------------------------------------------------------
	class EulerVehicleUpdate : public AbstractVehicleUpdate {
		OS_DECLARE_BASE(AbstractVehicleUpdate)
	public:
		EulerVehicleUpdate( AbstractVehicle* pkVehicle ):
		BaseClass( pkVehicle ),
			m_kForce(Vec3::zero),
			_smoothedAcceleration(Vec3::zero)
		{
		}
		virtual ~EulerVehicleUpdate(){}

		void setForce( const Vec3& kForce ){ m_kForce = kForce; }

		//---------------------------------------------------------------------
		virtual void update( const osScalar /*currentTime*/, const osScalar elapsedTime );

	private:
		Vec3 m_kForce;
		Vec3 _smoothedAcceleration;
	};

	//-------------------------------------------------------------------------
	class SteeringForceVehicleUpdate : public AbstractVehicleUpdate {
		OS_DECLARE_BASE(AbstractVehicleUpdate)
	public:
		SteeringForceVehicleUpdate( AbstractVehicle* pkVehicle ):
		BaseClass( pkVehicle ),
			m_kForce(Vec3::zero)
		{
		}
		virtual ~SteeringForceVehicleUpdate(){}

		const Vec3& getForce( void ){ return m_kForce; }

		//---------------------------------------------------------------------
		virtual void update( const osScalar /*currentTime*/, const osScalar elapsedTime );
	private:
		Vec3 m_kForce;
	};

}




#endif  // __ABSTRACTVEHICLEUPDATE_H__
