#ifndef __OPENSTEERUT_H__
#define	__OPENSTEERUT_H__
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
#include "glui/glui_internal_control.h"
#include "EduNetCommon/TUpdatePeriod.h"
#include "EduNetCommon/EduNetCommon.h"
#include "OpenSteer/GlobalSelection.h"

//-----------------------------------------------------------------------------
typedef struct ViewPort_t
{
	ViewPort_t( void ) :tx(0),ty(0),tw(0),th(0)
	{
	}
	int tx, ty, tw, th;
} ViewPort;

//-----------------------------------------------------------------------------
typedef struct OpenSteerUTData_t OpenSteerUTData;

//-----------------------------------------------------------------------------
typedef struct OpenSteerUTData_t
{
	OpenSteerUTData_t( void ):
		appGlui(NULL),
		updatePhaseActive(false),
		drawPhaseActive(false),
		globalSelection(NULL)
	{
	}

	GLUI* appGlui;
	ViewPort viewPort;
	bool updatePhaseActive;
	bool drawPhaseActive;

	OpenSteer::GlobalSelection* globalSelection;

	static OpenSteerUTData* g_openSteerUTDataPtr;

} OpenSteerUTData;



#endif // __OPENSTEERUT_H__
