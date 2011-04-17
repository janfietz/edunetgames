#include "EduNetConnectGuiReplicationSettings.h"
#include "EduNetConnect/EduNetConnect.h"

EduNetConnectGuiReplicationSettings::EduNetConnectGuiReplicationSettings( wxWindow* parent )
:
ReplicationSettings( parent ),
m_pReplicaManager(NULL)
{
	Enable(false);
}

void EduNetConnectGuiReplicationSettings::OnReplicationRateChanged( wxScrollEvent& event )
{
	int pos = event.GetPosition();
	RakNetTime interval = 1000 - pos; 
	m_pReplicaManager->SetAutoSerializeInterval( interval );
}

void EduNetConnectGuiReplicationSettings::OnReliabilityChanged( wxCommandEvent& event )
{
	int pos = event.GetInt();
	if ( RELIABLE_SEQUENCED >= pos )
	{
		m_pReplicaManager->SetDefaultPacketReliability( (PacketReliability)pos );
	}
}

void EduNetConnectGuiReplicationSettings::OnPriorityChanged( wxCommandEvent& event )
{
	int pos = event.GetInt();

	PacketPriority prio = LOW_PRIORITY;
	switch (pos)
	{
	case 1: prio = MEDIUM_PRIORITY; break;
	case 2: prio = HIGH_PRIORITY; break;
	default: prio = LOW_PRIORITY; break;
	}
	m_pReplicaManager->SetDefaultPacketPriority( prio );
}

void EduNetConnectGuiReplicationSettings::useReplicaManager( 
	RakNet::ReplicaManager3* pReplicaManager )
{
	m_pReplicaManager = pReplicaManager;
	if (m_pReplicaManager != NULL)
	{
		this->Enable( true );

		RakNet::PRO sendParams = pReplicaManager->GetDefaultSendParameters();

		int select;
		switch (sendParams.priority)
		{
		case LOW_PRIORITY: select = 0; break;
		case MEDIUM_PRIORITY: select = 1; break;
		case HIGH_PRIORITY: select = 2; break;
		default: select = 2; break;
		}
		this->m_ReplicationPriority->Select(select);
		this->m_ReplicationReliability->Select( sendParams.reliability );

		this->m_replicationRate->SetValue( 970 ); // default 30 Hz
	}
	else
	{
		this->Enable( false );
	}
}