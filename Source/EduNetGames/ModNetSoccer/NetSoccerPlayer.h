#ifndef __NETSOCCERPLAYER_H__
#define __NETSOCCERPLAYER_H__
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

//-----------------------------------------------------------------------------
#include "OpenSteerUT/AbstractVehicleUpdate.h"
#include "OpenSteerUT/AbstractVehicleUtilities.h"
#include "OpenSteerUT/VehicleClassIds.h"
#include "EduNetConnect/SimpleNetworkVehicle.h"
#include "NetSoccerBall.h"

//-----------------------------------------------------------------------------
class NetSoccerPlayer : public OpenSteer::SimpleNetworkVehicle
{
    ET_DECLARE_BASE ( OpenSteer::SimpleNetworkVehicle )
public:
    typedef  std::vector<NetSoccerPlayer*> Group;
    // constructor
    NetSoccerPlayer();   

    OS_IMPLEMENT_CLASSNAME ( NetSoccerPlayer )
    // reset state
    void reset ( void );

    // per frame simulation update
    // (parameter names commented out to prevent compiler warning from "-W")
	virtual osVector3 determineCombinedSteering( const float elapsedTime );

	virtual void update( const float currentTime, const float elapsedTime );
    // draw this character/vehicle into the scene
	virtual void draw( OpenSteer::AbstractRenderer*, 
		const float currentTime, const float elapsedTime ) OS_OVERRIDE;

	virtual void possessBy( OpenSteer::AbstractEntity* pkEntity );

	virtual int serialize( RakNet::SerializeParameters *serializeParameters ) const;
	virtual void deserialize( RakNet::DeserializeParameters *deserializeParameters );
	virtual void serializeConstruction(RakNet::BitStream *constructionBitstream);
	virtual bool deserializeConstruction(RakNet::BitStream *constructionBitstream );

	void setTeamIdAndPlayerNumber( bool isTeamA,  unsigned int id);
	void setPlayerGroupsAndBall(Group& kOpponentGroup, Group& kAllGroup, NetSoccerBall* pkBall)
	{
		this->m_others = &kOpponentGroup;
		this->m_AllPlayers = &kAllGroup;
		this->m_Ball = pkBall;
	}	
	void setHomeAndPosition(  const OpenSteer::Vec3& kPos );
    // per-instance reference to its group
    Group*  m_others;
    Group*  m_AllPlayers;
    NetSoccerBall*       m_Ball;
    bool b_ImTeamA;
    unsigned int m_MyID;
    OpenSteer::Vec3 m_home;
private:

	bool isKicking( void ) const;

	bool m_bDrawAnnotation;
	osVector3 m_behindBall;
};
typedef OpenSteer::VehicleClassIdMixin<NetSoccerPlayer, ET_CID_NETSOCCER_PLAYER> TNetSoccerPlayer;
#endif // __NETSOCCERPLAYER_H__
