#ifndef __ABSTRACTVEHICLEUTILITIES_H__
#define __ABSTRACTVEHICLEUTILITIES_H__

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

//#include "EduNetCommon/EduNetCommon.h"
#include "OpenSteer/AbstractVehicle.h"
#include "OpenSteer/Color.h"
#include "OpenSteer/Camera.h"
#include "OpenSteer/SimpleVehicle.h"
#include "OpenSteer/Plugin.h"
//-----------------------------------------------------------------------------
namespace OpenSteer
{
	using namespace OpenSteer;

	class VehicleUtilities
	{
	public:
		// draws a gray disk on the XZ plane under a given vehicle
		static void highlightVehicleUtility ( AbstractRenderer* pRenderer,
			const AbstractVehicle& vehicle);

		// draws a gray circle on the XZ plane under a given vehicle
		static void circleHighlightVehicleUtility ( AbstractRenderer* pRenderer,
			const AbstractVehicle& vehicle);

		// draw a box around a vehicle aligned with its local space
		// xxx not used as of 11-20-02
		static void drawBoxHighlightOnVehicle (AbstractRenderer* pRenderer,
			const AbstractVehicle& v,
			const Color& color);

		// draws a colored circle (perpendicular to view axis) around the center
		// of a given vehicle.  The circle's radius is the vehicle's radius times
		// radiusMultiplier.
		static void drawCircleHighlightOnVehicle (AbstractRenderer* pRenderer,
			const AbstractVehicle& v,
			const float radiusMultiplier,
			const Color& color);

		// select the "next" vehicle: cycle through the registry
		static void selectNextVehicle (void);

		// select vehicle nearest the given screen position (e.g.: of the mouse)
		static void selectVehicleNearestScreenPosition (OpenSteer::AbstractRenderer* pRenderer,
			int x, int y);

		// ---------------------------------------------------------- mouse support


		// Find the AbstractVehicle whose screen position is nearest the
		// given window coordinates, typically the mouse position.  Note
		// this will return NULL if there are no AbstractVehicles.
		static AbstractVehicle* findVehicleNearestScreenPosition (OpenSteer::AbstractRenderer* pRenderer,
			int x, int y);
	};

	//-------------------------------------------------------------------------
	template <class Super, EntityClassId classId = 0>
	class VehicleClassIdMixin : public Super
	{
	public:

		VehicleClassIdMixin()
		{
		}

		VehicleClassIdMixin( OpenSteer::ProximityDatabase* pd ):Super( pd )
		{

		}

		virtual ~VehicleClassIdMixin()
		{
		}

		// AbstractEntity interface
		virtual AbstractEntity* cloneEntity( void ) const
		{
			return new VehicleClassIdMixin();
		}

		// important implement new clone functionality
		//-------------------------------------------------------------------------
		virtual OpenSteer::AbstractVehicle* cloneVehicle( void ) const
		{
			OpenSteer::AbstractVehicle* pkVehicle = new VehicleClassIdMixin();
			return pkVehicle;
		}

		virtual EntityClassId getClassId( void ) const
		{
			static EntityClassId sClassId = classId;
			return sClassId;
		}

	private:

	};

}


#endif // __ABSTRACTVEHICLEUTILITIES_H__