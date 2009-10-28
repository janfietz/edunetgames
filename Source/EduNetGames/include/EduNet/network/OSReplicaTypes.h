#ifndef OSREPLICATYPES
#define OSREPLICATYPES

#include "OSReplica.h"
#include "Tutorial_03/plugins/Boids.h"

//-----------------------------------------------------------------------------
template< class OSType>
class OSReplica : public OSObjectReplica
{
public:
	OSReplica(){};
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

	virtual RakNet::RM3SerializationResult Serialize(RakNet::SerializeParameters *serializeParameters);
	virtual void Deserialize(RakNet::DeserializeParameters *deserializeParameters);

	OSType* AccessVehicle( void )const
	{
		return this->m_pVehicle;
	};
protected:
	OSType* m_pVehicle;
};

template< class OSType>
RakNet::RM3SerializationResult OSReplica<OSType>::Serialize(RakNet::SerializeParameters *serializeParameters)
{
	RakNet::BitStream& kStream = serializeParameters->outputBitstream[0];

	OpenSteer::Vec3 kVec = m_pVehicle->position();
	kStream.WriteAlignedBytes((const unsigned char*)&kVec,sizeof(kVec));
	
	kVec = m_pVehicle->forward();
	kStream.WriteAlignedBytes((const unsigned char*)&kVec,sizeof(kVec));

	
	return RakNet::RM3SR_BROADCAST_IDENTICALLY;
}

//-----------------------------------------------------------------------------
template< class OSType>
void OSReplica<OSType>::Deserialize(RakNet::DeserializeParameters *deserializeParameters)
{
	RakNet::BitStream& kStream = deserializeParameters->serializationBitstream[0];

	OpenSteer::Vec3 kVec;
	kStream.ReadAlignedBytes((unsigned char*)&kVec,sizeof(kVec));
	m_pVehicle->setPosition(kVec);
	
	kStream.ReadAlignedBytes((unsigned char*)&kVec,sizeof(kVec));
	m_pVehicle->setForward(kVec);	
	
}

//-----------------------------------------------------------------------------
class BoidReplica : public OSReplica<OpenSteer::Boid>
{
public:
	virtual RakNet::RakString GetName(void) const
	{
		return "BoidReplica";
	};

	BoidReplica( OpenSteer::ProximityDatabase& pd):
		m_pBoidPlugin(NULL)
	{
		this->m_pVehicle = new OpenSteer::Boid( pd );
		this->m_pVehicle->setIsRemoteObject(false);
	};

	BoidReplica( OpenSteer::BoidsPlugIn* pBoidPlugin  ):
		m_pBoidPlugin(pBoidPlugin)
	{
		this->m_pVehicle = new OpenSteer::Boid( 
			*this->m_pBoidPlugin->AccessProximityDataBase() );
		this->m_pVehicle->setIsRemoteObject(true);
	};

	virtual void DeallocReplica(RakNet::Connection_RM3 *sourceConnection)
	{
		m_pBoidPlugin->RemoveBoidFromFlock(this->m_pVehicle);
		delete this;
	}

private:
	OpenSteer::BoidsPlugIn* m_pBoidPlugin;
};

#endif //OSREPLICATYPES
