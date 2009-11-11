#ifndef __BOIDREPLICA_H__
#define __BOIDREPLICA_H__

#include "Boids.h"
#include "EduNetConnect/OSReplicaTypes.h"

//-----------------------------------------------------------------------------
class NetBoidReplica : public OSReplica<OpenSteer::Boid>
{
public:
	virtual RakNet::RakString GetName(void) const
	{
		return "NetBoidReplica";
	};

	NetBoidReplica( OpenSteer::ProximityDatabase& pd);

	NetBoidReplica( OpenSteer::BoidsPlugin* pBoidPlugin  );

	virtual void DeallocReplica(RakNet::Connection_RM3 *sourceConnection);

	virtual RakNet::RM3SerializationResult Serialize(
		RakNet::SerializeParameters *serializeParameters);
	
	virtual void Deserialize(
		RakNet::DeserializeParameters *deserializeParameters);
	
private:
	OpenSteer::BoidsPlugin* m_pBoidPlugin;
};

#endif // __BOIDREPLICA_H__