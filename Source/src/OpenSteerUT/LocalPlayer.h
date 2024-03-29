#ifndef __LOCALPLAYER_H__
#define __LOCALPLAYER_H__

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

//-----------------------------------------------------------------------------
namespace OpenSteer
{
	//-------------------------------------------------------------------------
	class LocalPlayerController : public SimpleController
	{
	public:
		LocalPlayerController();
		virtual ~LocalPlayerController();

		static AbstractController* accessLocalPlayerController( void );

		void configure( void );

		virtual void setOutputForce( const Vec3& kOutput )
		{
			this->m_kOutput = kOutput;
		}

		virtual const Vec3& getOutputForce( void ) const
		{
			return this->m_kOutput;
		}

		virtual void updateCustom( AbstractUpdated* pkParent, const osScalar currentTime, const osScalar elapsedTime );

		static bool keyboardFunc( unsigned char key, int x, int y );
		static bool keyboardFuncUp( unsigned char key, int x, int y );
	private:
		static LocalPlayerController& accessController( void );
		Vec3 m_kOutput;
	};
}

#endif //  __LOCALPLAYER_H__