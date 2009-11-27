#ifndef __SIMPLEPLAYER_H__
#define __SIMPLEPLAYER_H__

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

#include "OpenSteer/AbstractPlayer.h"
#include "OpenSteer/Entity.h"
#include "OpenSteer/Vec3.h"

//-----------------------------------------------------------------------------
namespace OpenSteer
{

	// contoller mixin templates
	typedef EntityVisibilityMixin<AbstractController> TAbstractController_000;
	typedef EntityParentMixin<TAbstractController_000> TAbstractController_00;
	typedef EntityMixin<TAbstractController_00> TAbstractController_0;

	typedef AbstractUpdatedMixin<TAbstractController_0> TAbstractController;

	// player mixin templates
	typedef EntityVisibilityMixin<AbstractPlayer> TAbstractPlayer_000;
	typedef EntityParentMixin<TAbstractPlayer_000> TAbstractPlayer_00;
	typedef EntityMixin<TAbstractPlayer_00> TAbstractPlayer_0;

	typedef AbstractUpdatedMixin<TAbstractPlayer_0> TAbstractPlayer;

	//-------------------------------------------------------------------------
	class SimpleController : public TAbstractController
	{
		OS_DECLARE_BASE( TAbstractController )
	public:
		SimpleController();
		virtual ~SimpleController();
		OS_IMPLEMENT_CLASSNAME( SimpleController )

		virtual void setOutputForce( const Vec3& kOutput )
		{
			this->m_kOutput = kOutput;
		}

		virtual const Vec3& getOutputForce( void ) const
		{
			return this->m_kOutput;
		}

	private:
		Vec3 m_kOutput;
	};

	//-------------------------------------------------------------------------
	class SimplePlayer : public TAbstractPlayer
	{
		OS_DECLARE_BASE( TAbstractPlayer )
	public:
		SimplePlayer( bool bIsLocalPlayer = false );
		virtual ~SimplePlayer();
		OS_IMPLEMENT_CLASSNAME( SimplePlayer )

		//---------------------------------------------------------------------
		// AbstractPlayer interface
		virtual void setController( AbstractController* pkController)
		{
			this->m_pkController = pkController;
		}

		virtual AbstractController* accessController( void ) const
		{
			return this->m_pkController;
		}

		virtual AbstractController const* const getController( void ) const
		{
			return this->m_pkController;
		}

		virtual bool isPlaying( void ) const
		{
			return (NULL != this->getControlledEntity());
		}

		virtual bool isLocalPlayer( void ) const
		{
			return this->m_bIsLocalPlayer;
		}

		static AbstractPlayer* accessLocalPlayer( void );

		//---------------------------------------------------------------------
		// AbstractUpdated interface
		virtual void update( const osScalar currentTime, const osScalar elapsedTime );

	private:
		AbstractController* m_pkController;
		bool m_bIsLocalPlayer;
	};

}

#endif //  __SIMPLEPLAYER_H__