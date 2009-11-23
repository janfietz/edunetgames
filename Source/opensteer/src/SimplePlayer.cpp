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

#include "OpenSteer/SimplePlayer.h"

using namespace OpenSteer;

// entity player test
#if 0
{
	NetPedestrian kVehicle;
	{
		SimplePlayer kPlayer0;
		SimplePlayer kPlayer1;
		kPlayer0.play( &kVehicle );
		AbstractPlayer* pkCP0 = kVehicle.getPlayer();
		AbstractEntity* pkCE0 = kPlayer0.getControlledEntity();

		kPlayer1.play( &kVehicle );
		AbstractPlayer* pkCP1 = kVehicle.getPlayer();
		AbstractEntity* pkCE1 = kPlayer1.getControlledEntity();
		pkCE0 = kPlayer0.getControlledEntity();
	}
	bool bTest0 = true;
	bTest0 = false;
}
bool bTest1 = true;
bTest1 = false;
#endif

//-----------------------------------------------------------------------------
SimpleController::SimpleController()
{

}

//-----------------------------------------------------------------------------
SimpleController::~SimpleController()
{

}

//-----------------------------------------------------------------------------
SimplePlayer::SimplePlayer( bool bIsLocalPlayer ):
	m_pkController(NULL),m_bIsLocalPlayer( bIsLocalPlayer )
{

}

//-----------------------------------------------------------------------------
SimplePlayer::~SimplePlayer()
{

}

//-----------------------------------------------------------------------------
AbstractPlayer* SimplePlayer::accessLocalPlayer( void )
{
	static SimplePlayer kPlayer(true);
	if( NULL == kPlayer.getController() )
	{
		static SimpleController kController;
		kPlayer.setController( &kController );
	}
	return &kPlayer;
}

//-----------------------------------------------------------------------------
void SimplePlayer::update( const osScalar currentTime, const osScalar elapsedTime )
{
	AbstractController* pkController = this->accessController();
	if( NULL != pkController )
	{
		pkController->update( currentTime, elapsedTime );
	}
	BaseClass::update( currentTime, elapsedTime );
}
