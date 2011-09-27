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

#include "PeerZonePlugin.h"
#include "OpenSteerUT/OpenSteerUT.h"
#include "OpenSteerUT/ZonePlugin.h"

//-----------------------------------------------------------------------------
PeerZonePlugin::PeerZonePlugin( bool bAddToRegistry ):BaseClass( bAddToRegistry )
{

}

//-----------------------------------------------------------------------------
void PeerZonePlugin::setZoneId( size_t zoneId )
{
	this->m_kGamePlugin.setZoneId( zoneId );
}

//-----------------------------------------------------------------------------
void PeerZonePlugin::initializeReplication()
{	
	this->m_pkReplicaManager = ET_NEW AbstractEntityReplicaManager();
	this->setGamePluginReplicaManager( this->m_pkReplicaManager );
	this->m_pkReplicaManager->setPlugin( &this->m_kGamePlugin );
	
	// attach vehicle factory
	this->m_pkNetPedestrianFactory = ET_NEW AbstractEntityReplicaFactory( this->m_pkReplicaManager );	
	this->m_kGamePlugin.setEntityFactory( this->m_pkNetPedestrianFactory );
}
//-----------------------------------------------------------------------------
void PeerZonePlugin::shutdownReplication()
{	
	if (NULL != this->m_pNetInterface)
	{
		this->m_pNetInterface->DetachPlugin( this->m_pkReplicaManager );
		this->setGamePluginReplicaManager( NULL );
		ET_SAFE_DELETE(this->m_pkReplicaManager);

		// attach vehicle factory
		this->m_kGamePlugin.setEntityFactory( NULL );
		ET_SAFE_DELETE(this->m_pkNetPedestrianFactory);
	}
}
//-----------------------------------------------------------------------------
const char* PeerZonePlugin::name() const
{
	if( this->m_kGamePlugin.getZoneId() < 4 )
	{
		if(this->m_kGamePlugin.createsContentZone(0))
			return "PeerZone-0";
		if(this->m_kGamePlugin.createsContentZone(1))
			return "PeerZone-1";
		if(this->m_kGamePlugin.createsContentZone(2))
			return "PeerZone-2";
		if(this->m_kGamePlugin.createsContentZone(3))
			return "PeerZone-3";
	}
	if( this->m_kGamePlugin.getZoneId() < 5 )
	{
		return "PeerZones";
	}
	else
	{
		return "PeerEmptyZones";
	}
}

//-----------------------------------------------------------------------------
void PeerZonePlugin::prepareOpen( void ) 
{ 
	BaseClass::prepareOpen();	
	this->m_kGamePlugin.prepareOpen();
}
//-----------------------------------------------------------------------------
void PeerZonePlugin::CreateContent( void )
{	
	this->initializeReplication();
	this->m_pNetInterface->AttachPlugin( this->m_pkReplicaManager );
	BaseClass::CreateContent();
}
//-----------------------------------------------------------------------------
void PeerZonePlugin::DeleteContent( void )
{
	BaseClass::DeleteContent();
	this->shutdownReplication();
}
//-----------------------------------------------------------------------------
AbstractEntityReplica* PeerZonePlugin::allocEntityReplica(
	OpenSteer::AbstractPlugin* pPlugin,
	OpenSteer::EntityClassId classId,
	bool bIsRemoteObject,
	bool bClientReplica ) const
{
	bool btest;
	btest = true;
	return BaseClass::allocEntityReplica(pPlugin,
		classId,
		bIsRemoteObject, bClientReplica);
}
//-----------------------------------------------------------------------------
OpenSteer::AbstractEntityFactory* PeerZonePlugin::getGamePluginEntityFactory(
	void ) const
{
	return &this->m_kOfflinePedestrianFactory;
}