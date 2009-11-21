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
		this->m_kLinearVelocity.accessValues(0).setName( "LinearVel" );
		this->m_kAngularVelocity.accessValues(0).setName( "AngularVel" );
		this->m_kSteeringForce.accessValues(0).setName( "SteeringForce" );
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
				this->m_kLinearVelocity.accessValues(0);
			kLinearVelocityValues.addValue( currentTime, kState.m_fLinearVelocity );

			Profile::GraphValues& kAngularVelocityValues = 
				this->m_kAngularVelocity.accessValues(0);
			kAngularVelocityValues.addValue( currentTime, kState.m_fAngularVelocity );
			Profile::GraphValues& kSteeringForceValues = 
				this->m_kSteeringForce.accessValues(0);
			kSteeringForceValues.addValue( currentTime, kState.m_kForce.length() );

			Color kColor;
			if( true == pkPhysicsVehicle->isRemoteObject() )
			{
				kColor = gGreen;
			}
			else
			{
				kColor = gRed;
			}
			this->m_kLinearVelocity.setColor( kColor.r(), kColor.g(), kColor.b(), kColor.a() );
			this->m_kAngularVelocity.setColor( kColor.r(), kColor.g(), kColor.b(), kColor.a() );
			this->m_kSteeringForce.setColor( kColor.r(), kColor.g(), kColor.b(), kColor.a() );

		}
	}

	void AbstractVehicleMotionStatePlot::draw( void ) const
	{
		// draw motion state plot
		const float fGraphStart = 220;
		const float fGraphHeight = 175;
		const float fGraphWidth = 400;
		Profile::GraphPlot kPlot;
		Profile::TGraphPointerArray kGraphArray;
		kGraphArray.push_back( &this->m_kLinearVelocity );
		kGraphArray.push_back( &this->m_kAngularVelocity );
		kGraphArray.push_back( &this->m_kSteeringForce );
		kPlot.draw( kGraphArray, 50, fGraphStart, fGraphWidth, fGraphHeight * kGraphArray.size() );
	}



} // namespace OpenSteer

