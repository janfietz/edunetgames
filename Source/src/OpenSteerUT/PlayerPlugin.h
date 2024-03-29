#ifndef __PLAYERPLUGIN_H__
#define __PLAYERPLUGIN_H__

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
// implements a plugin doing nothing
#include "EduNetCommon/TUpdatePeriod.h"
#include "EduNetCommon/InstanceCount.h"

#include "OpenSteer/Plugin.h"
#include "OpenSteer/SimpleVehicle.h"
#include "OpenSteerUT/AbstractVehicleGroup.h"
#include "OpenSteerUT/SimplePhysicsVehicle.h"
#include "OpenSteerUT/AbstractVehicleMotionStatePlot.h"
#include "OpenSteerUT/EmptyPlugin.h"


// anonymous namespace
namespace EduNet{

	using namespace OpenSteer;



	class PlayerPlugin : public Plugin
	{
		ET_DECLARE_BASE(Plugin)
	public:

		// construction / destruction
		PlayerPlugin (bool bAddToRegistry = true):BaseClass(bAddToRegistry),
			m_bShowMotionStatePlot(0)
		{};
		virtual ~PlayerPlugin() {}

		virtual const char* name (void) const {return "PlayerPlugin";}

		virtual float selectionOrderSortKey (void) const {return 0.9f;}

		virtual void open (void);

		virtual void update (const float currentTime, const float elapsedTime);

		virtual void redraw (OpenSteer::AbstractRenderer* pRenderer, const float currentTime, const float elapsedTime);

		virtual void close (void)
		{
		}

		virtual void reset (void)
		{
			AbstractVehicleGroup kVehicles( m_kVehicles );
			kVehicles.reset( );
		}

		const AVGroup& allVehicles (void) const {return m_kVehicles;}
		AVGroup& allVehicles (void) {return m_kVehicles;}

		void initGui( void* pkUserdata );

		OpenSteer::AbstractVehicleMotionStatePlot m_kMotionStateProfile;
		int m_bShowMotionStatePlot;
	private:
		AVGroup m_kVehicles; // for allVehicles
		EmptyVehicle m_kVehicle;
		ET_IMPLEMENT_CLASS_NO_COPY( PlayerPlugin )
	};
}
#endif //__PLAYERPLUGIN_H__