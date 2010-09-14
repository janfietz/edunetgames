#ifndef OPENSTEER_GLOBALDATA_H
#define OPENSTEER_GLOBALDATA_H

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
#include "OpenSteer/AbstractVehicle.h"
namespace EduNet
{
	class IProfile;
}
//-----------------------------------------------------------------------------
namespace OpenSteer {
	class AbstractPlayer;
	class AbstractController;
	class PluginRegistry;
	class Camera;

	class GlobalData
	{
	public:
		GlobalData();
		virtual ~GlobalData();

		static void _SDMInitApp( EduNet::IProfile* pkProfile );
		static void _SDMInitDLL( GlobalData* pkGlobalData );
		static GlobalData* getInstance( void );

		static AbstractPlayer* accessSimpleLocalPlayer( void );
		static Camera* accessCamera( void );
		
		static EduNet::IProfile* accessProfile( void );

		static PluginRegistry* accessPluginRegistry( void );

		int m_bReplicationDataConfig[ESerializeDataType_Count];
		size_t m_uiReplicationDataBytes[ESerializeDataType_Count];
		int m_bShowClientNetworkTrail;
		int m_bShowServerNetworkTrail;
		osScalar m_NetWriteFPS;
		int m_collect3DAnnotations;
		int m_bDebugNetStats;
		int m_bShowMotionStatePlot;
		osScalar m_SteeringForceFPS;

	private:
		void initializeGlobalData( void );
		
		// note: global member variable pointers
		AbstractPlayer* m_pkSimpleLocalPlayer;
		AbstractController* m_pkSimpleController;
		PluginRegistry* m_pkPluginRegistry;
		Camera* m_pkCamera;
		EduNet::IProfile* m_pkProfile;
		bool m_bIsDll;


		static void setInstance( GlobalData* pkGlobalData );
		
		static GlobalData* ms_pkGlobalDataInstance;
		static GlobalData ms_kGlobalDataInstance;
	};

} //! namespace OpenSteer    


//-----------------------------------------------------------------------------
#endif //! OPENSTEER_GLOBALDATA_H
