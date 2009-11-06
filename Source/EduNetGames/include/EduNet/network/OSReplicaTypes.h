#ifndef __OSREPLICATYPES_H__
#define __OSREPLICATYPES_H__

#include "OSReplica.h"

//-----------------------------------------------------------------------------
template< class OSType>
class OSReplica : public OSObjectReplica
{
public:
	OSReplica():m_pVehicle( NULL ){};
	virtual ~OSReplica()
	{
		delete m_pVehicle;
	};
	
	virtual RakNet::RM3ConstructionState QueryConstruction(
		RakNet::Connection_RM3 *destinationConnection,
		RakNet::ReplicaManager3 *replicaManager3)
	{
		return QueryConstruction_PeerToPeer(destinationConnection);
	}
	virtual bool QueryRemoteConstruction(RakNet::Connection_RM3 *sourceConnection)
	{
		return QueryRemoteConstruction_PeerToPeer(sourceConnection);
	}
	virtual RakNet::RM3QuerySerializationResult QuerySerialization(
		RakNet::Connection_RM3 *destinationConnection)
	{
		return QuerySerialization_PeerToPeer(destinationConnection);
	}
	virtual RakNet::RM3ActionOnPopConnection QueryActionOnPopConnection(
		RakNet::Connection_RM3 *droppedConnection) const
	{
		return QueryActionOnPopConnection_PeerToPeer(droppedConnection);
	}

	OSType* AccessVehicle( void )const
	{
		return this->m_pVehicle;
	};
protected:
	OSType* m_pVehicle;
};




#endif //__OSREPLICATYPES_H__
