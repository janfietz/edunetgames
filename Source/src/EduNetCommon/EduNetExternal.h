#ifndef __EDUNETEXTERNAL_H__
#define __EDUNETEXTERNAL_H__

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

// include configuration
#include "EduNetCore/EduNetConfig.h"

//-----------------------------------------------------------------------------
// opensteer
#if EDUNET_HAVE_OPENSTEER

#include "OpenSteer/Vec3.h"
#include "OpenSteer/Clock.h"
#include "OpenSteer/Plugin.h"
#include "OpenSteer/Camera.h"
#include "OpenSteer/Utilities.h"
#include "OpenSteer/AbstractVehicle.h"
#include "OpenSteer/SimpleVehicle.h"
#include "OpenSteer/Annotation.h"
#include "OpenSteer/Color.h"

#include "OpenSteer/Obstacle.h"
#include "OpenSteer/Proximity.h"
#include "OpenSteer/PolylineSegmentedPathwaySingleRadius.h"
#include "OpenSteer/SimplePlayer.h"



//-----------------------------------------------------------------------------
// profiling
#if EDUNET_HAVE_PROFILE
#include "iprof/prof.h"
//#define ET_PROFILE(x) Prof(x)
#else
//#define ET_PROFILE(x)
#endif


#if EDUNET_HAVE_PROFILE

#include "EduNetProfile/EduNetProfile.h"
#include "OpenSteer/GlobalData.h"

#define ET_PROFILE(__name) \
	EduNet::IProfileNodePtr spNode##__name ( (EduNet::IProfileNode*)NULL );\
	{EduNet::IProfile* pkProfile = OpenSteer::GlobalData::accessProfile();\
	if( NULL != pkProfile )\
	{\
		spNode##__name = pkProfile->allocNode( #__name );\
	}}

#else
#define ET_PROFILE(__name)
#endif



#endif // EDUNET_HAVE_OPENSTEER


#endif // __EDUNETEXTERNAL_H__
