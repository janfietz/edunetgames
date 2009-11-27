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
#include "OpenSteer/Utilities.h"

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
	this->reset();
}

//-----------------------------------------------------------------------------
SimpleController::~SimpleController()
{

}

//-----------------------------------------------------------------------------
void SimpleController::update( const osScalar currentTime, const osScalar elapsedTime )
{
	BaseClass::update( currentTime, elapsedTime );
	float fBackwardForward = this->getActionValue( EControllerAction_Forward ) - 
		this->getActionValue( EControllerAction_Backward );
	float fLeftRight = this->getActionValue(EControllerAction_Left) - 
		this->getActionValue(EControllerAction_Right);

	fBackwardForward = OpenSteer::clamp( fBackwardForward, -1.0f, 1.0f );
	fLeftRight = OpenSteer::clamp( fLeftRight, -1.0f, 1.0f );

	const bool bRightHanded = true;
	if( true == bRightHanded )
	{
		this->m_kOutput = Vec3::forward * fBackwardForward + Vec3::side * -fLeftRight;
	}
	else
	{
		this->m_kOutput = Vec3::forward * fBackwardForward + Vec3::side * fLeftRight;
	}
	const float fOutputLength = this->m_kOutput.length();
	if( fOutputLength > 0 )
	{
		this->m_kOutput /= fOutputLength;
	}
}

//-----------------------------------------------------------------------------
void SimpleController::reset( void )
{
	for( size_t i = 0; i < EAxis_Count; ++i )
	{
		this->m_bAxisMapped[i] = false;
	}
	for( size_t i = 0; i < EControllerAction_Count; ++i )
	{
		this->m_iAxisMapping[i][0] = -1;
		this->m_iAxisMapping[i][1] = -1;
		this->m_fActionValue[i] = 0.0f;
	}
	this->m_kOutput = Vec3::zero;
}

//-----------------------------------------------------------------------------
void SimpleController::setAxisValue( size_t uiAxis, float fValue )
{
	if( uiAxis < EAxis_Count )
	{
		fValue = OpenSteer::clamp( fValue, -1.0f, 1.0f );
		// as we want to submit this stuff to the network we also need to
		// set the corresponding action value
		for( size_t i = 0; i < EControllerAction_Count; ++i )
		{
			if( this->m_iAxisMapping[i][0] == uiAxis )
			{
				this->setActionValue( (EControllerAction)i, fValue );
				break;
			}
			else if( this->m_iAxisMapping[i][1] == uiAxis )
			{
				this->setActionValue( (EControllerAction)i, fValue );
				break;
			}
		}
	}
}

//-----------------------------------------------------------------------------
void SimpleController::setActionValue( EControllerAction eAction, float fValue )
{
	EControllerAction eClampedAction = OpenSteer::clamp( eAction, EControllerAction_First, EControllerAction_Last );
	if( eClampedAction == eAction )
	{
		this->m_fActionValue[eAction] = fValue;
	}
}

//-----------------------------------------------------------------------------
void SimpleController::setAxisMapping( size_t uiAxis, EControllerAction eAction, size_t uiIdx )
{
	EControllerAction eClampedAction = OpenSteer::clamp( eAction, EControllerAction_First, EControllerAction_Last );
	uiIdx = OpenSteer::clamp( uiIdx, (size_t)0, (size_t)1 );
	if( ( uiAxis < EAxis_Count ) && ( eClampedAction == eAction ) )
	{
		this->m_iAxisMapping[eAction][uiIdx] = uiAxis;
		this->m_bAxisMapped[uiAxis] = true;
	}
}

//-----------------------------------------------------------------------------
bool SimpleController::hasAxisMapping( size_t uiAxis ) const
{
	if( uiAxis < EAxis_Count )
	{
		return this->m_bAxisMapped[uiAxis];
	}
	return false;
}

//-----------------------------------------------------------------------------
float SimpleController::getActionValue( EControllerAction eAction ) const
{
	EControllerAction eClampedAction = OpenSteer::clamp( eAction, EControllerAction_First, EControllerAction_Last );
	if( eClampedAction == eAction )
	{
		return this->m_fActionValue[eAction];
	}
	return 0.0f;
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
