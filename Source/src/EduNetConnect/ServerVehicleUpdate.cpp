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

#include "EduNetConnect/ServerVehicleUpdate.h"

#include "EduNetConnect/SimpleNetworkVehicle.h"
#include "OpenSteerUT/AbstractVehicleMath.h"

using namespace OpenSteer;

//-----------------------------------------------------------------------------
void ServerVehicleUpdate::updateCustom( AbstractUpdated* pkParent, const osScalar currentTime, const osScalar elapsedTime )
{
	SimpleNetworkVehicle* pkNetworkVehicle = dynamic_cast<SimpleNetworkVehicle*>(pkParent);
	SimpleProxyVehicle& kProxy = pkNetworkVehicle->accessProxyVehicle();
	if( true == kProxy.m_bHasNewData )
	{
		this->m_eLastServerVehicleMode = this->determineServerVehicleMode( *pkNetworkVehicle );
		// add the new data to the history
		kProxy.m_kLocalSpaceData.addValue( kProxy.getLocalSpaceData() )._steeringForce = 
			kProxy.getSteeringForceUpdate().getForce();
	}
	else
	{

	}
	switch( this->m_eLastServerVehicleMode )
	{
	case( EServerVehicleMode_ExtrapolateProxy ):
		this->updateExtrapolateProxy( *pkNetworkVehicle, currentTime, elapsedTime );
		break;
	}
	pkNetworkVehicle->updateBase( currentTime, elapsedTime );
}

//-------------------------------------------------------------------------
void ServerVehicleUpdate::update( const osScalar currentTime, const osScalar elapsedTime )
{

}

//-------------------------------------------------------------------------
EServerVehicleMode ServerVehicleUpdate::determineServerVehicleMode( const class SimpleNetworkVehicle& kVehicle ) const
{
	EServerVehicleMode eType = EServerVehicleMode_Unknown;
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
		( kProxy.m_bSerializedDataTypes[ ESerializeDataType_AngularVelocity ] ) &&
		( kProxy.m_bSerializedDataTypes[ ESerializeDataType_LinearVelocity ] );
	const bool bHasSpeedUpdate = 
		( kProxy.m_bSerializedDataTypes[ ESerializeDataType_Speed ] ) ;

	if( bHasPositionUpdate && bHasOrientationUpdate && bHasVelocityUpdate )
	{
		eType = EServerVehicleMode_ExtrapolateProxy;
	}
	return eType;
}

//-------------------------------------------------------------------------
void ServerVehicleUpdate::updateExtrapolateProxy( class SimpleNetworkVehicle& kVehicle, const osScalar currentTime, const osScalar elapsedTime )
{
//	printf("updateExtrapolateProxy\n");
}
