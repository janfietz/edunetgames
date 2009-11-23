#ifndef __NETSOCCER_GAMELOGIC_H__
#define __NETSOCCER_GAMELOGIC_H__
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
#include "EduNetCommon/EduNetCommon.h"
#include "OpenSteerUT/AbstractVehicleUpdate.h"
#include "NetSoccerPlayer.h"

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
class NetSoccerGameLogic : public OpenSteer::AbstractVehicleUpdate
{
	ET_DECLARE_BASE( OpenSteer::AbstractVehicleUpdate );
public:
	NetSoccerGameLogic( osAbstractVehicle* pkVehicle = NULL);
	virtual ~NetSoccerGameLogic();

	OS_IMPLEMENT_CLASSNAME( NetSoccerGameLogic )

	//-------------------------------------------------------------------------
	// interface AbstractUpdated
	//-------------------------------------------------------------------
	// interface AbstractUpdated
	virtual void updateCustom( AbstractUpdated* /*pkParent*/, const osScalar /*currentTime*/, const osScalar /*elapsedTime*/ )
	{
		// nothing to do here
		return;
	}
	virtual void update (const float currentTime, const float elapsedTime);

	//-------------------------------------------------------------------------
	void setPlugin( class NetSoccerPlugin* pkPlugin )
	{
		this->m_pkPlugin = pkPlugin;
	}	
	
protected:
	class NetSoccerPlugin* m_pkPlugin;

private:
	void updateTeam( NetSoccerPlayer::Group& kPlayerGroup,
		NetSoccerPlayer::Group& kOpponentGroup,
		const float currentTime, const float elapsedTime);
};

#endif // __NETSOCCER_GAMELOGIC_H__
