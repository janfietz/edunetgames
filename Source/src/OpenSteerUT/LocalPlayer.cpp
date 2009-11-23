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

#include "LocalPlayer.h"

using namespace OpenSteer;

#if 0
AbstractPlayer* pkLocalPlayer = SimplePlayer::accessLocalPlayer();
AbstractEntity* pkControlledEntity = NULL;
{
	NetPedestrian kVehicle;
	pkLocalPlayer->play( &kVehicle );
	pkControlledEntity = pkLocalPlayer->getControlledEntity();
	pkControlledEntity = NULL;
}
pkControlledEntity = pkLocalPlayer->getControlledEntity();
pkControlledEntity = NULL;

#endif

Vec3 LocalPlayerController::ms_kOutput(Vec3::zero);

//-----------------------------------------------------------------------------
LocalPlayerController::LocalPlayerController()
{

}

//-----------------------------------------------------------------------------
LocalPlayerController::~LocalPlayerController()
{

}

//-----------------------------------------------------------------------------
AbstractController* LocalPlayerController::accessLocalPlayerController( void )
{
	static LocalPlayerController kLocalPlayerController;
	return &kLocalPlayerController;
}

//-----------------------------------------------------------------------------
bool LocalPlayerController::keyboardFunc( unsigned char key, int x, int y )
{
	// references
	float& forwardComponent = LocalPlayerController::ms_kOutput.z;
	float& sideComponent = LocalPlayerController::ms_kOutput.x;
	bool bHasAxis = false;
	switch (key)
	{
		// reset selected Plugin
	case 'w':
		forwardComponent = 1.0f;
		bHasAxis = true;
		break;
	case 'a':
		sideComponent = 1.0f;
		bHasAxis = true;
		break;
	case 's':
		forwardComponent = -1.0f;
		bHasAxis = true;
		break;
	case 'd':
		sideComponent = -1.0f;
		bHasAxis = true;
		break;
	}
	if( true == bHasAxis )
	{
		LocalPlayerController::ms_kOutput.normalize();
	}
	return bHasAxis;
}

//-----------------------------------------------------------------------------
bool LocalPlayerController::keyboardFuncUp( unsigned char key, int x, int y )
{
	// references
	float& forwardComponent = LocalPlayerController::ms_kOutput.z;
	float& sideComponent = LocalPlayerController::ms_kOutput.x;

	bool bHasAxis = false;
	switch (key)
	{
	case 'w':
	case 's':
		forwardComponent = 0.0f;
		bHasAxis = true;
		break;
	case 'd':
	case 'a':
		sideComponent = 0.0f;
		bHasAxis = true;
		break;
	}
	if( true == bHasAxis )
	{
		LocalPlayerController::ms_kOutput.normalize();
	}
	return bHasAxis;
}

//-----------------------------------------------------------------------------
void LocalPlayerController::updateCustom( 
	AbstractUpdated* pkParent, const osScalar currentTime, const osScalar elapsedTime )
{
	AbstractController* pkTargetController = dynamic_cast<AbstractController*>(pkParent);
	if( NULL != pkTargetController )
	{
		// pass the control force from here to the actual 'in game' controller
		pkTargetController->setOutputForce( this->getOutputForce() );
	}
}
