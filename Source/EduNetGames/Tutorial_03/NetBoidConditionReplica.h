#ifndef __NETBOIDCONDITIONREPLCIA_H__
#define __NETBOIDCONDITIONREPLCIA_H__

#include "EduNetConnect/OSReplica.h"
#include "BoidsPlugin.h"

//-----------------------------------------------------------------------------
class NetBoidConditionReplica : public OSObjectReplica
{
public:
	virtual RakNet::RakString GetName(void) const
	{
		return "BoidCondition";
	};

	NetBoidConditionReplica(OpenSteer::BoidsPlugin* pBoidPlugin = NULL):
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
	OpenSteer::BoidsPlugin* m_pBoidPlugin;
};

#endif // __NETBOIDCONDITIONREPLCIA_H__