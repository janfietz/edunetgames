#ifndef __NETPEDESTRIANREPLICACONNECTION_H__
#define __NETPEDESTRIANREPLICACONNECTION_H__

#include "EduNet/common/EduNetCommon.h"

#include "NetPedestrianFactory.h"


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

private:
	class NetPedestrianPlugin* m_pNetPedestrianPlugin;
};
//-----------------------------------------------------------------------------
class NetPedestrianReplicaFactory : public NetPedestrianFactory
{
public:
	NetPedestrianReplicaFactory(NetPedestrianReplicaManager* pkManager):
	  m_pkReplicaManager(pkManager){}

	  virtual class NetPedestrian* CreateNetPedestrian( OpenSteer::ProximityDatabase& pd );
	  virtual void DestroyNetPedestrian( const class NetPedestrian* );	

private:

	NetPedestrianReplicaManager* m_pkReplicaManager;
	DataStructures::Map<size_t, RakNet::Replica3* > m_uidMap;
};

//-----------------------------------------------------------------------------
class NetPedestrianDummyFactory : public NetPedestrianReplicaFactory
{
public:
	NetPedestrianDummyFactory(NetPedestrianReplicaManager* pkManager):
	  NetPedestrianReplicaFactory(pkManager){}

	  virtual class NetPedestrian* CreateNetPedestrian( OpenSteer::ProximityDatabase& pd )
	  {
		  return NULL;
	  };
	  virtual void DestroyNetPedestrian( const class NetPedestrian* )
	  {
	  }	
};


#endif // __NETPEDESTRIANREPLICACONNECTION_H__