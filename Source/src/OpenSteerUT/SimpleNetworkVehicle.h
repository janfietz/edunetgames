#ifndef __SIMPLENETWORKVEHICLE_H__
#define __SIMPLENETWORKVEHICLE_H__

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

#include "NetworkVehicle.h"
#include "VehicleClassIds.h"
#include "EduNetCommon/EduNetCommon.h"
#include "EduNetCommon/TUpdatePeriod.h"
#include "EduNetProfile/GraphPlot.h"
#include "OpenSteerUT/AbstractVehicleUpdate.h"

namespace OpenSteer {

	//-------------------------------------------------------------------------
	class AbstractVehicleMotionStatePlot
	{
	public:
		AbstractVehicleMotionStatePlot();
		virtual ~AbstractVehicleMotionStatePlot();

		void recordUpdate( AbstractVehicle* pkVehicle, const float currentTime, const float elapsedTime );
		void draw( void ) const;

		Profile::GraphValuesArray m_kLinearVelocity;
		Profile::GraphValuesArray m_kAngularVelocity;
		Profile::GraphValuesArray m_kSteeringForce;

	};

	//-------------------------------------------------------------------------
	class AbstractSimpleNetworkVehicle : public SimpleVehicle, public AbstractNetworkVehicle
	{
	public:
		AbstractSimpleNetworkVehicle() { /* Nothing to do. */ }
		virtual ~AbstractSimpleNetworkVehicle() { /* Nothing to do. */ }
	};


	// SimpleVehicle_1 adds concrete LocalSpace methods to AbstractVehicle
	typedef NetworkVehicleMixin<AbstractSimpleNetworkVehicle> SimpleNetworkVehicle_1;

	class SimpleNetworkVehicle : public SimpleNetworkVehicle_1
	{
		ET_DECLARE_BASE( SimpleNetworkVehicle_1 )
	public:
		SimpleNetworkVehicle();
		virtual ~SimpleNetworkVehicle();

		OS_IMPLEMENT_CLASSNAME( SimpleNetworkVehicle )

		const OpenSteer::EulerVehicleUpdate& getEulerUpdate( void ) const
		{
			return this->m_kEulerUpdate;
		}
		OpenSteer::EulerVehicleUpdate& accessEulerUpdate( void )
		{
			return this->m_kEulerUpdate;
		}

	protected:
		OpenSteer::EulerVehicleUpdate m_kEulerUpdate;
		OpenSteer::SteeringForceVehicleUpdate m_kSteeringForceUpdate;

	private:


	};


} // namespace OpenSteer

#endif