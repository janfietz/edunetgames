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

#include "AbstractVehicleReplica.h"
#include "OpenSteerUT/AbstractVehicleFactory.h"

#include "OpenSteerUT/AbstractVehicleGroup.h"
#include "OpenSteerUT/VehicleClassIds.h"

using namespace OpenSteer;

OpenSteer::AbstractVehicleFactory* AbstractVehicleReplica::ms_pkFactory = NULL;

//-----------------------------------------------------------------------------
AbstractVehicleReplica::AbstractVehicleReplica():m_pkHostPlugin(NULL)
{

}

//-----------------------------------------------------------------------------
AbstractVehicleReplica::AbstractVehicleReplica( OpenSteer::AbstractPlugin* pkHostPlugin, OpenSteer::EntityClassId classId, bool bIsRemoteObject  ):
m_pkHostPlugin(pkHostPlugin)
{
	assert( NULL != AbstractVehicleReplica::ms_pkFactory );
	this->m_pVehicle = AbstractVehicleReplica::ms_pkFactory->createVehicle( classId, this->m_pkHostPlugin->accessProximityDataBase() );
	assert( NULL != this->m_pVehicle );
	this->m_pVehicle->setIsRemoteObject( bIsRemoteObject );
	this->m_kClassName = this->m_pVehicle->getClassName();
};

//-----------------------------------------------------------------------------
void AbstractVehicleReplica::setAbstractVehicleFactory( OpenSteer::AbstractVehicleFactory* pkFactory )
{
	AbstractVehicleReplica::ms_pkFactory = pkFactory;
}

//-----------------------------------------------------------------------------
void AbstractVehicleReplica::SetNetworkID( NetworkID id )
{
	BaseClass::SetNetworkID( id );
	OpenSteer::AbstractVehicle* pkVehicle = this->AccessVehicle();
	pkVehicle->setNetworkId( id.guid.g );
}

//-----------------------------------------------------------------------------
RakNet::RakString AbstractVehicleReplica::GetName(void) const
{
	if( NULL != this->m_pVehicle )
	{
		return this->m_pVehicle->getClassName();
	}
	return this->m_kClassName;
}

//-----------------------------------------------------------------------------
void AbstractVehicleReplica::DeallocReplica(RakNet::Connection_RM3 *sourceConnection)
{
	AbstractVehicleGroup kVG( m_pkHostPlugin->allVehicles() );
	kVG.removeVehicle( this->m_pVehicle );
	ET_SAFE_DELETE( this->m_pVehicle );
	delete this;
}

//-----------------------------------------------------------------------------
RakNet::RM3SerializationResult AbstractVehicleReplica::Serialize(RakNet::SerializeParameters *serializeParameters)
{
	RakNet::BitStream& kStream = serializeParameters->outputBitstream[0];

	kStream.WriteAlignedBytes((const unsigned char*)&this->m_pVehicle->position(),sizeof(OpenSteer::Vec3));
	kStream.WriteAlignedBytes((const unsigned char*)&this->m_pVehicle->forward(),sizeof(OpenSteer::Vec3));
	kStream.WriteAlignedBytes((const unsigned char*)&this->m_pVehicle->lastSteeringForce(),sizeof(OpenSteer::Vec3));

	return RakNet::RM3SR_BROADCAST_IDENTICALLY;
}

//-----------------------------------------------------------------------------
void AbstractVehicleReplica::Deserialize(RakNet::DeserializeParameters *deserializeParameters)
{
	RakNet::BitStream& kStream = deserializeParameters->serializationBitstream[0];

	OpenSteer::Vec3 kVec;
	kStream.ReadAlignedBytes((unsigned char*)&kVec,sizeof(kVec));
	this->m_pVehicle->setPosition(kVec);

	kStream.ReadAlignedBytes((unsigned char*)&kVec,sizeof(kVec));
	this->m_pVehicle->setForward(kVec);	
	this->m_pVehicle->regenerateOrthonormalBasisUF( kVec );			

	kStream.ReadAlignedBytes((unsigned char*)&kVec,sizeof(kVec));
	this->m_pVehicle->setLastSteeringForce(kVec);	

}

