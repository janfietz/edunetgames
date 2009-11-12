#include "NetBoidReplica.h"


//-----------------------------------------------------------------------------
NetBoidReplica::NetBoidReplica( OpenSteer::ProximityDatabase& pd):
	m_pBoidPlugin(NULL)
{
	this->m_pVehicle = new OpenSteer::Boid( pd );
	this->m_pVehicle->setIsRemoteObject(false);
}
//-----------------------------------------------------------------------------
NetBoidReplica::NetBoidReplica( OpenSteer::BoidsPlugin* pBoidPlugin  ):
	m_pBoidPlugin(pBoidPlugin)
{
	this->m_pVehicle = new OpenSteer::Boid( 
		*this->m_pBoidPlugin->accessProximityDataBase() );
	this->m_pVehicle->setIsRemoteObject(true);
}

//-----------------------------------------------------------------------------
void NetBoidReplica::DeallocReplica(RakNet::Connection_RM3 *sourceConnection)
{
	m_pBoidPlugin->RemoveBoidFromFlock(this->m_pVehicle);
	delete this;
}

//-----------------------------------------------------------------------------
RakNet::RM3SerializationResult NetBoidReplica::Serialize(
	RakNet::SerializeParameters *serializeParameters)
{
	RakNet::BitStream& kStream = serializeParameters->outputBitstream[0];

	kStream.WriteAlignedBytes((const unsigned char*)&m_pVehicle->position(),sizeof(OpenSteer::Vec3));

	kStream.WriteAlignedBytes((const unsigned char*)&m_pVehicle->forward(),sizeof(OpenSteer::Vec3));


	return RakNet::RM3SR_BROADCAST_IDENTICALLY;
}

//-----------------------------------------------------------------------------
void NetBoidReplica::Deserialize(
	RakNet::DeserializeParameters *deserializeParameters)
{
	RakNet::BitStream& kStream = deserializeParameters->serializationBitstream[0];

	OpenSteer::Vec3 kVec;
	kStream.ReadAlignedBytes((unsigned char*)&kVec,sizeof(kVec));
	m_pVehicle->setPosition(kVec);

	kStream.ReadAlignedBytes((unsigned char*)&kVec,sizeof(kVec));
	m_pVehicle->setForward(kVec);
}