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

#include "OpenSteer/GlobalData.h"
#include "OpenSteer/AbstractPlugin.h"
#include "OpenSteer/AbstractVehicle.h"
#include "OpenSteer/SimplePlayer.h"
#include "OpenSteer/PluginRegistry.h"
#include "OpenSteer/Camera.h"
#include "OpenSteer/Clock.h"
#include "OpenSteer/GlobalSelection.h"
#include "OpenSteer/AbstractRenderer.h"


//-----------------------------------------------------------------------------
namespace OpenSteer {

	static bool g_bIsDll = false;
	static GlobalData* g_pkGlobalData = NULL;
	GlobalData* GlobalData::ms_pkGlobalDataInstance = NULL;

	class GlobalDataConstructor
	{
	public:
		GlobalDataConstructor()
		{
			g_pkGlobalData = new GlobalData();
		};
		virtual ~GlobalDataConstructor()
		{
			delete g_pkGlobalData;
		};

	};

	//-----------------------------------------------------------------------------
	void GlobalData::_SDMInitApp( EduNet::IProfile* pkProfile )
	{
		if( false == GlobalData::hasInstance() )
		{
			OpenSteer::GlobalSelection::_SDMInitApp( );

			static GlobalDataConstructor kConstructor;
			GlobalData* pkGlobalData = g_pkGlobalData;
			pkGlobalData->initializeGlobalData();
			pkGlobalData->m_pkProfile = pkProfile;
			GlobalData::setInstance( pkGlobalData );
		}
	}

	//-----------------------------------------------------------------------------
	void GlobalData::_SDMInitDLL( GlobalData* pkGlobalData )
	{
		g_bIsDll = true;
		GlobalData::setInstance( pkGlobalData );
	}

	//-----------------------------------------------------------------------------
	void GlobalData::setInstance( GlobalData* pkGlobalData )
	{
		GlobalData::ms_pkGlobalDataInstance = pkGlobalData;
	}

	//-----------------------------------------------------------------------------
	GlobalData* GlobalData::getInstance( void )
	{
		handleGlobalDataInstanceFailure();
		assert( NULL != GlobalData::ms_pkGlobalDataInstance );
		return GlobalData::ms_pkGlobalDataInstance;
	}

	//-----------------------------------------------------------------------------
	bool GlobalData::hasInstance( void)
	{
		return ( NULL != GlobalData::ms_pkGlobalDataInstance );
	}

	//-----------------------------------------------------------------------------
	AbstractPlayer* GlobalData::accessSimpleLocalPlayer( void )
	{
		return GlobalData::getInstance()->m_pkSimpleLocalPlayer;
	}

	//-----------------------------------------------------------------------------
	EduNet::IProfile* GlobalData::accessProfile( void )
	{
		return GlobalData::getInstance()->m_pkProfile;
	}

	//-----------------------------------------------------------------------------
	Camera* GlobalData::accessCamera( void )
	{
		// pointer to debug validity
		GlobalData* pkGlobalData = GlobalData::getInstance();

		// one static camera instance that automatically tracks selected vehicle
		return GlobalData::getInstance()->m_pkCamera;
	}

	//-----------------------------------------------------------------------------
	Clock* GlobalData::accessClock( void )
	{
		// pointer to debug validity
		GlobalData* pkGlobalData = GlobalData::getInstance();
		return GlobalData::getInstance()->m_pkClock;
	}

	//-----------------------------------------------------------------------------
	PluginRegistry* GlobalData::accessPluginRegistry( void )
	{
		return GlobalData::getInstance()->m_pkPluginRegistry;
	}

	//-----------------------------------------------------------------------------
	AbstractRenderer* GlobalData::accessRenderer( void )
	{
		return GlobalData::getInstance()->m_pkRenderer;
	}
	
	//-----------------------------------------------------------------------------
	void GlobalData::setRenderer( AbstractRenderer* pkRenderer )
	{
		GlobalData::getInstance()->m_pkRenderer = pkRenderer;
	}

	//-----------------------------------------------------------------------------
	bool GlobalData::getEnableAnnotation( void )
	{
		return GlobalData::getInstance()->m_bEnableAnnotation;
	}

	//-----------------------------------------------------------------------------
	void GlobalData::setEnableAnnotation( bool bValue )
	{
		GlobalData::getInstance()->m_bEnableAnnotation = bValue;
	}

	//-----------------------------------------------------------------------------
	bool GlobalData::getDrawPhaseActive( void )
	{
		return GlobalData::getInstance()->m_bDrawPhaseActive;
	}

	//-----------------------------------------------------------------------------
	void GlobalData::setDrawPhaseActive( bool bValue )
	{
		GlobalData::getInstance()->m_bDrawPhaseActive = bValue;
	}


	//-----------------------------------------------------------------------------
	GlobalData::GlobalData( void ):
		m_pkSimpleLocalPlayer( NULL ),
		m_pkSimpleController( NULL ),
		m_pkPluginRegistry( NULL ),
		m_pkCamera( NULL ),
		m_pkClock( NULL ),
		m_pkRenderer( NULL ),
		m_bShowClientNetworkTrail(0),
		m_bShowServerNetworkTrail(0),
		m_NetWriteFPS(20),
		m_collect3DAnnotations(0),
		m_bDebugNetStats(1),
		m_bShowMotionStatePlot(0),
		m_bEnableAnnotation(true),
		m_bDrawPhaseActive(false),
		m_SteeringForceFPS(30),
		m_bIsDll(false)
	{
		memset( this->m_bReplicationDataConfig, 0, sizeof(int) * ESerializeDataType_Count);
		this->m_bReplicationDataConfig[ESerializeDataType_Position] = 1;
		this->m_bReplicationDataConfig[ESerializeDataType_Forward] = 1;
		this->m_bReplicationDataConfig[ESerializeDataType_UpdateTicks] = 1;
	
		this->m_uiReplicationDataBytes[ESerializeDataType_Position] = sizeof(OpenSteer::Vec3) + 1;
		this->m_uiReplicationDataBytes[ESerializeDataType_Forward] = sizeof(OpenSteer::Vec3) + 1;
		this->m_uiReplicationDataBytes[ESerializeDataType_Side] = sizeof(OpenSteer::Vec3) + 1;
		this->m_uiReplicationDataBytes[ESerializeDataType_Up] = sizeof(OpenSteer::Vec3) + 1;
		this->m_uiReplicationDataBytes[ESerializeDataType_Force] = sizeof(OpenSteer::Vec3) + 1;
		this->m_uiReplicationDataBytes[ESerializeDataType_Radius] = sizeof(osScalar) + 1;
		this->m_uiReplicationDataBytes[ESerializeDataType_Speed] = sizeof(osScalar) + 1;
		this->m_uiReplicationDataBytes[ESerializeDataType_Orientation] = 0;
//		this->m_uiReplicationDataBytes[ESerializeDataType_Orientation] = sizeof(btQuaternion) + 1;
		this->m_uiReplicationDataBytes[ESerializeDataType_CompressedOrientation1] = sizeof(OpenSteer::Vec3) + 2;
		this->m_uiReplicationDataBytes[ESerializeDataType_CompressedOrientation2] = sizeof(char) * 3 + 2;
		this->m_uiReplicationDataBytes[ESerializeDataType_CompressedForce] = 0;
//		this->m_uiReplicationDataBytes[ESerializeDataType_CompressedForce] = sizeof(CompressedVector) + 1;
		this->m_uiReplicationDataBytes[ESerializeDataType_AngularVelocity] = sizeof(OpenSteer::Vec3) + 1;
		this->m_uiReplicationDataBytes[ESerializeDataType_LinearVelocity] = sizeof(OpenSteer::Vec3) + 1;
		this->m_uiReplicationDataBytes[ESerializeDataType_UpdateTicks] = sizeof(size_t) + 1;
		this->m_uiReplicationDataBytes[ESerializeDataType_ControllerAction] = sizeof(float) + 1;
		
		
	}

	//-----------------------------------------------------------------------------
	GlobalData::~GlobalData()
	{
		if( this == GlobalData::ms_pkGlobalDataInstance )
		{
			GlobalData::ms_pkGlobalDataInstance = NULL;
		}
	}

	void GlobalData::initializeGlobalData( void )
	{
		static PluginRegistry kPluginRegistry;
		this->m_pkPluginRegistry = &kPluginRegistry; 

		static Camera kCamera;
		this->m_pkCamera = &kCamera;

		static Clock kClock;
		this->m_pkClock = &kClock;


		static SimplePlayer kPlayer(true);
		this->m_pkSimpleLocalPlayer = &kPlayer;
		if( NULL == kPlayer.getController() )
		{
			static SimpleController kController;
			this->m_pkSimpleController = &kController;
			kPlayer.setController( &kController );
		}
	}

} //! namespace OpenSteer    


