#ifndef __CLIENTVEHICLEUPDATE_H__
#define __CLIENTVEHICLEUPDATE_H__

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

#include "OpenSteerUT/AbstractVehicleUpdate.h"

//-----------------------------------------------------------------------------
namespace OpenSteer {

	enum EVehicleUpdateMode
	{
		EVehicleUpdateMode_Unknown,
		EVehicleUpdateMode_BruteForce,
		EVehicleUpdateMode_PhysicsMotion,
		EVehicleUpdateMode_ForwardSpeed,
		EVehicleUpdateMode_Steer,
		EVehicleUpdateMode_Count,
	};

	//-------------------------------------------------------------------------
	class ClientVehicleUpdate : public AbstractVehicleUpdate {
		OS_DECLARE_BASE(AbstractVehicleUpdate)
	public:
		ClientVehicleUpdate( AbstractVehicle* pkVehicle ):
		BaseClass( pkVehicle ),m_eLastUpdateMode( EVehicleUpdateMode_Unknown )
		{
		}
		virtual ~ClientVehicleUpdate(){}

		//-------------------------------------------------------------------
		// interface AbstractUpdated
		virtual void updateCustom( AbstractUpdated* pkParent, const osScalar currentTime, const osScalar elapsedTime );
		virtual void update( const osScalar currentTime, const osScalar elapsedTime );

		
		EVehicleUpdateMode getVehicleUpdateMode( void ) const { return m_eLastUpdateMode; }
		static const char* getVehicleUpdateModeString( EVehicleUpdateMode eMode )
		{
			static const char* pszModes[EVehicleUpdateMode_Count] =
			{
				"Unknown",
				"BruteForce",
				"PhysicsMotion",
				"ForwardSpeed",
				"Steer"
			};
			return pszModes[eMode];
		}
	private:
		
		EVehicleUpdateMode determineUpdateMode( const class SimpleNetworkVehicle& kVehicle ) const;

		void updateBruteForce( class SimpleNetworkVehicle& kVehicle, const osScalar currentTime, const osScalar elapsedTime );
		void updatePhysicsMotion( class SimpleNetworkVehicle& kVehicle, const osScalar currentTime, const osScalar elapsedTime );
		void updateForwardSpeed( class SimpleNetworkVehicle& kVehicle, const osScalar currentTime, const osScalar elapsedTime );
		void updateSteer( class SimpleNetworkVehicle& kVehicle, const osScalar currentTime, const osScalar elapsedTime );
		void updateUnknown( class SimpleNetworkVehicle& kVehicle, const osScalar currentTime, const osScalar elapsedTime );

		EVehicleUpdateMode m_eLastUpdateMode;

	};



} // namespace OpenSteer

#endif // __SIMPLENETWORKVEHICLEUPDATE_H__