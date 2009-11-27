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
#include "NetworkPlugin.h"

#include "OpenSteerUT/AbstractEntityFactory.h"
#include "OpenSteerUT/AbstractVehicleGroup.h"

using namespace OpenSteer;

//-----------------------------------------------------------------------------
AbstractEntityReplica::AbstractEntityReplica():m_pkHostPlugin(NULL)
{

}

//-----------------------------------------------------------------------------
AbstractEntityReplica::AbstractEntityReplica( 
	OpenSteer::AbstractPlugin* pkHostPlugin, 
	OpenSteer::EntityClassId classId, bool bIsRemoteObject, bool bClientReplica ):
	OSReplica<OpenSteer::AbstractEntity>( bClientReplica ),
	m_pkHostPlugin(pkHostPlugin)
{
	if( classId == OS_CID_PLAYER )
	{
		bool bTest = true;
		bTest = false;
		printf( "creating player replica %s\n", this->isClientReplica() ? "CLIENT" : "PEER" );
	}
	if( true == bClientReplica )
	{
		bool bTest = true;
		bTest = false;
		printf( "creating client replica\n" );
	}
	this->m_classId = classId;
	// now retrieve the original game entity factory
	OpenSteer::AbstractPlugin* pkParentPlugin = pkHostPlugin->getParentPlugin();
	AbstractNetworkPlugin* pkNetworkPlugin = dynamic_cast<AbstractNetworkPlugin*>( pkParentPlugin );
	OpenSteer::AbstractEntityFactory* pkEntityFactory = pkNetworkPlugin->getGamePluginEntityFactory();
	assert( NULL != pkEntityFactory );
	this->setEntity( pkEntityFactory->createEntity( classId ) );

	assert( NULL != this->getEntity() );
	this->accessEntity()->setIsRemoteObject( bIsRemoteObject );
	this->m_kClassName = this->accessEntity()->getClassName();

	// remote objects have to be added to the client game plugin
	if( true == bIsRemoteObject )
	{
		OpenSteer::AbstractVehicle* pkVehicle = dynamic_cast<OpenSteer::AbstractVehicle*>( this->accessEntity() );
		if( NULL != pkVehicle )
		{
			pkHostPlugin->addVehicle( pkVehicle );
		}
		OpenSteer::AbstractObstacle* pkObstacle = dynamic_cast<OpenSteer::AbstractObstacle*>( this->accessEntity() );
		if( NULL != pkObstacle )
		{
			pkHostPlugin->addObstacle( pkObstacle );
		}
		OpenSteer::AbstractPlayer* pkPlayer = dynamic_cast<OpenSteer::AbstractPlayer*>( this->accessEntity() );
		if( NULL != pkPlayer )
		{
			pkHostPlugin->addPlayer( pkPlayer );
		}
	}
};

//-----------------------------------------------------------------------------
AbstractEntityReplica::~AbstractEntityReplica()
{
	if( this->m_classId == OS_CID_PLAYER )
	{
		printf( "destroy player replica %s\n", this->isClientReplica() ? "CLIENT" : "PEER" );
		bool bTest = true;
		bTest = false;
	}
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
	OpenSteer::AbstractVehicle* pkVehicle = dynamic_cast<OpenSteer::AbstractVehicle*>( this->accessEntity() );
	if( NULL != pkVehicle )
	{
		m_pkHostPlugin->removeVehicle( pkVehicle );	
	}

	OpenSteer::AbstractObstacle* pkObstacle = dynamic_cast<OpenSteer::AbstractObstacle*>( this->accessEntity() );
	if( NULL != pkObstacle )
	{
		m_pkHostPlugin->removeObstacle( pkObstacle );		
	}

	OpenSteer::AbstractPlayer* pkPlayer = dynamic_cast<OpenSteer::AbstractPlayer*>( this->accessEntity() );
	if( NULL != pkPlayer )
	{		
		m_pkHostPlugin->removePlayer( pkPlayer );		
	}

	this->releaseEntity();
	ET_DELETE this;
}

//-----------------------------------------------------------------------------
RakNet::RM3SerializationResult AbstractEntityReplica::Serialize(RakNet::SerializeParameters *serializeParameters)
{
	OpenSteer::NetworkEntitySerializer kSerializer( this->accessEntity() );
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
	OpenSteer::NetworkEntitySerializer kSerializer( this->accessEntity() );
	kSerializer.deserialize( deserializeParameters );
}

//-----------------------------------------------------------------------------
void AbstractEntityReplica::SerializeConstruction(RakNet::BitStream *constructionBitstream,
	RakNet::Connection_RM3 *destinationConnection)
{
	OpenSteer::NetworkEntitySerializer kSerializer( this->accessEntity() );
	kSerializer.serializeConstruction( constructionBitstream );
}

//-----------------------------------------------------------------------------
bool AbstractEntityReplica::DeserializeConstruction(RakNet::BitStream *constructionBitstream,
	RakNet::Connection_RM3 *sourceConnection)
{
	OpenSteer::NetworkEntitySerializer kSerializer( this->accessEntity() );
	return kSerializer.deserializeConstruction( constructionBitstream );
}
