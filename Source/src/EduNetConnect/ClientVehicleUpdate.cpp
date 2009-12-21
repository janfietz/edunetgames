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
		this->m_eLastUpdateMode = this->determineUpdateMode( *pkNetworkVehicle );
		// add the new data to the history
		kProxy.m_kLocalSpaceData.addValue( kProxy.getLocalSpaceData() )._steeringForce = 
			kProxy.getSteeringForceUpdate().getForce();
	}
	else
	{

	}
	switch( this->m_eLastUpdateMode )
	{
	case( EVehicleUpdateMode_Unknown ):
		this->updateUnknown( *pkNetworkVehicle, currentTime, elapsedTime );
		break;
	case( EVehicleUpdateMode_Position ):
		this->updatePosition( *pkNetworkVehicle, currentTime, elapsedTime );
		break;
	case( EVehicleUpdateMode_PositionOrientation ):
		this->updatePositionOrientation( *pkNetworkVehicle, currentTime, elapsedTime );
		break;
	case( EVehicleUpdateMode_BruteForce ):
		this->updateBruteForce( *pkNetworkVehicle, currentTime, elapsedTime );
		break;
	case( EVehicleUpdateMode_PhysicsMotion ):
		this->updatePhysicsMotion( *pkNetworkVehicle, currentTime, elapsedTime );
		break;
	case( EVehicleUpdateMode_ForwardSpeed ):
		this->updateForwardSpeed( *pkNetworkVehicle, currentTime, elapsedTime );
		break;
	case( EVehicleUpdateMode_Steer ):
		this->updateSteer( *pkNetworkVehicle, currentTime, elapsedTime );
		break;
	}
}

//-------------------------------------------------------------------------
void ClientVehicleUpdate::update( const osScalar currentTime, const osScalar elapsedTime )
{

}

//-------------------------------------------------------------------------
EVehicleUpdateMode ClientVehicleUpdate::determineUpdateMode( const class SimpleNetworkVehicle& kVehicle ) const
{
	EVehicleUpdateMode eType = EVehicleUpdateMode_Unknown;
	const SimpleProxyVehicle& kProxy = kVehicle.getProxyVehicle();

	const bool bHasPositionUpdate = 
		( kProxy.m_bSerializedDataTypes[ ESerializeDataType_Position ] );
	const bool bHasOrientationUpdate = 
		( kProxy.m_bSerializedDataTypes[ ESerializeDataType_Forward ] ) ||
		( kProxy.m_bSerializedDataTypes[ ESerializeDataType_Side ] ) ||
		( kProxy.m_bSerializedDataTypes[ ESerializeDataType_Up ] ) ||
		( kProxy.m_bSerializedDataTypes[ ESerializeDataType_Orientation ] ) ||
		( kProxy.m_bSerializedDataTypes[ ESerializeDataType_CompressedOrientation1 ] ) ||
		( kProxy.m_bSerializedDataTypes[ ESerializeDataType_CompressedOrientation2 ] );
	const bool bHasForceUpdate = 
		( kProxy.m_bSerializedDataTypes[ ESerializeDataType_Force ] ) ||
		( kProxy.m_bSerializedDataTypes[ ESerializeDataType_CompressedForce ] );
	const bool bHasVelocityUpdate = 
		( kProxy.m_bSerializedDataTypes[ ESerializeDataType_AngularVelocity ] ) ||
		( kProxy.m_bSerializedDataTypes[ ESerializeDataType_LinearVelocity ] );
	const bool bHasSpeedUpdate = 
		( kProxy.m_bSerializedDataTypes[ ESerializeDataType_Speed ] ) ;

	if( bHasPositionUpdate && bHasOrientationUpdate && bHasVelocityUpdate )
	{
		eType = EVehicleUpdateMode_PhysicsMotion;
	}
	else if( bHasPositionUpdate && bHasOrientationUpdate && bHasForceUpdate )
	{
		eType = EVehicleUpdateMode_Steer;
	}
	else if( bHasPositionUpdate && bHasOrientationUpdate && bHasSpeedUpdate )
	{
		eType = EVehicleUpdateMode_ForwardSpeed;
	}
	else if( bHasPositionUpdate && bHasOrientationUpdate )
	{
		eType = EVehicleUpdateMode_PositionOrientation;
	}
	else if( bHasPositionUpdate )
	{
		eType = EVehicleUpdateMode_Position;
	}
	return eType;
}

//-------------------------------------------------------------------------
void ClientVehicleUpdate::updatePosition( 
class SimpleNetworkVehicle& kVehicle, const osScalar currentTime, const osScalar elapsedTime )
{
	float m_sThreshold = 0.1f;
	SimpleProxyVehicle& kProxy = kVehicle.accessProxyVehicle();
	if( true == kProxy.m_bHasNewData )
	{
		LocalSpaceData& kCurrentLocalSpaceData = kVehicle.accessLocalSpaceData();
		const size_t currentUpdateTicks = kCurrentLocalSpaceData._updateTicks;
		// preserve client updateTicks ?
		const bool bPreserveUpdateTicks = true;
		osVector3 kDirection; 
		Vec3 tmp = kProxy.position() - kVehicle.position() ;	
		bool bTr = true;
		if(tmp.length() > m_sThreshold){
			tmp = kVehicle.position() + tmp * 0.5f;
			kDirection = tmp - kVehicle.position();
			kVehicle.setPosition(tmp);
			bTr = false;
		}

		size_t uiReceivedRecords = kProxy.m_kLocalSpaceData.size(); 
		if( uiReceivedRecords >= 2 )
		{

			const LocalSpaceData* pkLocalSpaceData[2] =
			{
				&kProxy.m_kLocalSpaceData[ uiReceivedRecords - 2 ], // 0 previous
				&kProxy.m_kLocalSpaceData[ uiReceivedRecords - 1 ], // 1 last
			};

			
			
			
			if(bTr) kDirection = pkLocalSpaceData[1]->_position - pkLocalSpaceData[0]->_position;
			//kVehicle.setLocalSpaceData(kProxy.getLocalSpaceData(), bPreserveUpdateTicks);

			// compute direction
			
			const float fDistance = kDirection.length();
			float fSpeed = 0;
			if( fDistance > 0 )
			{
				kVehicle.setForward( kDirection / fDistance );
				kVehicle.regenerateLocalSpace( kVehicle.forward(), 0 );

				// compute velocity
				const size_t uiTickDifference = pkLocalSpaceData[1]->_updateTicks - pkLocalSpaceData[0]->_updateTicks;
				if( uiTickDifference > 0)
				{
					float fTickDifferenceTime = kVehicle.getUpdateTickTime() * uiTickDifference;
					fSpeed = fDistance / fTickDifferenceTime;
					kProxy.setSpeed( fSpeed );
					kVehicle.setSpeed( kProxy.speed() );
					kVehicle.setLinearVelocity( kVehicle.velocity() );
				}
			}
		}
	}
	else
	{
		kVehicle.setAngularVelocity( Vec3::zero );
	}
	const EEulerUpdateMode ePrevMode = kVehicle.getEulerUpdate().getUpdateMode();
	kVehicle.accessEulerUpdate().setUpdateMode( EEulerUpdateMode_IntegrateMotionState );
	kVehicle.updateBase( currentTime, elapsedTime );
	kVehicle.accessEulerUpdate().setUpdateMode( ePrevMode );

	kVehicle.setSpeed( kProxy.speed() );
	kVehicle.setLinearVelocity( kVehicle.velocity() );
}

//-------------------------------------------------------------------------
void ClientVehicleUpdate::updatePositionOrientation( 
class SimpleNetworkVehicle& kVehicle, const osScalar currentTime, const osScalar elapsedTime )
{
	SimpleProxyVehicle& kProxy = kVehicle.accessProxyVehicle();
	if( true == kProxy.m_bHasNewData )
	{
		LocalSpaceData& kCurrentLocalSpaceData = kVehicle.accessLocalSpaceData();
		const size_t uiReceivedRecords = kProxy.m_kLocalSpaceData.size(); 

		const size_t currentUpdateTicks = kCurrentLocalSpaceData._updateTicks;
		// preserve client updateTicks ?
		const bool bPreserveUpdateTicks = true;

		float fSpeed = -1; // invalid

		// first time data
		if( uiReceivedRecords == 1 )
		{
			// preserve client updateTicks ?
			const bool bPreserveUpdateTicks = true;
			kVehicle.setLocalSpaceData( kProxy.getLocalSpaceData(), bPreserveUpdateTicks );
			fSpeed = 0;
		}
		else
		{
			// client side interpolation
			// compute distance between server and client position
			Vec3 kDistance = kProxy.position() - kVehicle.position();
			const float fDistanceSquared = kDistance.lengthSquared();
			const float fDistanceThreshhold = NetworkVehicle::ms_ClientSideInterpolation.m_fDistanceThreshHold *
				NetworkVehicle::ms_ClientSideInterpolation.m_fDistanceThreshHold;

			// position interpolation
			if( fDistanceSquared > fDistanceThreshhold )
			{
				const float fDistance = sqrtf( fDistanceSquared ) * NetworkVehicle::ms_ClientSideInterpolation.m_fPositionInterpolationFactor;
				fSpeed = fDistance / elapsedTime;
				Vec3 kInterpolationOffset = kDistance * NetworkVehicle::ms_ClientSideInterpolation.m_fPositionInterpolationFactor;
				Vec3 kNewPosition = kVehicle.position() + kInterpolationOffset;
				// preserve client updateTicks ?
				const bool bPreserveUpdateTicks = true;
				kVehicle.setLocalSpaceData( kProxy.getLocalSpaceData(), bPreserveUpdateTicks );
				kVehicle.setPosition( kNewPosition );
			}

			// orientation interpolation
			const float fDot = kVehicle.forward().dot( kProxy.forward() );
			if( fDot > 0.5 )
			{
				Vec3 kNewForward = kProxy.forward() + kVehicle.forward();
				kNewForward *= 0.5f;
				kNewForward.normalize();
				kVehicle.setForward( kNewForward );
				kVehicle.regenerateLocalSpace( kVehicle.forward(), 0 );
			}
			else
			{
				kVehicle.setForward( kProxy.forward() );
				kVehicle.regenerateLocalSpace( kVehicle.forward(), 0 );
			}
		}

		if( uiReceivedRecords >= 2 && fSpeed < 0 )
		{
			const LocalSpaceData* pkLocalSpaceData[2] =
			{
				&kProxy.m_kLocalSpaceData[ uiReceivedRecords - 2 ], // 0 previous
				&kProxy.m_kLocalSpaceData[ uiReceivedRecords - 1 ], // 1 last
			};

			// compute direction
			osVector3 kDirection = pkLocalSpaceData[1]->_position - pkLocalSpaceData[0]->_position;
			const float fDistance = kDirection.length();
			if( fDistance > 0 )
			{
				// compute velocity
				const size_t uiTickDifference = pkLocalSpaceData[1]->_updateTicks - pkLocalSpaceData[0]->_updateTicks;
				if( uiTickDifference > 0 )
				{
					float fTickDifferenceTime = kVehicle.getUpdateTickTime() * uiTickDifference;
					fSpeed = fDistance / fTickDifferenceTime;
				}
			}
			else
			{
				fSpeed = 0;
			}
		}

		if( fSpeed >= 0 )
		{
			kProxy.setSpeed( fSpeed );
			kVehicle.setSpeed( kProxy.speed() );
			kVehicle.setLinearVelocity( kVehicle.velocity() );
		}
	}
	else
	{
		kVehicle.setAngularVelocity( Vec3::zero );
	}
	const EEulerUpdateMode ePrevMode = kVehicle.getEulerUpdate().getUpdateMode();
	kVehicle.accessEulerUpdate().setUpdateMode( EEulerUpdateMode_IntegrateMotionState );
	kVehicle.updateBase( currentTime, elapsedTime );
	kVehicle.accessEulerUpdate().setUpdateMode( ePrevMode );

	kVehicle.setSpeed( kProxy.speed() );
	kVehicle.setLinearVelocity( kVehicle.velocity() );
}

//-------------------------------------------------------------------------
void ClientVehicleUpdate::updateBruteForce( 
	class SimpleNetworkVehicle& kVehicle, const osScalar currentTime, const osScalar elapsedTime )
{
	const SimpleProxyVehicle& kProxy = kVehicle.getProxyVehicle();
	if( true == kProxy.m_bHasNewData )
	{
		LocalSpaceData& kCurrentLocalSpaceData = kVehicle.accessLocalSpaceData();
		const size_t currentUpdateTicks = kCurrentLocalSpaceData._updateTicks;
		// preserve client updateTicks ?
		const bool bPreserveUpdateTicks = true;

		kVehicle.setLocalSpaceData( kProxy.getLocalSpaceData(), bPreserveUpdateTicks );
	}
	else
	{
		kVehicle.setAngularVelocity( Vec3::zero );
		kVehicle.setLinearVelocity( Vec3::zero );
	}
	const EEulerUpdateMode ePrevMode = kVehicle.getEulerUpdate().getUpdateMode();
	kVehicle.accessEulerUpdate().setUpdateMode( EEulerUpdateMode_IntegrateMotionState );
	kVehicle.updateBase( currentTime, elapsedTime );
	kVehicle.accessEulerUpdate().setUpdateMode( ePrevMode );
}

//-------------------------------------------------------------------------
void ClientVehicleUpdate::updatePhysicsMotion( 
	class SimpleNetworkVehicle& kVehicle, const osScalar currentTime, const osScalar elapsedTime )
{
	const SimpleProxyVehicle& kProxy = kVehicle.getProxyVehicle();
	if( true == kProxy.m_bHasNewData )
	{
		LocalSpaceData& kCurrentLocalSpaceData = kVehicle.accessLocalSpaceData();
		const size_t currentUpdateTicks = kCurrentLocalSpaceData._updateTicks;

		// client side interpolation
		// compute distance between server and client position
		Vec3 kDistance = kProxy.position() - kVehicle.position();
		const float fDistanceThreshhold = NetworkVehicle::ms_ClientSideInterpolation.m_fDistanceThreshHold *
			NetworkVehicle::ms_ClientSideInterpolation.m_fDistanceThreshHold;
		if( kDistance.lengthSquared() > fDistanceThreshhold )
		{
			Vec3 kNewPosition = kVehicle.position() + kDistance * NetworkVehicle::ms_ClientSideInterpolation.m_fPositionInterpolationFactor;
			// preserve client updateTicks ?
			const bool bPreserveUpdateTicks = true;
			kVehicle.setLocalSpaceData( kProxy.getLocalSpaceData(), bPreserveUpdateTicks );
			kVehicle.setPosition( kNewPosition );

			// manually update the motion state once
//			kVehicle.accessEulerUpdate().updateMotionState( currentTime, elapsedTime );
		}
	}
	else
	{
		kVehicle.setAngularVelocity( Vec3::zero );
	}
	const EEulerUpdateMode ePrevMode = kVehicle.getEulerUpdate().getUpdateMode();
	kVehicle.accessEulerUpdate().setUpdateMode( EEulerUpdateMode_IntegrateMotionState );
	kVehicle.updateBase( currentTime, elapsedTime );
	kVehicle.accessEulerUpdate().setUpdateMode( ePrevMode );

//	printf( "vehicle speed %f\n", kVehicle.speed() );
}

//-------------------------------------------------------------------------
void ClientVehicleUpdate::updateForwardSpeed( 
	class SimpleNetworkVehicle& kVehicle, const osScalar currentTime, const osScalar elapsedTime )
{
	const SimpleProxyVehicle& kProxy = kVehicle.getProxyVehicle();
	if( true == kProxy.m_bHasNewData )
	{
		LocalSpaceData& kCurrentLocalSpaceData = kVehicle.accessLocalSpaceData();
		const size_t currentUpdateTicks = kCurrentLocalSpaceData._updateTicks;
		// preserve client updateTicks ?
		const bool bPreserveUpdateTicks = true;

		kVehicle.setLocalSpaceData( kProxy.getLocalSpaceData(), bPreserveUpdateTicks );

		kVehicle.accessEulerUpdate().updateMotionState( currentTime, elapsedTime );

		kVehicle.setSpeed( kProxy.speed() );
		kVehicle.setLinearVelocity( kVehicle.velocity() );
	}
	else
	{
		kVehicle.setAngularVelocity( Vec3::zero );
	}
	const EEulerUpdateMode ePrevMode = kVehicle.getEulerUpdate().getUpdateMode();
	kVehicle.accessEulerUpdate().setUpdateMode( EEulerUpdateMode_IntegrateMotionState );
	kVehicle.updateBase( currentTime, elapsedTime );
	kVehicle.accessEulerUpdate().setUpdateMode( ePrevMode );

	kVehicle.setSpeed( kProxy.speed() );
	kVehicle.setLinearVelocity( kVehicle.velocity() );
}

#define ET_STEERUPDATE_ACCELERATE 1
#define ET_STEERUPDATE_NO_NEWDATA_INTEGRATE 0
#define ET_STEERUPDATE_ADJUST_FORCE_00 0
#define ET_STEERUPDATE_ADJUST_FORCE_01 0

//-------------------------------------------------------------------------
void ClientVehicleUpdate::updateSteer( 
	class SimpleNetworkVehicle& kVehicle, const osScalar currentTime, const osScalar elapsedTime )
{
	const SimpleProxyVehicle& kProxy = kVehicle.getProxyVehicle();
	if( true == kProxy.m_bHasNewData )
	{
		LocalSpaceData& kCurrentLocalSpaceData = kVehicle.accessLocalSpaceData();
		const size_t currentUpdateTicks = kCurrentLocalSpaceData._updateTicks;
		// preserve client updateTicks ?
		const bool bPreserveUpdateTicks = true;

		kVehicle.setLocalSpaceData( kProxy.getLocalSpaceData(), bPreserveUpdateTicks );
		SteeringForceVehicleUpdate& kSteeringForceUpdate = kVehicle.accessSteeringForceUpdate();
		kSteeringForceUpdate.setForce( kProxy.getSteeringForceUpdate().getForce() );
	}
	else
	{
#if ET_STEERUPDATE_ADJUST_FORCE_00 // experimental code 1
		kSteeringForceUpdate.setForce( kSteeringForceUpdate.getForce() * 0.75f );
#endif
#if ET_STEERUPDATE_ADJUST_FORCE_01 // experimental code 2
		SteeringForceVehicleUpdate& kSteeringForceUpdate = kVehicle.accessSteeringForceUpdate();
		// adjust the steering force each tick
		Vec3 kCurrentForce = kSteeringForceUpdate.getForce();
		Vec3 kAdjustedForce = kVehicle.adjustRawSteeringForce( kCurrentForce, elapsedTime );
		kSteeringForceUpdate.setForce( kAdjustedForce );
#endif
	}
	const EEulerUpdateMode ePrevMode = kVehicle.getEulerUpdate().getUpdateMode();
	SteeringForceVehicleUpdate& kSteeringForceUpdate = kVehicle.accessSteeringForceUpdate();
	const bool bSteeringForceUpdateEnabled = kSteeringForceUpdate.isEnabled();
	if( true == kProxy.m_bHasNewData )
	{
#if ET_STEERUPDATE_ACCELERATE
		kVehicle.accessEulerUpdate().setUpdateMode( EEulerUpdateMode_Accelerate );
		// new server data - apply those
		kSteeringForceUpdate.setEnabled( false );
#endif
	}
	else
	{
#if ET_STEERUPDATE_ACCELERATE
		kVehicle.accessEulerUpdate().setUpdateMode( EEulerUpdateMode_Accelerate );
		// no server data - client side steering force computation
		kSteeringForceUpdate.setEnabled( true );
#endif
#if ET_STEERUPDATE_NO_NEWDATA_INTEGRATE // experimental code 3
		kVehicle.accessEulerUpdate().setUpdateMode( EEulerUpdateMode_IntegrateMotionState );
#endif
	}
	kVehicle.updateBase( currentTime, elapsedTime );
	kVehicle.accessEulerUpdate().setUpdateMode( ePrevMode );
	kSteeringForceUpdate.setEnabled( bSteeringForceUpdateEnabled );
}

//-------------------------------------------------------------------------
void ClientVehicleUpdate::updateUnknown( 
	class SimpleNetworkVehicle& kVehicle, const osScalar currentTime, const osScalar elapsedTime )
{
	kVehicle.updateBase( currentTime, elapsedTime );
}