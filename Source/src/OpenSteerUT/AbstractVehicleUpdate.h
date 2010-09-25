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

//#include "EduNetCommon/EduNetCommon.h"
#include "EduNetCommon/TUpdatePeriod.h"
#include "OpenSteer/Annotation.h"
#include "OpenSteerUT/PhysicsMotionState.h"

class btTransform;

namespace OpenSteer
{
using namespace OpenSteer;

	typedef OpenSteer::AnnotationMixin<AbstractUpdated> TAbstractAnnotatedUpdated;
	//-------------------------------------------------------------------------
	class AbstractVehicleUpdate : public TAbstractAnnotatedUpdated {
	public:
		AbstractVehicleUpdate( AbstractVehicle* pkVehicle ):
		  m_pkVehicle( pkVehicle ), m_pkCustomUpdated(NULL), m_bEnabled( true )
		  {
		  }
		  virtual ~AbstractVehicleUpdate(){}

		  //-------------------------------------------------------------------
		  // interface AbstractUpdated
		  virtual void update( const osScalar currentTime, const osScalar elapsedTime )
		  {
			  if( 0 != this->getCustomUpdated() )
			  {
				  // in case the custom updater decides to call the base class
				  // prevent infinite recursion, store the custom updater locally
				  // and restore it once done with the update
				  AbstractUpdated* pkCustomUpdated = this->getCustomUpdated();
				  this->setCustomUpdated( 0 );
				  pkCustomUpdated->updateCustom( this, currentTime, elapsedTime );
				  this->setCustomUpdated( pkCustomUpdated );
			  }
		  }

		  virtual void updateCustom( AbstractUpdated* /*pkParent*/, const osScalar /*currentTime*/, const osScalar /*elapsedTime*/ )
		  {
			  // nothing to do here
			  return;
		  }

		  virtual void setCustomUpdated( AbstractUpdated* pkUpdated )
		  {
			  this->m_pkCustomUpdated = pkUpdated;
		  }

		  virtual AbstractUpdated* getCustomUpdated( void ) const
		  {
			  return this->m_pkCustomUpdated;
		  }

		  //-------------------------------------------------------------------
		  virtual void setVehicle( AbstractVehicle* pkVehicle )
		  {
			  this->m_pkVehicle = pkVehicle;
		  }

		  bool isVehicleUpdate( void ) const
		  {
			  return (NULL != this->m_pkVehicle);
		  }

		  AbstractVehicle& vehicle( void ) const
		  {
			  assert( NULL != this->m_pkVehicle );
			  return *this->m_pkVehicle;
		  }

		  virtual bool isEnabled( void ) const { return this->m_bEnabled; };
		  virtual void setEnabled( bool bEnabled ){ this->m_bEnabled = bEnabled; };

// 		  virtual class AbstractServerVehicleUpdate& serverVehicleUpdate(void) = 0;
// 		  virtual class AbstractClientVehicleUpdate& clientVehicleUpdate(void) = 0;

	protected:
		AbstractVehicle* m_pkVehicle;
		AbstractUpdated* m_pkCustomUpdated;
		bool m_bEnabled;

	};

	//----------------------------------------------------------------------------
	class AbstractServerVehicleUpdate : public AbstractVehicleUpdate
	{
	public:
// 		virtual EServerVehicleMode getServerVehicleMode( void ) const = 0;
// 		virtual void resetExtrapolationData ( const class SimpleNetworkVehicle& kVehicle) = 0;
	};

	//----------------------------------------------------------------------------
	class AbstractClientVehicleUpdate : public AbstractVehicleUpdate
	{
	public:
	};

	enum EEulerUpdateMode
	{
		EEulerUpdateMode_Accelerate,
		EEulerUpdateMode_IntegrateMotionState,
		EEulerUpdateMode_Count,
	};

	//-------------------------------------------------------------------------
	class EulerVehicleUpdate : public AbstractVehicleUpdate
	{
		OS_DECLARE_BASE(AbstractVehicleUpdate)
	public:
		EulerVehicleUpdate( AbstractVehicle* pkVehicle ):
		BaseClass( pkVehicle ),
			m_kSmoothedAcceleration(Vec3::zero),
			m_eUpdateMode( EEulerUpdateMode_Accelerate )
		{
		}
		virtual ~EulerVehicleUpdate(){}

		void setForce( const Vec3& kForce ){ m_kMotionState.m_kForce = kForce; }
		const Vec3& getForce( void ) const { return m_kMotionState.m_kForce; }
		const PhysicsMotionState& getMotionState( void ) const { return m_kMotionState; }
		PhysicsMotionState& accessMotionState( void ) { return m_kMotionState; }

		//---------------------------------------------------------------------
		virtual void update( const osScalar currentTime, const osScalar elapsedTime );

		virtual void setVehicle( AbstractVehicle* pkVehicle );

		//---------------------------------------------------------------------
		EEulerUpdateMode getUpdateMode( void ) const { return this->m_eUpdateMode; }
		void setUpdateMode( EEulerUpdateMode eMode ){ this->m_eUpdateMode = eMode; }
		void updateMotionState( const osScalar currentTime, const osScalar elapsedTime );
	private:

		void integrateMotionState( const osScalar currentTime, const osScalar elapsedTime );
		Vec3 updateLinearVelocity( const osScalar currentTime, const osScalar elapsedTime );

		PhysicsMotionState m_kMotionState;
		Vec3 m_kSmoothedAcceleration;
		EEulerUpdateMode m_eUpdateMode;
	};

	//-------------------------------------------------------------------------
	class SteeringForceVehicleUpdate : public AbstractVehicleUpdate {
		OS_DECLARE_BASE(AbstractVehicleUpdate)
	public:
		SteeringForceVehicleUpdate( AbstractVehicle* pkVehicle );
		virtual ~SteeringForceVehicleUpdate(){}

		Vec3 determineCombinedSteering( const osScalar elapsedTime ) const;

		void setForce( const Vec3& kForce, bool bCompress = true );
		const Vec3& getForce( void ) const { return this->m_kForce; }
		const CompressedVector& getCompressedForce( void ) const { return m_cForce; }

		//---------------------------------------------------------------------
		virtual void update( const osScalar /*currentTime*/, const osScalar elapsedTime );

	private:
		Vec3 m_kForce;
		CompressedVector m_cForce;
		TUpdatePeriod<osScalar, FloatMathLimits> m_kUpdatePeriod;
	};

}




#endif  // __ABSTRACTVEHICLEUPDATE_H__
