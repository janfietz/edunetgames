#ifndef __ABSTRACTPLAYER_H__
#define __ABSTRACTPLAYER_H__

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

#include "OpenSteer/AbstractUpdated.h"
#include "OpenSteer/AbstractEntity.h"

#include <vector>
//-----------------------------------------------------------------------------
namespace OpenSteer
{
	class Vec3;
	class AbstractEntity;

	//-------------------------------------------------------------------------
	enum EControllerAction
	{
		EControllerAction_Yaw,
		EControllerAction_Pitch,
		EControllerAction_Roll,
		EControllerAction_Left,
		EControllerAction_Right,
		EControllerAction_Backward,
		EControllerAction_Forward,
		EControllerAction_Thrust,
		EControllerAction_Jump,
		EControllerAction_Duck,
		EControllerAction_Prone,
		EControllerAction_Shoot,
		EControllerAction_Move,
		EControllerAction_Kick,
		EControllerAction_Special_00,
		EControllerAction_Special_01,
		EControllerAction_Special_02,
		EControllerAction_Special_03,
		EControllerAction_Special_04,
		EControllerAction_Special_05,
		EControllerAction_Special_06,
		EControllerAction_Special_07,
		EControllerAction_Special_08,
		EControllerAction_Special_09,
		EControllerAction_Special_10,
		EControllerAction_Count,
		EControllerAction_First = EControllerAction_Yaw,
		EControllerAction_Last = EControllerAction_Count - 1,
	};

	//-------------------------------------------------------------------------
	class AbstractController : public AbstractEntity, public AbstractUpdated
	{
	public:
		virtual ~AbstractController(){}

		virtual const Vec3& getOutputForce( void ) const OS_ABSTRACT;

		virtual float getActionValue( EControllerAction eAction ) const OS_ABSTRACT;
		virtual void setActionValue( EControllerAction eAction, float fValue ) OS_ABSTRACT;
	};

	//-------------------------------------------------------------------------
	class AbstractPlayer : public AbstractEntity, public AbstractUpdated 
	{
	public:
		virtual ~AbstractPlayer(){}

		virtual void setController( AbstractController* ) OS_ABSTRACT;
		virtual AbstractController* accessController( void ) const OS_ABSTRACT;
		virtual AbstractController const* const getController( void ) const OS_ABSTRACT;

		virtual AbstractEntity* getControlledEntity( void ) const OS_ABSTRACT;

		virtual bool isPlaying( void ) const OS_ABSTRACT; 

		virtual bool isLocalPlayer( void ) const OS_ABSTRACT; 

	};

	//-------------------------------------------------------------------------
	typedef std::vector<AbstractPlayer*> AbstractPlayerGroup;


}

#endif //  __ABSTRACTPLAYER_H__