#ifndef __NETBOIDFACTORY_H__
#define __NETBOIDFACTORY_H__

#include "Boids.h"
#include "NetBoidReplicaConnection.h"

// ----------------------------------------------------------------------------
class NetBoidReplicaFactory : public OpenSteer::BoidFactory
{
public:
	NetBoidReplicaFactory(NetBoidReplicaManager* pkManager):
	  m_pkReplicaManager(pkManager){}

	virtual OpenSteer::Boid* CreateBoid( OpenSteer::ProximityDatabase& pd );
	virtual void DestroyBoid( const OpenSteer::Boid* );	

private:

	NetBoidReplicaManager* m_pkReplicaManager;
	DataStructures::Map<size_t, RakNet::Replica3* > m_uidMap;
};

// ----------------------------------------------------------------------------
class BoidDummyFactory : public NetBoidReplicaFactory
{
public:
	BoidDummyFactory(NetBoidReplicaManager* pkManager):
	  NetBoidReplicaFactory(pkManager){}

	virtual OpenSteer::Boid* CreateBoid( OpenSteer::ProximityDatabase& pd )
	{
		return NULL;
	};
	virtual void DestroyBoid( const OpenSteer::Boid* )
	{
	}	
};

#endif // __NETBOIDFACTORY_H__