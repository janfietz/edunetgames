#ifndef REPLICAMANAGER
#define REPLICAMANAGER

#include "EduNetCommon/EduNetCommon.h"

template< class ReplicaConnectionClass >
class OSReplicaManager : public RakNet::ReplicaManager3
{
	virtual RakNet::Connection_RM3* AllocConnection(SystemAddress systemAddress, RakNetGUID rakNetGUID) const {
		return new ReplicaConnectionClass(systemAddress,rakNetGUID);
	}
	virtual void DeallocConnection(RakNet::Connection_RM3 *connection) const {
		delete connection;
	}	
};


#endif //REPLICAMANAGER