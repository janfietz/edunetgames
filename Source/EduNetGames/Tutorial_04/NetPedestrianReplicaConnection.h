#ifndef __NETPEDESTRIANREPLICACONNECTION_H__
#define __NETPEDESTRIANREPLICACONNECTION_H__

#include "EduNetCommon/EduNetCommon.h"

#include "NetPedestrianFactory.h"
#include "NetPedestrianPlugin.h"


//-----------------------------------------------------------------------------
class NetPedestrianReplicaConnection : public RakNet::Connection_RM3
{
public:
	NetPedestrianReplicaConnection(SystemAddress _systemAddress, RakNetGUID _guid,
		class NetPedestrianPlugin* pNetPedestrianPlugin) :
	Connection_RM3(_systemAddress, _guid)
	{
		m_pNetPedestrianPlugin = pNetPedestrianPlugin;
	};
	virtual ~NetPedestrianReplicaConnection() {}

	virtual RakNet::Replica3 *AllocReplica(RakNet::BitStream *allocationId,
		RakNet::ReplicaManager3 *replicaManager3);
private:
	class NetPedestrianPlugin* m_pNetPedestrianPlugin;
};

//-----------------------------------------------------------------------------
class NetPedestrianReplicaManager : public RakNet::ReplicaManager3
{
	virtual RakNet::Connection_RM3* AllocConnection(SystemAddress systemAddress, RakNetGUID rakNetGUID) const 
	{
		return new NetPedestrianReplicaConnection(systemAddress,rakNetGUID, this->m_pNetPedestrianPlugin);
	}
	virtual void DeallocConnection(RakNet::Connection_RM3 *connection) const {
		delete connection;
	}	
public:
	void SetPlugin( class NetPedestrianPlugin* pPlugin )
	{
		this->m_pNetPedestrianPlugin = pPlugin;
	}

	OpenSteer::AbstractPlugin* getPlugin( void )
	{
		return this->m_pNetPedestrianPlugin;
	}

private:
	class NetPedestrianPlugin* m_pNetPedestrianPlugin;
};

//-----------------------------------------------------------------------------
class NetPedestrianReplicaFactory : public NetPedestrianFactory
{
	ET_DECLARE_BASE( NetPedestrianFactory );
public:
	NetPedestrianReplicaFactory(NetPedestrianReplicaManager* pkManager):
	  m_pkReplicaManager(pkManager){}

	  virtual OpenSteer::AbstractVehicle* createVehicle( OpenSteer::ProximityDatabase* pkProximityDatabase ) const;
	  virtual void destroyVehicle( OpenSteer::AbstractVehicle* pkVehicle ) const;

private:

	NetPedestrianReplicaManager* m_pkReplicaManager;
	mutable DataStructures::Map<OpenSteer::InstanceTracker::Id, RakNet::Replica3* > m_uidMap;
};


#endif // __NETPEDESTRIANREPLICACONNECTION_H__