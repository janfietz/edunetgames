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

#include "EduNetConnect/SimpleNetworkVehicleUpdate.h"
#include "EduNetConnect/SimpleNetworkVehicle.h"
#include "OpenSteerUT/AbstractVehicleMath.h"

using namespace OpenSteer;

//-----------------------------------------------------------------------------
void SimpleNetworkVehicleUpdate::collect3DTextAnnotation( std::ostringstream& kStream )
{
	if( this->vehicle().isRemoteObject() )
	{
		kStream << "UpdateMode: "
			<< ClientVehicleUpdate::getVehicleUpdateModeString( 
			this->m_kClientVehicleUpdate.getVehicleUpdateMode() )
			<< std::endl;
	}
	else
	{
		size_t uiPackageSize = 0;
		for( size_t i = 0; i < ESerializeDataType_Count; ++i)
		{
			if( SimpleNetworkVehicle::ms_bReplicationDataConfig[i] != 0 )
			{
				uiPackageSize += SimpleNetworkVehicle::ms_uiReplicationDataBytes[i];
			}
		}
		kStream << "Packagesize: "
			<< uiPackageSize
			<< " Bytes"
			<< std::endl;
	}
}

//-----------------------------------------------------------------------------
void SimpleNetworkVehicleUpdate::updateCustom( AbstractUpdated* pkParent, const osScalar currentTime, const osScalar elapsedTime )
{
	SimpleNetworkVehicle* pkNetworkVehicle = dynamic_cast<SimpleNetworkVehicle*>(pkParent);
	if( pkNetworkVehicle->isRemoteObject() )
	{
		this->m_kClientVehicleUpdate.updateCustom( pkParent, currentTime, elapsedTime );
	}
	else
	{
		this->m_kServerVehicleUpdate.updateCustom( pkParent, currentTime, elapsedTime );
	}
}

//-------------------------------------------------------------------------
void SimpleNetworkVehicleUpdate::update( const osScalar currentTime, const osScalar elapsedTime )
{

}


