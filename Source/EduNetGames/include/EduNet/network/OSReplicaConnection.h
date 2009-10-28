#ifndef OSREPLICACONNECTION
#define OSREPLICACONNECTION

#include "EduNet/common/EduNetCommon.h"

class OSReplicaConnection : public RakNet::Connection_RM3 {
public:
	OSReplicaConnection(SystemAddress _systemAddress, RakNetGUID _guid) :
	   RakNet::Connection_RM3(_systemAddress, _guid) {}
	virtual ~OSReplicaConnection() {}

	virtual  RakNet::Replica3 *AllocReplica(RakNet::BitStream *allocationId,
		 RakNet::ReplicaManager3 *replicaManager3);
};

#endif //OSREPLICACONNECTION