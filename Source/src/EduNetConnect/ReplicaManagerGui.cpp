//-----------------------------------------------------------------------------
// Copyright (c) 2011, Jan Fietz, Cyrus Preuss
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

#include "ReplicaManagerGui.h"
#include "EduNetCommon/EduNetDraw.h"

//-----------------------------------------------------------------------------
ReplicaManagerGui::ReplicaManagerGui( void ):
	m_pkReplicaManager( NULL )
{
	RakNet::PRO defaultSendParameter;
	// and change to UNRELIABLE
	defaultSendParameter.reliability = UNRELIABLE;
	// and change to low priority
	defaultSendParameter.priority = LOW_PRIORITY;

	this->m_kReplicationParams.sendParameter = defaultSendParameter;

}

//-----------------------------------------------------------------------------
ReplicaManagerGui::~ReplicaManagerGui( void )
{

}

//-----------------------------------------------------------------------------
void ReplicaManagerGui::initGui( void* pkUserdata )
{
	this->addReplicaManagerGui( pkUserdata );
}

//-----------------------------------------------------------------------------
void ReplicaManagerGui::incrementReplicationInterval( int additionalTime )
{
	RakNetTime _additionalTime(0);
	ReplicationParams& kReplicationParams = this->accessReplicationParams();
	if( additionalTime < 0 )
	{
		// reverse sign
		additionalTime *= -1;
		const int currentInterval = static_cast<int>(kReplicationParams.interval);
		additionalTime = ::etMin( additionalTime, currentInterval );
		kReplicationParams.interval -= additionalTime;
	}
	else
	{
		kReplicationParams.interval += additionalTime;
	}
	// clamp interval
	if( kReplicationParams.interval < 5 )
	{
		kReplicationParams.interval = 5;
	}
	printf("Changed replication interval to: %d ms\n",
		kReplicationParams.interval);
	this->onChangedReplicationParams( kReplicationParams );
}

//-----------------------------------------------------------------------------
void ReplicaManagerGui::onChangedReplicationParams( const ReplicationParams& kParams )
{
	if( NULL == this->m_pkReplicaManager )
	{
		return;
	}
	RakNet::ReplicaManager3& kReplicaManager = *this->m_pkReplicaManager;
	kReplicaManager.SetAutoSerializeInterval(
		kParams.interval);
	kReplicaManager.SetDefaultPacketReliability(
		kParams.sendParameter.reliability);
	kReplicaManager.SetDefaultPacketPriority(
		kParams.sendParameter.priority);
}

//-----------------------------------------------------------------------------
void ReplicaManagerGui::retrieveReplicaManagerDefaultSendParams( 
	class RakNet::ReplicaManager3* pkReplicaManager )
{
	if( NULL == pkReplicaManager )
	{
		return;
	}

	RakNet::PRO sendParameter = this->m_kReplicationParams.sendParameter;
	this->m_kReplicationParams.sendParameter = 
		pkReplicaManager->GetDefaultSendParameters();
	// fix parameter right now - TODO add user setting ?
	sendParameter.orderingChannel = this->m_kReplicationParams.sendParameter.orderingChannel;

	// apply defaults
	this->m_kReplicationParams.sendParameter = sendParameter;

	this->onChangedReplicationParams( this->m_kReplicationParams );
}

//-----------------------------------------------------------------------------
void ReplicaManagerGui::setReplicaManager( RakNet::ReplicaManager3* pkReplicaManager )
{
	this->m_pkReplicaManager = pkReplicaManager;
	this->retrieveReplicaManagerDefaultSendParams( this->m_pkReplicaManager );
}

//-----------------------------------------------------------------------------
void ReplicaManagerGui::addReplicaManagerGui( void* pkUserdata )
{
	GLUI* glui = ::getRootGLUI();
	GLUI_Panel* pluginPanel = static_cast<GLUI_Panel*>( pkUserdata );
	GLUI_Panel* replicaPanel = glui->add_rollout_to_panel( pluginPanel,
		"Replication Settings", false );


	GLUI_EditText* repSpinner =
		glui->add_edittext_to_panel(replicaPanel, "Replication FPS",
		GLUI_EDITTEXT_FLOAT, NULL, -1, changeReplicationDelay);
	repSpinner->set_float_limits( 1.0f, 50.0f );
	repSpinner->set_float_val(this->m_kReplicationParams.fReplicationFrameRate);
	repSpinner->set_ptr_val( this );

	GLUI_Listbox* pluginList = 
		glui->add_listbox_to_panel( replicaPanel, "Default Reliability", NULL, 1, ReplicaManagerGui::changeReplicationSendParams );
	pluginList->add_item( UNRELIABLE, "Unreliable" );
	pluginList->add_item( UNRELIABLE_SEQUENCED, "Unreliable Sequenced" );
	pluginList->add_item( RELIABLE, "Reliable" );
	pluginList->add_item( RELIABLE_ORDERED, "Reliable Ordered" );
	pluginList->add_item( RELIABLE_SEQUENCED, "Reliable Sequenced" );
	pluginList->do_selection( this->m_kReplicationParams.sendParameter.reliability );
	pluginList->set_ptr_val( this );
//	pluginList->do_selection( UNRELIABLE );

	pluginList = 
		glui->add_listbox_to_panel( replicaPanel,"Default Priority", NULL, 2, ReplicaManagerGui::changeReplicationSendParams );
	pluginList->add_item( HIGH_PRIORITY, "High" );
	pluginList->add_item( MEDIUM_PRIORITY, "Medium" );
	pluginList->add_item( LOW_PRIORITY, "Low" );
	pluginList->do_selection( this->m_kReplicationParams.sendParameter.priority );
	pluginList->set_ptr_val( this );
}


//-----------------------------------------------------------------------------
void ReplicaManagerGui::changeReplicationDelay( GLUI_Control* pkControl )
{
	ReplicaManagerGui* pkPlugin = (ReplicaManagerGui*)pkControl->ptr_val;
	if (NULL == pkPlugin)
	{
		return;
	}
	ReplicationParams& kReplicationParams = pkPlugin->accessReplicationParams();	
	kReplicationParams.fReplicationFrameRate = pkControl->get_float_val();
	// make sure this value is above zero
	kReplicationParams.fReplicationFrameRate = ::etMax( 1.0f, kReplicationParams.fReplicationFrameRate );
	kReplicationParams.interval = static_cast<RakNetTime>(1000.0f / kReplicationParams.fReplicationFrameRate);
	pkPlugin->onChangedReplicationParams(kReplicationParams);
}

//-----------------------------------------------------------------------------
void ReplicaManagerGui::changeReplicationSendParams( GLUI_Control* pkControl )
{
	ReplicaManagerGui* pkPlugin = (ReplicaManagerGui*)pkControl->ptr_val;
	if (NULL == pkPlugin)
	{
		return;
	}
	ReplicationParams& kReplicationParams = pkPlugin->accessReplicationParams();

	GLUI_Listbox* pkList = (GLUI_Listbox*)pkControl;
	GLUI_Listbox_Item* pkItem = pkList->get_item_ptr( pkList->curr_text.c_str() );
	switch( pkControl->get_id() )
	{
	case 1: 
		{
			kReplicationParams.sendParameter.reliability = (PacketReliability)pkItem->id;
		}break;
	case 2: 
		{
			kReplicationParams.sendParameter.priority = (PacketPriority)pkItem->id;
		}break;
	}

	pkPlugin->onChangedReplicationParams(kReplicationParams);
}
