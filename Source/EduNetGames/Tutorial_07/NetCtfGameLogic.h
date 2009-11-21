#ifndef __NETCTFGAMELOGIC_H__
#define __NETCTFGAMELOGIC_H__
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
#include "EduNetCommon/EduNetCommon.h"
#include "OpenSteerUT/AbstractVehicleUpdate.h"

//-----------------------------------------------------------------------------
class NetCtfGameLogic : public OpenSteer::AbstractVehicleUpdate
{
	ET_DECLARE_BASE( OpenSteer::AbstractVehicleUpdate );
public:
	NetCtfGameLogic( osAbstractVehicle* pkVehicle = NULL);
	virtual ~NetCtfGameLogic();

	OS_IMPLEMENT_CLASSNAME( NetCtfGameLogic )

	//-------------------------------------------------------------------------
	// interface AbstractUpdated
	virtual void update (const float currentTime, const float elapsedTime);

	//-------------------------------------------------------------------------
	void setPlugin( class NetCtfPlugin* pkPlugin )
	{
		this->m_pkPlugin = pkPlugin;
	}

	bool isGameOver( void )
	{
		return m_bGameOver;
	}

	static osVector3 ms_kHomeBaseCenter;
	static float ms_fHomeBaseRadius;

protected:
	class NetCtfPlugin* m_pkPlugin;
	bool m_bGameOver;
};



#endif // __NETCTFGAMELOGIC_H__