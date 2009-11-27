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

#include "EduNetConnect/ServerVehicleUpdate.h"

#include "EduNetCommon/EduNetDraw.h"
#include "EduNetConnect/SimpleNetworkVehicle.h"
#include "OpenSteerUT/AbstractVehicleMath.h"

using namespace OpenSteer;

//-----------------------------------------------------------------------------
void ServerVehicleUpdate::updateCustom( AbstractUpdated* pkParent, const osScalar currentTime, const osScalar elapsedTime )
{
	SimpleNetworkVehicle* pkNetworkVehicle = dynamic_cast<SimpleNetworkVehicle*>(pkParent);
	SimpleProxyVehicle& kProxy = pkNetworkVehicle->accessProxyVehicle();
	pkNetworkVehicle->updateBase( currentTime, elapsedTime );
}

//-------------------------------------------------------------------------
void ServerVehicleUpdate::update( const osScalar currentTime, const osScalar elapsedTime )
{

}


//-------------------------------------------------------------------------
void ServerVehicleUpdate::initGui( GLUI_Panel* parentPanel )
{
	assert( NULL != parentPanel );
	GLUI* glui = ::getRootGLUI();
	// general network plugin gui
	GLUI_Rollout* replicationRollout = glui->add_rollout_to_panel( parentPanel, "Entity Replication", true );	
	GLUI_Panel* replicationPanel = replicationRollout;
	glui->add_checkbox_to_panel( replicationPanel, "UpdateTicks", &SimpleNetworkVehicle::ms_bReplicationDataConfig[ESerializeDataType_UpdateTicks]);
	glui->add_separator_to_panel( replicationPanel );
	glui->add_checkbox_to_panel( replicationPanel, "Position", &SimpleNetworkVehicle::ms_bReplicationDataConfig[ESerializeDataType_Position]);
	glui->add_separator_to_panel( replicationPanel );
	glui->add_checkbox_to_panel( replicationPanel, "Forward", &SimpleNetworkVehicle::ms_bReplicationDataConfig[ESerializeDataType_Forward]);
	glui->add_checkbox_to_panel( replicationPanel, "Side", &SimpleNetworkVehicle::ms_bReplicationDataConfig[ESerializeDataType_Side]);
	glui->add_checkbox_to_panel( replicationPanel, "Up", &SimpleNetworkVehicle::ms_bReplicationDataConfig[ESerializeDataType_Up]);
	glui->add_checkbox_to_panel( replicationPanel, "Orientation", &SimpleNetworkVehicle::ms_bReplicationDataConfig[ESerializeDataType_Orientation]);
	glui->add_checkbox_to_panel( replicationPanel, "CompressedOrientation1", &SimpleNetworkVehicle::ms_bReplicationDataConfig[ESerializeDataType_CompressedOrientation1]);
	glui->add_checkbox_to_panel( replicationPanel, "CompressedOrientation2", &SimpleNetworkVehicle::ms_bReplicationDataConfig[ESerializeDataType_CompressedOrientation2]);
	glui->add_separator_to_panel( replicationPanel );
	glui->add_checkbox_to_panel( replicationPanel, "AngularVelocity", &SimpleNetworkVehicle::ms_bReplicationDataConfig[ESerializeDataType_AngularVelocity]);
	glui->add_checkbox_to_panel( replicationPanel, "LinearVelocity", &SimpleNetworkVehicle::ms_bReplicationDataConfig[ESerializeDataType_LinearVelocity]);
	glui->add_checkbox_to_panel( replicationPanel, "Speed", &SimpleNetworkVehicle::ms_bReplicationDataConfig[ESerializeDataType_Speed]);
	glui->add_separator_to_panel( replicationPanel );
	glui->add_checkbox_to_panel( replicationPanel, "Force", &SimpleNetworkVehicle::ms_bReplicationDataConfig[ESerializeDataType_Force]);
	glui->add_checkbox_to_panel( replicationPanel, "CompressedForce", &SimpleNetworkVehicle::ms_bReplicationDataConfig[ESerializeDataType_CompressedForce]);
	glui->add_separator_to_panel( replicationPanel );
	glui->add_checkbox_to_panel( replicationPanel, "Radius", &SimpleNetworkVehicle::ms_bReplicationDataConfig[ESerializeDataType_Radius]);
}
