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

#include "EduNetConnect/NetworkVehicle.h"
#include "EduNetConnect/SimpleNetworkVehicle.h"
#include "EduNetCommon/EduNetDraw.h"
#include "OpenSteer/GlobalData.h"

using namespace OpenSteer;

//-------------------------------------------------------------------------
ClientSideInterpolation NetworkVehicle::ms_ClientSideInterpolation;

//-------------------------------------------------------------------------
void NetworkVehicle::initGui( GLUI_Panel* parentPanel, bool bRemoteGui )
{
	assert( NULL != parentPanel );
	GLUI* glui = ::getRootGLUI();
	// network vehicle  gui
	GLUI_Rollout* replicationRollout = glui->add_rollout_to_panel( parentPanel, "Entity Replication", true );	
	GLUI_Panel* replicationPanel = replicationRollout;
	if( false == bRemoteGui )
	{
		glui->add_checkbox_to_panel( replicationPanel, "Data Trail", &GlobalData::getInstance()->m_bShowServerNetworkTrail);
		glui->add_separator_to_panel( replicationPanel );
		glui->add_checkbox_to_panel( replicationPanel, "UpdateTicks", &GlobalData::getInstance()->m_bReplicationDataConfig[ESerializeDataType_UpdateTicks]);
		glui->add_separator_to_panel( replicationPanel );
		glui->add_checkbox_to_panel( replicationPanel, "Position", &GlobalData::getInstance()->m_bReplicationDataConfig[ESerializeDataType_Position]);
		glui->add_separator_to_panel( replicationPanel );
		glui->add_checkbox_to_panel( replicationPanel, "Forward", &GlobalData::getInstance()->m_bReplicationDataConfig[ESerializeDataType_Forward]);
		glui->add_checkbox_to_panel( replicationPanel, "Side", &GlobalData::getInstance()->m_bReplicationDataConfig[ESerializeDataType_Side]);
		glui->add_checkbox_to_panel( replicationPanel, "Up", &GlobalData::getInstance()->m_bReplicationDataConfig[ESerializeDataType_Up]);
		glui->add_checkbox_to_panel( replicationPanel, "Orientation", &GlobalData::getInstance()->m_bReplicationDataConfig[ESerializeDataType_Orientation]);
		glui->add_checkbox_to_panel( replicationPanel, "CompressedOrientation1", &GlobalData::getInstance()->m_bReplicationDataConfig[ESerializeDataType_CompressedOrientation1]);
		glui->add_checkbox_to_panel( replicationPanel, "CompressedOrientation2", &GlobalData::getInstance()->m_bReplicationDataConfig[ESerializeDataType_CompressedOrientation2]);
		glui->add_separator_to_panel( replicationPanel );
		glui->add_checkbox_to_panel( replicationPanel, "AngularVelocity", &GlobalData::getInstance()->m_bReplicationDataConfig[ESerializeDataType_AngularVelocity]);
		glui->add_checkbox_to_panel( replicationPanel, "LinearVelocity", &GlobalData::getInstance()->m_bReplicationDataConfig[ESerializeDataType_LinearVelocity]);
		glui->add_checkbox_to_panel( replicationPanel, "Speed", &GlobalData::getInstance()->m_bReplicationDataConfig[ESerializeDataType_Speed]);
		glui->add_separator_to_panel( replicationPanel );
		glui->add_checkbox_to_panel( replicationPanel, "Force", &GlobalData::getInstance()->m_bReplicationDataConfig[ESerializeDataType_Force]);
		glui->add_checkbox_to_panel( replicationPanel, "CompressedForce", &GlobalData::getInstance()->m_bReplicationDataConfig[ESerializeDataType_CompressedForce]);
		glui->add_separator_to_panel( replicationPanel );
		glui->add_checkbox_to_panel( replicationPanel, "Radius", &GlobalData::getInstance()->m_bReplicationDataConfig[ESerializeDataType_Radius]);
	}
	else
	{
//		glui->add_separator_to_panel( replicationPanel );
		glui->add_checkbox_to_panel( replicationPanel, "Data Trail", &GlobalData::getInstance()->m_bShowClientNetworkTrail);
		GLUI_Spinner* distanceThreshHoldSpinner =
			glui->add_spinner_to_panel( replicationPanel, "Interpolation Threshhold", GLUI_SPINNER_FLOAT, 
			&NetworkVehicle::ms_ClientSideInterpolation.m_fDistanceThreshHold);
		distanceThreshHoldSpinner->set_float_limits(0.01f, 5.0f);
		GLUI_Spinner* interpolationFactorSpinner =
			glui->add_spinner_to_panel( replicationPanel, "Position Interpolation Factor", GLUI_SPINNER_FLOAT, 
			&NetworkVehicle::ms_ClientSideInterpolation.m_fPositionInterpolationFactor);
		interpolationFactorSpinner->set_float_limits(0.25f, 1.0f);
	}
}
