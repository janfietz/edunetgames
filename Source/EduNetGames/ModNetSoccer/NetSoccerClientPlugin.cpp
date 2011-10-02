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
#include "NetSoccerClientPlugin.h"
#include "NetSoccerEntityFactory.h"

#include "OpenSteerUT/LocalPlayer.h"

#include "EduNetConnect/AbstractEntityReplica.h"

//-----------------------------------------------------------------------------
NetSoccerEntityFactory gCLientNetSoccerEntityFactory;

//-------------------------------------------------------------------------
void SoccerClientPlugin::CreateContent( void )
{
	this->setGamePluginReplicaManager( this->m_kReplicaManager );
	this->m_kReplicaManager->setPlugin( &this->m_kGamePlugin );
	this->m_kGamePlugin.setEntityFactory( NULL );

	this->m_pNetInterface->AttachPlugin( this->m_kReplicaManager );

	this->m_pkClientFactory = ET_NEW AbstractEntityCCReplicaFactory( this->m_kReplicaManager );
	
	AbstractEntity* pkNetworkPlayerEntity = this->m_pkClientFactory->createEntity( OS_CID_CLIENT_PLAYER, this );
	OpenSteer::AbstractPlayer* pkPlayer = dynamic_cast<OpenSteer::AbstractPlayer*>( pkNetworkPlayerEntity );
	if( NULL != pkPlayer )
	{
		this->addPlayer( pkPlayer );
	}

	BaseClass::CreateContent();
	
	
}

//-------------------------------------------------------------------------
void SoccerClientPlugin::DeleteContent( void )
{	
	BaseClass::DeleteContent();
	if (NULL != this->m_pkClientFactory)
	{
		this->removePlayer( this->m_pkClientPlayer );
		this->m_pkClientFactory->destroyEntity( this->m_pkClientPlayer );	
	}	
	ET_SAFE_DELETE( this->m_pkClientFactory );
	
	if (NULL != this->m_kReplicaManager)
	{
		this->m_pNetInterface->DetachPlugin( this->m_kReplicaManager );
	}	
	this->setGamePluginReplicaManager(NULL);
	ET_SAFE_DELETE(this->m_kReplicaManager);

	
}
//-------------------------------------------------------------------------
void SoccerClientPlugin::addPlayer (OpenSteer::AbstractPlayer* pkPlayer)
{
	BaseClass::addPlayer(pkPlayer);
	this->m_pkClientPlayer = pkPlayer;
	osAbstractController* pkController = pkPlayer->accessController();
	if( NULL != pkController )
	{
		pkController->setCustomUpdated( OpenSteer::LocalPlayerController::accessLocalPlayerController() );
	}
}

//-------------------------------------------------------------------------
void SoccerClientPlugin::removePlayer (OpenSteer::AbstractPlayer* pkPlayer)
{
	this->m_pkClientPlayer = NULL;
	BaseClass::removePlayer(pkPlayer);
}

//-------------------------------------------------------------------------
void SoccerClientPlugin::update (const float currentTime, const float elapsedTime)
{
	if( NULL != this->m_pkClientPlayer )
	{
		OpenSteer::CastToAbstractUpdated( m_pkClientPlayer )->update( currentTime, elapsedTime );
	}
	BaseClass::update( currentTime,  elapsedTime );
};

//-------------------------------------------------------------------------
OpenSteer::AbstractEntityFactory* SoccerClientPlugin::getGamePluginEntityFactory( void ) const
{
	return &gCLientNetSoccerEntityFactory;
}
