#ifndef __EduNetConnectGuiReplicationSettings__
#define __EduNetConnectGuiReplicationSettings__

/**
@file
Subclass of ReplicationSettings, which is generated by wxFormBuilder.
*/

#include "EduNetConnectGui.h"

//// end generated include

namespace RakNet
{
	class ReplicaManager3;
}

/** Implementing ReplicationSettings */
class EduNetConnectGuiReplicationSettings : public ReplicationSettings
{
	protected:
		// Handlers for ReplicationSettings events.
		void OnReplicationRateChanged( wxScrollEvent& event );
		void OnReliabilityChanged( wxCommandEvent& event );
		void OnPriorityChanged( wxCommandEvent& event );
	public:
		/** Constructor */
		EduNetConnectGuiReplicationSettings( wxWindow* parent );
	//// end generated class members

		void useReplicaManager( RakNet::ReplicaManager3* pReplicaManager );
private:

	RakNet::ReplicaManager3* m_pReplicaManager;
	
};

#endif // __EduNetConnectGuiReplicationSettings__
