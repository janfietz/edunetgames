#pragma once
#include "PeerPlugin.h"
#include "ClientPlugin.h"
#include "opensteer/plugins/SamplePlugins.h"
#include "ReplicaManager.h"

// ----------------------------------------------------------------------------
class BoidReplicaConnection : public RakNet::Connection_RM3
{
public:
	BoidReplicaConnection(SystemAddress _systemAddress, RakNetGUID _guid,
		OpenSteer::BoidsPlugIn* pBoidPlugin) :
		Connection_RM3(_systemAddress, _guid)
	{
		m_pBoidPlugin = pBoidPlugin;
	};
	virtual ~BoidReplicaConnection() {}
	
	virtual RakNet::Replica3 *AllocReplica(RakNet::BitStream *allocationId,
		RakNet::ReplicaManager3 *replicaManager3);
private:
	OpenSteer::BoidsPlugIn* m_pBoidPlugin;
};

// ----------------------------------------------------------------------------
class BoidReplicaManager : public RakNet::ReplicaManager3
{
	virtual RakNet::Connection_RM3* AllocConnection(SystemAddress systemAddress, RakNetGUID rakNetGUID) const 
	{
		return new BoidReplicaConnection(systemAddress,rakNetGUID, this->m_pBoidPlugin);
	}
	virtual void DeallocConnection(RakNet::Connection_RM3 *connection) const {
		delete connection;
	}	
public:
	void SetPlugin(OpenSteer::BoidsPlugIn* pPlugin)
	{
		this->m_pBoidPlugin = pPlugin;
	}

private:
	OpenSteer::BoidsPlugIn* m_pBoidPlugin;
};
// ----------------------------------------------------------------------------
class BoidReplicaFactory : public OpenSteer::BoidFactory
{
public:
	BoidReplicaFactory(BoidReplicaManager* pkManager):
	  m_pkReplicaManager(pkManager){}

	virtual OpenSteer::Boid* CreateBoid( OpenSteer::ProximityDatabase& pd );
	virtual void DestroyBoid( const OpenSteer::Boid* );
	

private:	
	BoidReplicaManager* m_pkReplicaManager;
	DataStructures::Map<unsigned int, RakNet::Replica3* > m_uidMap;
};

// ----------------------------------------------------------------------------
class NetPeerBoidPlugin : public PeerPlugin<OpenSteer::BoidsPlugIn>
{
public:
	NetPeerBoidPlugin();
	virtual ~NetPeerBoidPlugin(){};

	virtual const char* name (void){return "NetPeerBoidPlugin";};

	 bool requestInitialSelection (void) {return true;}
	 virtual float selectionOrderSortKey (void) { return 2.0f ;}

	 virtual void StartNetworkSession( void );

private:

	BoidReplicaFactory* m_pkBoidFactory;
	BoidReplicaManager m_kReplicaManager;
};

// ----------------------------------------------------------------------------
class NetClientBoidPlugin : public ClientPlugin<OpenSteer::BoidsPlugIn>
{
public:
	NetClientBoidPlugin(){};
	virtual ~NetClientBoidPlugin(){};

	virtual const char* name (void){return "NetClientBoidPlugin";};

	virtual float selectionOrderSortKey (void) { return 3.0f ;}
};