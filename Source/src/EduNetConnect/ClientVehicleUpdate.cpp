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

#include "EduNetConnect/ClientVehicleUpdate.h"
#include "EduNetConnect/SimpleNetworkVehicle.h"
#include "OpenSteerUT/AbstractVehicleMath.h"

using namespace OpenSteer;

//-----------------------------------------------------------------------------
void ClientVehicleUpdate::updateCustom( AbstractUpdated* pkParent, const osScalar currentTime, const osScalar elapsedTime )
{
	SimpleNetworkVehicle* pkNetworkVehicle = dynamic_cast<SimpleNetworkVehicle*>(pkParent);
	SimpleProxyVehicle& kProxy = pkNetworkVehicle->accessProxyVehicle();
	if( true == kProxy.m_bHasNewData )
	{
		kProxy.m_kProxyMotionState.readLocalSpaceData( kProxy.getLocalSpaceData() );
		if( ( kProxy.m_bReveivedDataConfig[ ESerializeDataType_UpdateTicks ] ) &&
			( kProxy.getLocalSpaceData()._updateTicks > 0 ) )
		{
			// compute the time difference
			// local update ticks / against server update ticks
			// usually one would expect one of both sides being late
			// if this it not the case the server is running in the future
			const size_t uiClientTicks = pkNetworkVehicle->getLocalSpaceData()._updateTicks;
			const size_t uiServerTicks = kProxy.getLocalSpaceData()._updateTicks;
			if( uiClientTicks < uiServerTicks )
			{
				kProxy.m_kProxyMotionState.writeLocalSpaceData( *pkNetworkVehicle );
				pkNetworkVehicle->accessLocalSpaceData()._updateTicks = uiServerTicks;
				pkNetworkVehicle->updateBase( currentTime, elapsedTime );
				/*
				const size_t uiTickDifference = uiServerTicks - uiClientTicks;
				float fTickDifferenceTime = pkNetworkVehicle->getUpdateTickTime() * uiTickDifference;
				if( ( fTickDifferenceTime > 0 ) && ( fTickDifferenceTime < 5.0f ) )
				{
					PhysicsMotionState kExtrapolatedMotionState;
					kProxy.m_kProxyMotionState.integrateMotionState( 
						kExtrapolatedMotionState, pkNetworkVehicle->getUpdateTickTime(), uiTickDifference );
					kExtrapolatedMotionState.writeLocalSpaceData( *pkNetworkVehicle );
				}
				else
				{
					kProxy.m_kProxyMotionState.writeLocalSpaceData( *pkNetworkVehicle );
				}
				*/
			}
			else if( uiClientTicks > uiServerTicks )
			{
				const size_t uiTickDifference = uiClientTicks - uiServerTicks;
				float fTickDifferenceTime = pkNetworkVehicle->getUpdateTickTime() * uiTickDifference;
				if( ( fTickDifferenceTime > 0 ) && ( fTickDifferenceTime < 5.0f ) )
				{
					PhysicsMotionState kExtrapolatedMotionState;
					kProxy.m_kProxyMotionState.integrateMotionState( 
						kExtrapolatedMotionState, pkNetworkVehicle->getUpdateTickTime(), uiTickDifference );
					kExtrapolatedMotionState.writeLocalSpaceData( *pkNetworkVehicle );
				}
				else
				{
					kProxy.m_kProxyMotionState.writeLocalSpaceData( *pkNetworkVehicle );
				}
				pkNetworkVehicle->accessLocalSpaceData()._updateTicks = uiServerTicks;
			}
			else
			{
				kProxy.m_kProxyMotionState.writeLocalSpaceData( *pkNetworkVehicle );
				pkNetworkVehicle->accessLocalSpaceData()._updateTicks = uiServerTicks;
			}

			// update the motion state manually now
			PhysicsMotionState kNetworkVehicleMotionState = pkNetworkVehicle->accessEulerUpdate().accessMotionState();
			kNetworkVehicleMotionState.readLocalSpaceData( pkNetworkVehicle->getLocalSpaceData() );
			kNetworkVehicleMotionState.m_fLastUpdateTime = currentTime;
			pkNetworkVehicle->setLastSteeringForce( kProxy.lastSteeringForce() );

		}
		else
		{
			// now read the proxy and apply the data to the scene vehicle
			// position interpolation
			float fFactor = 0.5;
			osVector3 kNewPosition = interpolate( fFactor, pkNetworkVehicle->position(), kProxy.getLocalSpaceData()._position);

			osVector3 kNewForward = interpolate( fFactor, pkNetworkVehicle->forward(), kProxy.getLocalSpaceData()._forward);
			kNewForward.normalize();
			kProxy.accessLocalSpaceData()._forward = kNewForward;
			kProxy.accessLocalSpaceData()._position = kNewPosition;

			pkNetworkVehicle->setLocalSpaceData( kProxy.getLocalSpaceData() );
			pkNetworkVehicle->setSpeed( kProxy.speed() );


			// update the motion state manually now
			pkNetworkVehicle->accessEulerUpdate().updateMotionState( currentTime, elapsedTime );
		}


		if( ( kProxy.m_bReveivedDataConfig[ ESerializeDataType_Force ] ) )
		{
			pkNetworkVehicle->setLastSteeringForce( kProxy.lastSteeringForce() );
		}
		if( ( kProxy.m_bReveivedDataConfig[ ESerializeDataType_AngularVelocity ] ) )
		{
			pkNetworkVehicle->setAngularVelocity( kProxy.angularVelocity() );
		}
		if( ( kProxy.m_bReveivedDataConfig[ ESerializeDataType_LinearVelocity ] ) )
		{
			pkNetworkVehicle->setLinearVelocity( kProxy.linearVelocity() );
		}
	}
	else
	{
		pkNetworkVehicle->updateBase( currentTime, elapsedTime );
	}
}

//-------------------------------------------------------------------------
void ClientVehicleUpdate::update( const osScalar currentTime, const osScalar elapsedTime )
{

}


