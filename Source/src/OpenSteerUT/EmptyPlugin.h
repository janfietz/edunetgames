#ifndef __EMPTYPLUGIN_H__
#define __EMPTYPLUGIN_H__

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

#include "OpenSteerUT/PluginArray.h"
#include "OpenSteer/SimpleVehicle.h"
#include "OpenSteerUT/AbstractVehicleGroup.h"
#include "OpenSteerUT/SimplePhysicsVehicle.h"
#include "OpenSteerUT/AbstractVehicleMotionStatePlot.h"

#include "OpenSteerUT/CameraPlugin.h"
#include "OpenSteerUT/GridPlugin.h"


// anonymous namespace
namespace EduNet{

	using namespace OpenSteer;

	class EmptyVehicle : public SimplePhysicsVehicle
	{
		ET_DECLARE_BASE( SimplePhysicsVehicle )
	public:

		EmptyVehicle():m_uiId( EmptyVehicle::ms_kInstanceCount.Constructor() )
		{
			this->reset();
		}

		virtual ~EmptyVehicle()
		{
			EmptyVehicle::ms_kInstanceCount.Destructor();
		}

		// reset vehicle state
		virtual void reset (void)
		{
			BaseClass::reset();
			this->setMaxForce( 9 );
			// velocity is clipped to this magnitude
			this->setMaxSpeed( 9 );

			m_kUpdatePeriod.SetPeriodTime( 0.5f );
		}

		// per frame simulation update
		virtual void update (const float currentTime, const float elapsedTime)
		{
			BaseClass::update( currentTime, elapsedTime );

			// test the UpdatePeriod
			size_t uiTicks = m_kUpdatePeriod.UpdateDeltaTime( elapsedTime );
			if( uiTicks >= 1 )
			{
				bool bTest = true;
				bTest  = false;
			}
		}

		// draw this character/vehicle into the scene
		virtual void draw (OpenSteer::AbstractRenderer* pRenderer, const float currentTime, const float elapsedTime)
		{
			pRenderer->drawBasic2dCircularVehicle (*this, gGray50);
		}
		EduNet::UpdatePeriodFloat m_kUpdatePeriod;
		static OpenSteer::InstanceTracker ms_kInstanceCount;
		size_t m_uiId;
		ET_IMPLEMENT_CLASS_NO_COPY( EmptyVehicle )
	};


	class EmptyPlugin : public PluginArray
	{
		ET_DECLARE_BASE(PluginArray)
	public:

		// construction / destruction
		EmptyPlugin (bool bAddToRegistry = true):BaseClass(bAddToRegistry),
			m_bShowMotionStatePlot(0),
			m_bShowSamplePlot(1)
		{};
		virtual ~EmptyPlugin() {}

		virtual const char* name (void) const {return "EmptyPlugin";}

		virtual float selectionOrderSortKey (void) const {return 1.0f;}

		virtual class wxWindow* prepareGui (class wxWindow* parent ) OS_OVERRIDE;
		virtual void open (void);

		virtual void update (const float currentTime, const float elapsedTime);

		virtual void redraw (OpenSteer::AbstractRenderer* pRenderer, const float currentTime, const float elapsedTime);

		virtual void close (void);

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
		int m_bShowSamplePlot;
	private:
		AVGroup m_kVehicles; // for allVehicles
		EmptyVehicle m_kVehicle;

		OpenSteer::CameraPlugin* m_pCamera;
		OpenSteer::GridPlugin* m_pGrid;

		ET_IMPLEMENT_CLASS_NO_COPY( EmptyPlugin )
	};
}
#endif //__EMPTYPLUGIN_H__