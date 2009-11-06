#ifndef __BOIDREPLICA_H__
#define __BOIDREPLICA_H__

#include "EduNet/network/OSReplicaTypes.h"

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


	virtual RakNet::RM3SerializationResult Serialize(RakNet::SerializeParameters *serializeParameters)
	{
		RakNet::BitStream& kStream = serializeParameters->outputBitstream[0];

		kStream.WriteAlignedBytes((const unsigned char*)&m_pVehicle->position(),sizeof(OpenSteer::Vec3));

		kStream.WriteAlignedBytes((const unsigned char*)&m_pVehicle->forward(),sizeof(OpenSteer::Vec3));


		return RakNet::RM3SR_BROADCAST_IDENTICALLY;
	}

	//-----------------------------------------------------------------------------
	virtual void Deserialize(RakNet::DeserializeParameters *deserializeParameters)
	{
		RakNet::BitStream& kStream = deserializeParameters->serializationBitstream[0];

		OpenSteer::Vec3 kVec;
		kStream.ReadAlignedBytes((unsigned char*)&kVec,sizeof(kVec));
		m_pVehicle->setPosition(kVec);

		kStream.ReadAlignedBytes((unsigned char*)&kVec,sizeof(kVec));
		m_pVehicle->setForward(kVec);	

	}

private:
	OpenSteer::BoidsPlugIn* m_pBoidPlugin;
};

#endif // __BOIDREPLICA_H__