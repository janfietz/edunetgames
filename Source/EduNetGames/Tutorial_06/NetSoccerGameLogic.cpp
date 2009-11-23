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
#include "NetSoccerGameLogic.h"
#include "NetSoccerPlugin.h"

//-----------------------------------------------------------------------------
NetSoccerGameLogic::NetSoccerGameLogic( osAbstractVehicle* pkVehicle):
	BaseClass(pkVehicle), m_pkPlugin(NULL)
{

}

//-----------------------------------------------------------------------------
NetSoccerGameLogic::~NetSoccerGameLogic()
{

}

//-----------------------------------------------------------------------------
void NetSoccerGameLogic::update( const float currentTime, const float elapsedTime )
{	
	NetSoccerPlayer::Group& teamA = this->m_pkPlugin->m_kTeamA;
	NetSoccerPlayer::Group& teamB = this->m_pkPlugin->m_kTeamB;
	this->updateTeam(teamA, teamB, currentTime, elapsedTime);	
	this->updateTeam(teamB, teamA, currentTime, elapsedTime);


	NetSoccerBall* pkBall = this->m_pkPlugin->m_Ball;
	pkBall->setBox(this->m_pkPlugin->m_bbox);
	pkBall->update ( currentTime, elapsedTime );
}

//-----------------------------------------------------------------------------
void NetSoccerGameLogic::updateTeam( NetSoccerPlayer::Group& kPlayerGroup,
	NetSoccerPlayer::Group& kOpponentGroup,
	const float currentTime, const float elapsedTime)
{
	NetSoccerPlayer::Group& kAllGroup = this->m_pkPlugin->m_AllPlayers;
	NetSoccerBall* pkBall = this->m_pkPlugin->m_Ball;

	NetSoccerPlayer::Group::iterator kIter = kPlayerGroup.begin();
	const NetSoccerPlayer::Group::const_iterator kIterEnd = kPlayerGroup.end();
	while(kIterEnd != kIter)
	{
		NetSoccerPlayer* pkPlayer = (*kIter);
		pkPlayer->setPlayerGroupsAndBall( kOpponentGroup, kAllGroup, pkBall );
		pkPlayer->update( currentTime, elapsedTime);
		++kIter;
	}	
}

