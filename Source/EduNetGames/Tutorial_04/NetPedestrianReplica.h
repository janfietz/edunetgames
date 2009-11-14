#ifndef __NETPEDESTRIANREPLICA_H__
#define __NETPEDESTRIANREPLICA_H__

#include "EduNetConnect/OSReplicaTypes.h"

//-----------------------------------------------------------------------------
class NetPedestrianReplica : public OSReplica< OpenSteer::AbstractVehicle >
{
	ET_DECLARE_BASE( OSReplica< OpenSteer::AbstractVehicle > )
public:
	NetPedestrianReplica();
	NetPedestrianReplica( OpenSteer::AbstractPlugin* pPlugin, bool bIsRemoteObject );

	//-------------------------------------------------------------------------
	// replica interface
	virtual RakNet::RakString GetName(void) const;

	virtual void SetNetworkID( NetworkID id );

	virtual void DeallocReplica( RakNet::Connection_RM3 *sourceConnection );

	virtual RakNet::RM3SerializationResult Serialize( RakNet::SerializeParameters *serializeParameters );

	virtual void Deserialize(RakNet::DeserializeParameters *deserializeParameters);

private:
	OpenSteer::AbstractPlugin* m_pkHostPlugin;

	ET_IMPLEMENT_CLASS_NO_COPY( NetPedestrianReplica );
};


#endif //  __NETPEDESTRIANREPLICA_H__