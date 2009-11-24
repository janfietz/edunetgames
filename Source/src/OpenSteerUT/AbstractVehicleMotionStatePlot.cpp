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

#include "AbstractVehicleMotionStatePlot.h"
#include "SimplePhysicsVehicle.h"

//-----------------------------------------------------------------------------
namespace OpenSteer {

	AbstractVehicleMotionStatePlot::AbstractVehicleMotionStatePlot()
	{
		Color kColor0(0.0f, 1.0f, 0.0f);
		Color kColor1(1.0f, 0.0f, 0.0f);
		Profile::GraphValues& kValues0 = this->m_kLinearVelocity.accessValues(0).
			setName( "LinearVel" ).
			setColor( kColor0.r(), kColor0.g(), kColor0.b(), kColor0.a() );
//		kValues0.setColor( kColor0.r(), kColor0.g(), kColor0.b(), kColor0.a() );
		this->m_kAngularVelocity.accessValues(0).
			setName( "AngularVel" ).
			setColor( kColor0.r(), kColor0.g(), kColor0.b(), kColor0.a() );
		this->m_kSteeringForce.accessValues(0).
			setName( "SteeringForce" ).
			setColor( kColor0.r(), kColor0.g(), kColor0.b(), kColor0.a() );
		this->m_kLinearVelocity.accessValues(1).
			setName( "LinearVel" ).
			setColor( kColor1.r(), kColor1.g(), kColor1.b(), kColor1.a() );
		this->m_kAngularVelocity.accessValues(1).
			setName( "AngularVel" ).
			setColor( kColor1.r(), kColor1.g(), kColor1.b(), kColor1.a() );
		this->m_kSteeringForce.accessValues(1).
			setName( "SteeringForce" ).
			setColor( kColor1.r(), kColor1.g(), kColor1.b(), kColor1.a() );
	}

	AbstractVehicleMotionStatePlot::~AbstractVehicleMotionStatePlot()
	{
	}

	void AbstractVehicleMotionStatePlot::recordUpdate( AbstractVehicle* pkVehicle, const float currentTime, const float elapsedTime )
	{
		if( NULL == pkVehicle )
		{
			return;
		}
		size_t uiRecordIndex = pkVehicle->isRemoteObject() ? 1 : 0;
		// update motion state plot
		SimplePhysicsVehicle* pkPhysicsVehicle = dynamic_cast<SimplePhysicsVehicle*>(pkVehicle);
		if( pkPhysicsVehicle != NULL )
		{
			// a hack ...
			if( false )
			{
				OpenSteer::EulerVehicleUpdate& kEulerUpdateAccess = pkPhysicsVehicle->accessEulerUpdate();
				PhysicsMotionState& kUpdateState = kEulerUpdateAccess.accessMotionState();
				kUpdateState.updateMotionState( pkPhysicsVehicle, currentTime, elapsedTime );
			}

			const OpenSteer::EulerVehicleUpdate& kEulerUpdate = pkPhysicsVehicle->getEulerUpdate();
			const PhysicsMotionState& kState = kEulerUpdate.getMotionState();

			Profile::GraphValues& kLinearVelocityValues = 
				this->m_kLinearVelocity.accessValues(uiRecordIndex);
			kLinearVelocityValues.addValue( currentTime, kState.m_fLinearVelocity );

			Profile::GraphValues& kAngularVelocityValues = 
				this->m_kAngularVelocity.accessValues(uiRecordIndex);
			kAngularVelocityValues.addValue( currentTime, kState.m_fAngularVelocity );
			Profile::GraphValues& kSteeringForceValues = 
				this->m_kSteeringForce.accessValues(uiRecordIndex);
			kSteeringForceValues.addValue( currentTime, kState.m_kForce.length() );
		}
	}

	void AbstractVehicleMotionStatePlot::draw( float currentTime ) const
	{
		// draw only once
		if( currentTime == this->m_currentTime )
		{
			return;
		}
		this->m_currentTime = currentTime;
		// draw motion state plot
		const float fGraphStart = 100;
		const float fGraphHeight = 150;
		const float fGraphWidth = 400;
		Profile::GraphPlot kPlot;
		Profile::TGraphPointerArray kGraphArray;
		kGraphArray.push_back( &this->m_kLinearVelocity );
		kGraphArray.push_back( &this->m_kAngularVelocity );
		kGraphArray.push_back( &this->m_kSteeringForce );
		kPlot.draw( kGraphArray, 50, fGraphStart, fGraphWidth, fGraphHeight * kGraphArray.size() );
	}



} // namespace OpenSteer

