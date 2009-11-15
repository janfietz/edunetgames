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
#include "EduNetCommon/EduNetCommon.h"
#include "EduNetCommon/TUpdatePeriod.h"
#include "EduNetCommon/InstanceCount.h"

#include "OpenSteer/SimpleVehicle.h"
#include "OpenSteerUT/AbstractVehicleGroup.h"



// ???
// class AbstractGamePlugin : public AbstractPlugin
// {
// public:
// }


// typedef GamePluginMixin<Plugin> SimpleGamePlugin;


// anonymous namespace
namespace EduNet{

	using namespace OpenSteer;

	class EmptyVehicle : public SimpleVehicle
	{
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
			SimpleVehicle::reset();
			m_kUpdatePeriod.SetPeriodTime( 0.5f );
		}

		// per frame simulation update
		virtual void update (const float currentTime, const float elapsedTime)
		{
			// do nothing

			// test the UpdatePeriod
			size_t uiTicks = m_kUpdatePeriod.UpdateDeltaTime( elapsedTime );
			if( uiTicks >= 1 )
			{
				bool bTest = true;
				bTest  = false;
			}
		}

		// draw this character/vehicle into the scene
		virtual void draw (const float currentTime, const float elapsedTime)
		{
			drawBasic2dCircularVehicle (*this, gGray50);
		}
		EduNet::UpdatePeriodFloat m_kUpdatePeriod;
		static OpenSteer::InstanceTracker ms_kInstanceCount;
		size_t m_uiId;
		ET_IMPLEMENT_CLASS_NO_COPY( EmptyVehicle );
	};


	class EmptyPlugin : public Plugin
	{
		ET_DECLARE_BASE(Plugin);
	public:

		// construction / destruction
		EmptyPlugin (bool bAddToRegistry = true):BaseClass(bAddToRegistry){};
		virtual ~EmptyPlugin() {}

		virtual const char* name (void) const {return "EmptyPlugin";}

		virtual float selectionOrderSortKey (void) const {return 1.0f;}

		virtual void open (void);

		virtual void update (const float currentTime, const float elapsedTime)
		{
			AbstractVehicleGroup kVehicles( m_kVehicles );
			kVehicles.update( currentTime, elapsedTime );
		}

		virtual void redraw (const float currentTime, const float elapsedTime);

		virtual void close (void)
		{
		}

		virtual void reset (void)
		{
			AbstractVehicleGroup kVehicles( m_kVehicles );
			kVehicles.reset( );
		}

		const AVGroup& allVehicles (void) const {return (const AVGroup&) m_kVehicles;}

		void initGui( void* pkUserdata );

	private:
		AVGroup m_kVehicles; // for allVehicles
		EmptyVehicle m_kVehicle;
		ET_IMPLEMENT_CLASS_NO_COPY( EmptyPlugin );
	};
}
#endif //__EMPTYPLUGIN_H__