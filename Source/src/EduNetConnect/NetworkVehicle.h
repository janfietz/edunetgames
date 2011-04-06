#ifndef __NETWORKVEHICLE_H__
#define __NETWORKVEHICLE_H__

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
#include "EduNetConnect/AbstractNetSerializable.h"

namespace OpenSteer {

	//-------------------------------------------------------------------------
	enum EVehicleUpdateMode
	{
		EVehicleUpdateMode_Unknown,
		EVehicleUpdateMode_Position,
		EVehicleUpdateMode_PositionOrientation,
		EVehicleUpdateMode_BruteForce,
		EVehicleUpdateMode_PhysicsMotion,
		EVehicleUpdateMode_ForwardSpeed,
		EVehicleUpdateMode_Steer,
		EVehicleUpdateMode_Count,
	};

	//-------------------------------------------------------------------------
	enum EServerVehicleMode
	{
		EServerVehicleMode_Unknown,
		EServerVehicleMode_ExtrapolateProxy,
		EServerVehicleMode_Count,
	};

	//-------------------------------------------------------------------------
	template <class Super>
	class NetworkVehicleMixin : public Super, public AbstractNetSerializable
	{
		ET_DECLARE_BASE( Super )
	public:

		// constructor
		NetworkVehicleMixin ();

		// destructor
		virtual ~NetworkVehicleMixin ();

		// AbstractNetSerializable interface
		virtual void querySendParameters( RakNet::PRO& kPro ) const
		{

		}

	private:


	};



	//----------------------------------------------------------------------------
	// Constructor and destructor
	template<class Super>
	NetworkVehicleMixin<Super>::NetworkVehicleMixin(void)
	{
	}


	template<class Super>
	NetworkVehicleMixin<Super>::~NetworkVehicleMixin(void)
	{
	}

	//----------------------------------------------------------------------------
	// interface


	//----------------------------------------------------------------------------
	typedef struct ClientSideInterpolation_t
	{
		ClientSideInterpolation_t();
		float m_fDistanceThreshHold;
		float m_fPositionInterpolationFactor;
	} ClientSideInterpolation;

	//----------------------------------------------------------------------------
	class NetworkVehicle
	{
	public:
		static void initGui( GLUI_Panel* parentPanel, bool bRemoteGui );

		static const char* getVehicleUpdateModeString( EVehicleUpdateMode eMode )
		{
			static const char* pszModes[EVehicleUpdateMode_Count] =
			{
				"Unknown",
				"Position",
				"BruteForce",
				"PhysicsMotion",
				"ForwardSpeed",
				"Steer"
			};
			return pszModes[eMode];
		}
	};

	//----------------------------------------------------------------------------
	class NetworkVehicleUpdate : public AbstractVehicleUpdate {
		OS_DECLARE_BASE(AbstractVehicleUpdate)
	public:
		NetworkVehicleUpdate( AbstractVehicle* pkVehicle ):
		BaseClass( pkVehicle ),
			m_eLastUpdateMode( EVehicleUpdateMode_Unknown ),
			m_eLastServerVehicleMode( EServerVehicleMode_Unknown )
		{
		}
		virtual ~NetworkVehicleUpdate(){}

		EVehicleUpdateMode getVehicleUpdateMode( void ) const { return m_eLastUpdateMode; }
		EServerVehicleMode getServerVehicleMode( void ) const { return m_eLastServerVehicleMode; }
	protected:
		EVehicleUpdateMode m_eLastUpdateMode;
		EServerVehicleMode m_eLastServerVehicleMode;
	};

} // namespace OpenSteer

#endif