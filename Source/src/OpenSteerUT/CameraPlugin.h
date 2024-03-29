#ifndef __CAMERAPLUGIN_H__
#define __CAMERAPLUGIN_H__

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
#include "EduNetCommon/EduNetCommon.h"


namespace OpenSteer
{
	//-------------------------------------------------------------------------
	class CameraPlugin : public Plugin
	{
		ET_DECLARE_BASE(OpenSteer::Plugin)
	public:
		CameraPlugin (bool bAddToRegistry = false):BaseClass(bAddToRegistry)
		{};

		OS_IMPLEMENT_CLASSNAME( CameraPlugin )
		//----------------------------------------------------------------------------
		// OpenSteer::Plugin interface
		virtual void initGui( void* pkUserdata );
		// required methods:
		virtual const char* name (void) const {return this->getClassName();}
		virtual void open (void);
		virtual void update (const float currentTime, const float elapsedTime);
		virtual void redraw ( AbstractRenderer* pRenderer,
			const float currentTime, const float elapsedTime) OS_OVERRIDE;
		virtual void close (void) { }

		const AVGroup& allVehicles (void) const {return m_kVehicles;}
		AVGroup& allVehicles (void) {return m_kVehicles;}

		// optional methods (see comments in AbstractPlugin for explanation):
		void reset (void) { } // default is to reset by doing close-then-open
		float selectionOrderSortKey (void) const {return 1000000;}
		bool requestInitialSelection (void) const {return false;}
		
		// ------------------------------------------------------- camera utilities
		// set a certain initial camera state used by several plug-ins
		void init2dCamera (AbstractLocalSpace& selected);
		void init2dCamera (AbstractLocalSpace& selected,
			float distance,
			float elevation);
		void init3dCamera (AbstractLocalSpace& selected);
		void init3dCamera (AbstractLocalSpace& selected,
			float distance,
			float elevation);

		// set initial position of camera based on a vehicle
		void position3dCamera (AbstractLocalSpace& selected);
		void position3dCamera (AbstractLocalSpace& selected,
			float distance,
			float elevation);
		void position2dCamera (AbstractLocalSpace& selected);
		void position2dCamera (AbstractLocalSpace& selected,
			float distance,
			float elevation);

		// camera updating utility used by several (all?) plug-ins
		void setCameraTarget ( const AbstractLocalSpace* selected );

		Camera& accessCamera() const { return m_Camera; }

		// some camera-related default constants
		static const float camera2dElevation;
		static const float cameraTargetDistance;
		static const Vec3 cameraTargetOffset;

	private:
			
		mutable Camera m_Camera;
		AVGroup m_kVehicles; // for allVehicles

		ET_IMPLEMENT_CLASS_NO_COPY(CameraPlugin)

	};

}


#endif  // __CAMERAPLUGIN_H__