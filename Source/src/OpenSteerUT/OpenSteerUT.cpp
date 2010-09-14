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


#include "OpenSteerUT.h"

#include "OpenSteer/GlobalSelection.h"
#include "OpenSteer/GlobalData.h"

OpenSteerUTData* OpenSteerUTData::g_openSteerUTDataPtr = NULL;

//-----------------------------------------------------------------------------
void OpenSteerUTData::_SDMInitApp( EduNet::IProfile* pkProfile )
{
	OpenSteer::GlobalData::_SDMInitApp( pkProfile );
	// note: set up data to pass to loaded modules
	static OpenSteerUTData g_openSteerUTData;
	OpenSteerUTData::g_openSteerUTDataPtr = &g_openSteerUTData;
	OpenSteerUTData::g_openSteerUTDataPtr->globalSelection = OpenSteer::GlobalSelection::getInstance();
	OpenSteerUTData::g_openSteerUTDataPtr->globalData = OpenSteer::GlobalData::getInstance();

	static OpenSteer::LocalPlayerController g_localPlayerController;
	OpenSteer::AbstractController* localPlayerController = OpenSteer::LocalPlayerController::accessLocalPlayerController();
	if( NULL != localPlayerController )
	{
		bool bTest = true;
		bTest = false;
	}
}


//-----------------------------------------------------------------------------
void OpenSteerUTData::_SDMInitDLL( OpenSteerUTData* pkData )
{
	OpenSteerUTData::g_openSteerUTDataPtr = pkData;
	OpenSteer::GlobalSelection::_SDMInitDLL( OpenSteerUTData::g_openSteerUTDataPtr->globalSelection );
	OpenSteer::GlobalData::_SDMInitDLL( OpenSteerUTData::g_openSteerUTDataPtr->globalData );
}

//-----------------------------------------------------------------------------
// graphical annotation: master on/off switch
bool OpenSteer::enableAnnotation = true;

//-----------------------------------------------------------------------------
namespace OpenSteer
{
	bool updatePhaseActive = false;
	bool drawPhaseActive = false;
}

//-----------------------------------------------------------------------------
GLUI* getRootGLUI()
{
	assert( NULL != OpenSteerUTData::g_openSteerUTDataPtr );
	return OpenSteerUTData::g_openSteerUTDataPtr->appGlui;
}

//-----------------------------------------------------------------------------
// TODO: find a good place to call this
// void updateDataAndPhases( void )
// {
// 	// note: in case this is a dll the data ptr is different to the data
// 	if( g_openSteerUTDataPtr != &g_openSteerUTData )
// 	{
// 		g_openSteerUTData = *g_openSteerUTDataPtr;
// 		OpenSteer::updatePhaseActive = g_openSteerUTData.updatePhaseActive;
// 		OpenSteer::drawPhaseActive = g_openSteerUTData.drawPhaseActive;
// 	}
// }

//-----------------------------------------------------------------------------
namespace
{
}

//-----------------------------------------------------------------------------
// accessors for GLUT's window dimensions
float
OpenSteer::drawGetWindowHeight ( void )
{
	assert( NULL != OpenSteerUTData::g_openSteerUTDataPtr );
	if ( OpenSteerUTData::g_openSteerUTDataPtr->viewPort.th > 0 )
	{
		return OpenSteerUTData::g_openSteerUTDataPtr->viewPort.th;
	}
	return glutGet ( GLUT_WINDOW_HEIGHT );
}

//-----------------------------------------------------------------------------
float
OpenSteer::drawGetWindowWidth ( void )
{
	assert( NULL != OpenSteerUTData::g_openSteerUTDataPtr );
	if ( OpenSteerUTData::g_openSteerUTDataPtr->viewPort.tw > 0 )
	{
		return OpenSteerUTData::g_openSteerUTDataPtr->viewPort.tw;
	}
	return glutGet ( GLUT_WINDOW_WIDTH );
}


