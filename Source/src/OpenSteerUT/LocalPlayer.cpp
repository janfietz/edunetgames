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

#include "LocalPlayer.h"
#include "OpenSteerUT.h"

using namespace OpenSteer;

#if 0
AbstractPlayer* pkLocalPlayer = SimplePlayer::accessLocalPlayer();
AbstractEntity* pkControlledEntity = NULL;
{
	StudentVehicle kVehicle;
	pkLocalPlayer->play( &kVehicle );
	pkControlledEntity = pkLocalPlayer->getControlledEntity();
	pkControlledEntity = NULL;
}
pkControlledEntity = pkLocalPlayer->getControlledEntity();
pkControlledEntity = NULL;

#endif

//-----------------------------------------------------------------------------
LocalPlayerController::LocalPlayerController():m_kOutput(Vec3::zero)
{
	assert( NULL != OpenSteerUTData::g_openSteerUTDataPtr );
	assert( NULL == OpenSteerUTData::g_openSteerUTDataPtr->localPlayerController );
	OpenSteerUTData::g_openSteerUTDataPtr->localPlayerController = this;
	this->configure();
}

//-----------------------------------------------------------------------------
LocalPlayerController::~LocalPlayerController()
{

}

//-----------------------------------------------------------------------------
AbstractController* LocalPlayerController::accessLocalPlayerController( void )
{
	return &LocalPlayerController::accessController();
}

//-----------------------------------------------------------------------------
LocalPlayerController& LocalPlayerController::accessController( void )
{
	assert( NULL != OpenSteerUTData::g_openSteerUTDataPtr );
	assert( NULL != OpenSteerUTData::g_openSteerUTDataPtr->localPlayerController );
	return *OpenSteerUTData::g_openSteerUTDataPtr->accessLocalPlayerController();
}

//-----------------------------------------------------------------------------
void LocalPlayerController::configure( void )
{
	this->setAxisMapping( 'w', EControllerAction_Forward );
	this->setAxisMapping( 'a', EControllerAction_Left );
	this->setAxisMapping( 's', EControllerAction_Backward );
	this->setAxisMapping( 'd', EControllerAction_Right );
	this->setAxisMapping( 'k', EControllerAction_Kick );
}

//-----------------------------------------------------------------------------
bool LocalPlayerController::keyboardFunc( unsigned char key, int x, int y )
{
	LocalPlayerController& kController = LocalPlayerController::accessController();
	if( true == kController.hasAxisMapping( key ) )
	{
		kController.setAxisValue( key, 1.0f );
		return true;
	}
	return false;
}

//-----------------------------------------------------------------------------
bool LocalPlayerController::keyboardFuncUp( unsigned char key, int x, int y )
{
	LocalPlayerController& kController = LocalPlayerController::accessController();
	if( true == kController.hasAxisMapping( key ) )
	{
		kController.setAxisValue( key, 0.0f );
		return true;
	}
	return false;
}

//-----------------------------------------------------------------------------
void LocalPlayerController::updateCustom( 
	AbstractUpdated* pkParent, const osScalar currentTime, const osScalar elapsedTime )
{
	AbstractController* pkTargetController = dynamic_cast<AbstractController*>(pkParent);
	if( NULL != pkTargetController )
	{
		for( size_t i = 0; i < EControllerAction_Count; ++i )
		{
			EControllerAction eAction = (EControllerAction)i;
			// pass the control force from here to the actual 'in game' controller
			pkTargetController->setActionValue( eAction, this->getActionValue( eAction ) );
		}
		pkTargetController->update( currentTime, elapsedTime );

	}
}
