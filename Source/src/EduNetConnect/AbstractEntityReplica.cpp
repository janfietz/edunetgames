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
AbstractEntityReplica::AbstractEntityReplica( OpenSteer::AbstractPlugin* pkHostPlugin, OpenSteer::EntityClassId classId, bool bIsRemoteObject  ):
m_pkHostPlugin(pkHostPlugin)
{
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
			OpenSteer::AbstractVehicleGroup kVG( pkHostPlugin->allVehicles() );
			kVG.addVehicle( pkVehicle );
		}
		OpenSteer::AbstractObstacle* pkObstacle = dynamic_cast<OpenSteer::AbstractObstacle*>( this->accessEntity() );
		if( NULL != pkObstacle )
		{
			pkHostPlugin->allObstacles().push_back( pkObstacle );
		}
		OpenSteer::AbstractPlayer* pkPlayer = dynamic_cast<OpenSteer::AbstractPlayer*>( this->accessEntity() );
		if( NULL != pkPlayer )
		{
			pkHostPlugin->allPlayers().push_back( pkPlayer );
		}
	}
};

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
	OpenSteer::AbstractVehicle* pkVehicle = dynamic_cast<OpenSteer::AbstractVehicle*>( this->accessEntity() );
	if( NULL != pkVehicle )
	{
		kVG.removeVehicle( pkVehicle );
	}

	OpenSteer::AbstractObstacle* pkObstacle = dynamic_cast<OpenSteer::AbstractObstacle*>( this->accessEntity() );
	if( NULL != pkObstacle )
	{
		ObstacleGroup::iterator kIter = std::find( m_pkHostPlugin->allObstacles().begin(), m_pkHostPlugin->allObstacles().end(), pkObstacle );
		if( kIter != m_pkHostPlugin->allObstacles().end() )
		{
			m_pkHostPlugin->allObstacles().erase( kIter );
		}
	}

	OpenSteer::AbstractPlayer* pkPlayer = dynamic_cast<OpenSteer::AbstractPlayer*>( this->accessEntity() );
	if( NULL != pkPlayer )
	{
		PlayerGroup::iterator kIter = std::find( m_pkHostPlugin->allPlayers().begin(), m_pkHostPlugin->allPlayers().end(), pkPlayer );
		if( kIter != m_pkHostPlugin->allPlayers().end() )
		{
			m_pkHostPlugin->allPlayers().erase( kIter );
		}
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

