#ifndef __NETPEDESTRIANREPLICA_H__
#define __NETPEDESTRIANREPLICA_H__

#include "EduNet/network/OSReplicaTypes.h"

//-----------------------------------------------------------------------------
class NetPedestrianReplica : public OSReplica<class NetPedestrian>
{
	ET_DECLARE_BASE( NetPedestrianReplica )
public:
	NetPedestrianReplica():m_pkHostPlugin(NULL){}
	NetPedestrianReplica( OpenSteer::ProximityDatabase& pd );

	NetPedestrianReplica( OpenSteer::AbstractPlugin* pPlugin );

	//-------------------------------------------------------------------------
	// replica interface
	virtual RakNet::RakString GetName(void) const;

	virtual void DeallocReplica( RakNet::Connection_RM3 *sourceConnection );

	virtual RakNet::RM3SerializationResult Serialize( RakNet::SerializeParameters *serializeParameters );

	virtual void Deserialize(RakNet::DeserializeParameters *deserializeParameters);

private:
	OpenSteer::AbstractPlugin* m_pkHostPlugin;

	ET_IMPLEMENT_CLASS_NO_COPY( NetPedestrianReplica );
};


#endif //  __NETPEDESTRIANREPLICA_H__