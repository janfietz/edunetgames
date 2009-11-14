#ifndef __NETBOIDREPLICACONNECTION_H__
#define __NETBOIDREPLICACONNECTION_H__

#include "BoidsPlugin.h"

// ----------------------------------------------------------------------------
class NetBoidReplicaConnection : public RakNet::Connection_RM3
{
public:
	NetBoidReplicaConnection(SystemAddress _systemAddress, RakNetGUID _guid,
		OpenSteer::BoidsPlugin* pBoidPlugin);
	virtual ~NetBoidReplicaConnection() {}
	
	virtual RakNet::Replica3 *AllocReplica(RakNet::BitStream *allocationId,
		RakNet::ReplicaManager3 *replicaManager3);
private:
	OpenSteer::BoidsPlugin* m_pBoidPlugin;
};

// ----------------------------------------------------------------------------
class NetBoidReplicaManager : public RakNet::ReplicaManager3
{
	virtual RakNet::Connection_RM3* AllocConnection(SystemAddress systemAddress, RakNetGUID rakNetGUID) const 
	{
		return new NetBoidReplicaConnection(systemAddress,rakNetGUID, this->m_pBoidPlugin);
	}
	virtual void DeallocConnection(RakNet::Connection_RM3 *connection) const {
		delete connection;
	}	
public:
	void SetPlugin(OpenSteer::BoidsPlugin* pPlugin)
	{
		this->m_pBoidPlugin = pPlugin;
	}

private:
	OpenSteer::BoidsPlugin* m_pBoidPlugin;
};

#endif // __NETBOIDREPLICACONNECTION_H__