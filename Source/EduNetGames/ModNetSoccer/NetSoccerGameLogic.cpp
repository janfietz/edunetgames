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
	if( this->isVehicleUpdate() )
	{
		osAbstractVehicle& kVehicle = this->vehicle();
		kVehicle.update( currentTime, elapsedTime );		
	}else
	{
		NetSoccerPlayer::Group teamA;
		NetSoccerPlayer::Group teamB;
		NetSoccerBall* pkBall(NULL);

		const osAVGroup& kVehicles = this->m_pkPlugin->allVehicles();
		{
			osAVCIterator kIter = kVehicles.begin();
			osAVCIterator kEnd = kVehicles.end();
			while( kIter != kEnd )
			{
				osAbstractVehicle* pkVehicle = (*kIter);
				// right now only one seeker
				NetSoccerPlayer* pkPlayer = dynamic_cast<NetSoccerPlayer*>( pkVehicle );
				if( NULL != pkPlayer )
				{
					if (pkPlayer->b_ImTeamA)
					{
						teamA.push_back( pkPlayer );
					} else
					{
						teamB.push_back( pkPlayer );
					}
				}else
				if( NULL == pkBall )
				{
					pkBall = dynamic_cast<NetSoccerBall*>( pkVehicle );
				}				
				++kIter;
			}
		}		

		if (NULL != pkBall)
		{
			pkBall->setBox(this->m_pkPlugin->m_bbox);
		}

		this->updateTeam(teamA, teamB, pkBall, currentTime, elapsedTime);	
		this->updateTeam(teamB, teamA, pkBall, currentTime, elapsedTime);
	}

		
}

//-----------------------------------------------------------------------------
void NetSoccerGameLogic::updateTeam( NetSoccerPlayer::Group& kPlayerGroup,
	NetSoccerPlayer::Group& kOpponentGroup, NetSoccerBall* pkBall,
	const float currentTime, const float elapsedTime)
{
	NetSoccerPlayer::Group& kAllGroup = this->m_pkPlugin->m_AllPlayers;	

	NetSoccerPlayer::Group::iterator kIter = kPlayerGroup.begin();
	const NetSoccerPlayer::Group::const_iterator kIterEnd = kPlayerGroup.end();
	while(kIterEnd != kIter)
	{
		NetSoccerPlayer* pkPlayer = (*kIter);
		pkPlayer->setPlayerGroupsAndBall( kOpponentGroup, kAllGroup, pkBall );		
		++kIter;
	}	
}

