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
#include "NetSoccerPeerPlugin.h"

//-----------------------------------------------------------------------------
void SoccerPeerPlugin::StartNetworkSession( void )
{
	BaseClass::StartNetworkSession();
	this->m_kReplicaManager = ET_NEW AbstractEntityReplicaManager();
}
//-----------------------------------------------------------------------------
void SoccerPeerPlugin::StopNetworkSession( void )
{
	BaseClass::StopNetworkSession();	
}
//-----------------------------------------------------------------------------
void SoccerPeerPlugin::CreateContent( void )
{
	this->setGamePluginReplicaManager( this->m_kReplicaManager );
	this->m_kReplicaManager->setPlugin( &this->m_kGamePlugin );

	// remap the entity factory
	this->m_pkNetBoidFactory = ET_NEW AbstractEntityReplicaFactory( this->m_kReplicaManager );
	this->m_kGamePlugin.setEntityFactory( this->m_pkNetBoidFactory );

	this->m_pNetInterface->AttachPlugin( this->m_kReplicaManager );

	BaseClass::CreateContent();
}

//-----------------------------------------------------------------------------
void SoccerPeerPlugin::DeleteContent( void )
{
	BaseClass::DeleteContent();

	this->m_kGamePlugin.setEntityFactory( NULL );
	ET_SAFE_DELETE(this->m_pkNetBoidFactory);

	if (NULL != this->m_kReplicaManager)
	{
		this->m_pNetInterface->DetachPlugin( this->m_kReplicaManager );
	}	
	ET_SAFE_DELETE(this->m_kReplicaManager);
}

//-----------------------------------------------------------------------------
AbstractEntityReplica* SoccerPeerPlugin::allocEntityReplica(  
	OpenSteer::AbstractPlugin* pPlugin, 
	OpenSteer::EntityClassId classId, 
	bool bIsRemoteObject,  bool bClientReplica ) const
{
	if (OS_CID_CLIENT_PLAYER == classId)
	{
		return ET_NEW AbstractEntityCCReplica(pPlugin,
			classId,bIsRemoteObject, bClientReplica);
	}
	return BaseClass::allocEntityReplica( pPlugin, classId,
		bIsRemoteObject, bClientReplica);
}