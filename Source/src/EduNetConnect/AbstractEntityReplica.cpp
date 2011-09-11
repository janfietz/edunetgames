//-----------------------------------------------------------------------------
// Copyright (c) 2011, Jan Fietz, Cyrus Preuss
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
#include "NetworkEntity.h"
#include "NetworkVehicle.h"
#include "NetworkPlugin.h"

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
#if 0
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
#endif
	this->m_classId = classId;
	// now retrieve the original game entity factory
	OpenSteer::AbstractPlugin* pkParentPlugin = pkHostPlugin->getParentPlugin();
	AbstractNetworkPlugin* pkNetworkPlugin = dynamic_cast<AbstractNetworkPlugin*>( pkParentPlugin );
	if(NULL == pkNetworkPlugin)
	{
		pkParentPlugin = pkHostPlugin;
	}
	pkNetworkPlugin = dynamic_cast<AbstractNetworkPlugin*>( pkParentPlugin );
	OpenSteer::AbstractEntityFactory* pkEntityFactory = pkNetworkPlugin->getGamePluginEntityFactory();

	assert( NULL != pkEntityFactory );
	this->setEntity( pkEntityFactory->createEntity( classId ) );

	assert( NULL != this->getEntity() );
	this->accessEntity()->setIsRemoteObject( bIsRemoteObject );
	this->m_kClassName = this->accessEntity()->getClassName();

	// remote objects have to be added to the client game plugin
	// now done post serialize construction
	// this->addEntityToHostPlugin();
}

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
	bool bDeserialized = kSerializer.deserializeConstruction( constructionBitstream );
	return bDeserialized;
}
//-----------------------------------------------------------------------------
void AbstractEntityReplica::PostDeserializeConstruction( 
	RakNet::Connection_RM3 *sourceConnection )
{
	this->addEntityToHostPlugin();
}

//-----------------------------------------------------------------------------
void AbstractEntityReplica::addEntityToHostPlugin( void )
{
	assert( NULL != this->getEntity() );
	assert( NULL != this->m_pkHostPlugin );

	// remote objects have to be added to the client game plugin
	if( true == this->getEntity()->isRemoteObject() )
	{
		OpenSteer::AbstractVehicle* pkVehicle = dynamic_cast<OpenSteer::AbstractVehicle*>( this->accessEntity() );
		if( NULL != pkVehicle )
		{
			this->m_pkHostPlugin->addVehicle( pkVehicle );
		}
		OpenSteer::AbstractObstacle* pkObstacle = dynamic_cast<OpenSteer::AbstractObstacle*>( this->accessEntity() );
		if( NULL != pkObstacle )
		{
			this->m_pkHostPlugin->addObstacle( pkObstacle );
		}
		OpenSteer::AbstractPlayer* pkPlayer = dynamic_cast<OpenSteer::AbstractPlayer*>( this->accessEntity() );
		if( NULL != pkPlayer )
		{
			this->m_pkHostPlugin->addPlayer( pkPlayer );
		}
	}
}

//-----------------------------------------------------------------------------
AbstractEntityCCReplica::AbstractEntityCCReplica()
{

}

//-----------------------------------------------------------------------------
AbstractEntityCCReplica::AbstractEntityCCReplica(
	OpenSteer::AbstractPlugin* pPlugin,
	OpenSteer::EntityClassId classId,
	bool bIsRemoteObject,
	bool bClientReplica) : BaseClass(pPlugin, classId, bIsRemoteObject, bClientReplica)
{

}

//-----------------------------------------------------------------------------
RakNet::RM3ConstructionState AbstractEntityCCReplica::QueryConstruction(
	RakNet::Connection_RM3 *destinationConnection,
	RakNet::ReplicaManager3 *replicaManager3)
{
	return QueryConstruction_ClientConstruction( destinationConnection );
}
//-----------------------------------------------------------------------------
bool AbstractEntityCCReplica::QueryRemoteConstruction(
	RakNet::Connection_RM3 *sourceConnection)
{
	return QueryRemoteConstruction_ClientConstruction( sourceConnection );
}
//-----------------------------------------------------------------------------
RakNet::RM3QuerySerializationResult AbstractEntityCCReplica::QuerySerialization(
	RakNet::Connection_RM3 *destinationConnection)
{
	return QuerySerialization_ClientSerializable( destinationConnection );
}
//-----------------------------------------------------------------------------
RakNet::RM3ActionOnPopConnection AbstractEntityCCReplica::QueryActionOnPopConnection(
	RakNet::Connection_RM3 *droppedConnection) const
{
	return QueryActionOnPopConnection_Client(droppedConnection);
}

//-----------------------------------------------------------------------------
AbstractEntitySSReplica::AbstractEntitySSReplica()
{

}
//-----------------------------------------------------------------------------
AbstractEntitySSReplica::AbstractEntitySSReplica(
	OpenSteer::AbstractPlugin* pPlugin,
	OpenSteer::EntityClassId classId,
	bool bIsRemoteObject,
	bool bClientReplica) : BaseClass(pPlugin, classId, bIsRemoteObject, bClientReplica)
{

}

//-----------------------------------------------------------------------------
RakNet::RM3ConstructionState AbstractEntitySSReplica::QueryConstruction(
	RakNet::Connection_RM3 *destinationConnection,
	RakNet::ReplicaManager3 *replicaManager3)
{
	return QueryConstruction_ServerConstruction( destinationConnection );
}
//-----------------------------------------------------------------------------
bool AbstractEntitySSReplica::QueryRemoteConstruction(
	RakNet::Connection_RM3 *sourceConnection)
{
	return QueryRemoteConstruction_ServerConstruction( sourceConnection );
}
//-----------------------------------------------------------------------------
RakNet::RM3QuerySerializationResult AbstractEntitySSReplica::QuerySerialization(
	RakNet::Connection_RM3 *destinationConnection)
{
	return QuerySerialization_ServerSerializable( destinationConnection );
}
//-----------------------------------------------------------------------------
RakNet::RM3ActionOnPopConnection AbstractEntitySSReplica::QueryActionOnPopConnection(
	RakNet::Connection_RM3 *droppedConnection) const
{
	return QueryActionOnPopConnection_Server(droppedConnection);
}
