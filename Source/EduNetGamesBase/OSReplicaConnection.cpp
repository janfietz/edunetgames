#include "OSReplicaConnection.h"
#include "opensteer/plugins/SamplePlugins.h"

//-----------------------------------------------------------------------------
 RakNet::Replica3* OSReplicaConnection::AllocReplica(
	RakNet::BitStream *allocationId,
	 RakNet::ReplicaManager3 *replicaManager3)
{
	RakNet::RakString typeName;
	allocationId->Read(typeName);
	if (typeName=="ReplicaBoid"){
		//return //0;
	}
	return 0;
}
