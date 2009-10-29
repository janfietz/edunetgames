#pragma once
#include "Tutorial_02/plugins/ClientPlugin.h"
#include "Tutorial_02/plugins/PeerPlugin.h"
#include "EduNet/network/OSReplica.h"
#include "Boids.h"


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
class BoidDummyFactory : public BoidReplicaFactory
{
public:
	BoidDummyFactory(BoidReplicaManager* pkManager):
	  BoidReplicaFactory(pkManager){}

	virtual OpenSteer::Boid* CreateBoid( OpenSteer::ProximityDatabase& pd )
	{
		return NULL;
	};
	virtual void DestroyBoid( const OpenSteer::Boid* )
	{
	}	
};

//-----------------------------------------------------------------------------
class BoidConditionReplica : public OSObjectReplica
{
public:
	virtual RakNet::RakString GetName(void) const
	{
		return "BoidCondition";
	};

	BoidConditionReplica(OpenSteer::BoidsPlugIn* pBoidPlugin):
		m_pBoidPlugin(pBoidPlugin){}

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
		RakNet::Connection_RM3 *destinationConnection);


	virtual RakNet::RM3ActionOnPopConnection QueryActionOnPopConnection(
		RakNet::Connection_RM3 *droppedConnection) const
	{
		return QueryActionOnPopConnection_PeerToPeer(droppedConnection);
	}
	virtual void SerializeConstructionExisting(
		RakNet::BitStream *constructionBitstream, 
		RakNet::Connection_RM3 *destinationConnection);

	virtual void DeserializeConstructionExisting(
		RakNet::BitStream *constructionBitstream,
		RakNet::Connection_RM3 *sourceConnection) ;

	virtual RakNet::RM3SerializationResult Serialize(
		RakNet::SerializeParameters *serializeParameters);

	virtual void Deserialize(
		RakNet::DeserializeParameters *deserializeParameters);

	virtual void SerializeConstructionRequestAccepted(
		RakNet::BitStream *serializationBitstream,
		RakNet::Connection_RM3 *requestingConnection);
	virtual void DeserializeConstructionRequestAccepted(
		RakNet::BitStream *serializationBitstream,
		RakNet::Connection_RM3 *acceptingConnection);


protected:
	OpenSteer::BoidsPlugIn* m_pBoidPlugin;
};




// ----------------------------------------------------------------------------
class NetPeerBoidPlugin : public PeerPlugin<OpenSteer::BoidsPlugIn>
{
	ET_DECLARE_BASE(PeerPlugin<OpenSteer::BoidsPlugIn>);
public:
	NetPeerBoidPlugin();
	virtual ~NetPeerBoidPlugin(){};

	virtual const char* name (void){return "NetPeerBoidPlugin";};

	 bool requestInitialSelection (void) {return true;}
	 virtual float selectionOrderSortKey (void) { return 2.0f ;}
	 
	virtual bool needRedraw ( void ) const { return false; }
	


	virtual void StartNetworkSession( void );
	virtual void CreateContent( void );
	virtual void DeleteContent( void );

private:
	BoidConditionReplica* m_pkConditionReplic;
	BoidReplicaFactory* m_pkBoidFactory;
	BoidReplicaManager m_kReplicaManager;
};

// ----------------------------------------------------------------------------
class NetClientBoidPlugin : public ClientPlugin<OpenSteer::BoidsPlugIn>
{
	ET_DECLARE_BASE(ClientPlugin<OpenSteer::BoidsPlugIn>);
public:
	NetClientBoidPlugin();
	virtual ~NetClientBoidPlugin(){};

	virtual const char* name (void){return "NetClientBoidPlugin";};

	virtual float selectionOrderSortKey (void) { return 3.0f ;}

	virtual void StartNetworkSession( void );
	virtual void CreateContent( void );
	virtual void DeleteContent( void );

private:

	BoidReplicaFactory* m_pkBoidFactory;
	BoidReplicaManager m_kReplicaManager;
	BoidConditionReplica* m_pkConditionReplic;

};