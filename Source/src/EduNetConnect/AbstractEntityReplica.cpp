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

#include "AbstractEntityReplica.h"
#include "NetworkVehicle.h"

#include "OpenSteerUT/AbstractEntityFactory.h"
#include "OpenSteerUT/AbstractVehicleGroup.h"
#include "OpenSteer/Plugin.h"

using namespace OpenSteer;

OpenSteer::AbstractEntityFactory* AbstractEntityReplica::ms_pkFactory = NULL;

//-----------------------------------------------------------------------------
AbstractEntityReplica::AbstractEntityReplica():m_pkHostPlugin(NULL)
{

}

//-----------------------------------------------------------------------------
AbstractEntityReplica::AbstractEntityReplica( OpenSteer::AbstractPlugin* pkHostPlugin, OpenSteer::EntityClassId classId, bool bIsRemoteObject  ):
m_pkHostPlugin(pkHostPlugin)
{
	assert( NULL != AbstractEntityReplica::ms_pkFactory );
	this->setEntity( AbstractEntityReplica::ms_pkFactory->createVehicle( classId ) );
	assert( NULL != this->getEntity() );
	this->accessEntity()->setIsRemoteObject( bIsRemoteObject );
	this->m_kClassName = this->accessEntity()->getClassName();
};

//-----------------------------------------------------------------------------
void AbstractEntityReplica::setAbstractEntityFactory( OpenSteer::AbstractEntityFactory* pkFactory )
{
	AbstractEntityReplica::ms_pkFactory = pkFactory;
}

//-----------------------------------------------------------------------------
RakNet::RakString AbstractEntityReplica::GetName(void) const
{
	if( NULL != this->accessEntity() )
	{
		return this->accessEntity()->getClassName();
	}
	return this->m_kClassName;
}

//-----------------------------------------------------------------------------
void AbstractEntityReplica::DeallocReplica(RakNet::Connection_RM3 *sourceConnection)
{
	AbstractVehicleGroup kVG( m_pkHostPlugin->allVehicles() );
	kVG.removeVehicle( this->accessEntity() );
	this->releaseEntity();
	ET_DELETE this;
}

//-----------------------------------------------------------------------------
RakNet::RM3SerializationResult AbstractEntityReplica::Serialize(RakNet::SerializeParameters *serializeParameters)
{
	OpenSteer::NetworkVehicleSerializer kSerializer( this->accessEntity() );
	int nResult = kSerializer.serialize( serializeParameters );
	if( nResult >= 0 )
	{
		return static_cast<RakNet::RM3SerializationResult>(nResult);
	}
	return RakNet::RM3SR_DO_NOT_SERIALIZE;
}

//-----------------------------------------------------------------------------
void AbstractEntityReplica::Deserialize(RakNet::DeserializeParameters *deserializeParameters)
{
	OpenSteer::NetworkVehicleSerializer kSerializer( this->accessEntity() );
	kSerializer.deserialize( deserializeParameters );
}

