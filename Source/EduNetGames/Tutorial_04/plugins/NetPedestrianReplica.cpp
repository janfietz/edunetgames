#include "NetPedestrianReplica.h"
#include "NetPedestrian.h"
#include "OpenSteerExtras/AbstractVehicleGroup.h"

using namespace OpenSteer;

//-----------------------------------------------------------------------------
NetPedestrianReplica::NetPedestrianReplica():m_pkHostPlugin(NULL)
{

}

//-----------------------------------------------------------------------------
NetPedestrianReplica::NetPedestrianReplica( OpenSteer::ProximityDatabase& pd):
m_pkHostPlugin( NULL )
{
	this->m_pVehicle = new NetPedestrian( pd );
	this->m_pVehicle->setNetworkId( this->GetNetworkID().guid.g );
	this->m_pVehicle->setIsRemoteObject(false);
};

//-----------------------------------------------------------------------------
NetPedestrianReplica::NetPedestrianReplica( OpenSteer::AbstractPlugin* pkHostPlugin  ):
m_pkHostPlugin(pkHostPlugin)
{
	this->m_pVehicle = new NetPedestrian( 
		*this->m_pkHostPlugin->accessProximityDataBase() );
	this->m_pVehicle->setNetworkId( this->GetNetworkID().guid.g );
	this->m_pVehicle->setIsRemoteObject(true);
};

//-----------------------------------------------------------------------------
RakNet::RakString NetPedestrianReplica::GetName(void) const
{
	static NetPedestrian kProtoType;
	return kProtoType.getClassName();
}

//-----------------------------------------------------------------------------
void NetPedestrianReplica::DeallocReplica(RakNet::Connection_RM3 *sourceConnection)
{
	AbstractVehicleGroup kVG( m_pkHostPlugin->allVehicles() );
	kVG.removeVehicle( this->m_pVehicle );
	delete this;
}

//-----------------------------------------------------------------------------
RakNet::RM3SerializationResult NetPedestrianReplica::Serialize(RakNet::SerializeParameters *serializeParameters)
{
	RakNet::BitStream& kStream = serializeParameters->outputBitstream[0];

	kStream.WriteAlignedBytes((const unsigned char*)&m_pVehicle->position(),sizeof(OpenSteer::Vec3));
	kStream.WriteAlignedBytes((const unsigned char*)&m_pVehicle->forward(),sizeof(OpenSteer::Vec3));

	return RakNet::RM3SR_BROADCAST_IDENTICALLY;
}

//-----------------------------------------------------------------------------
void NetPedestrianReplica::Deserialize(RakNet::DeserializeParameters *deserializeParameters)
{
	RakNet::BitStream& kStream = deserializeParameters->serializationBitstream[0];

	OpenSteer::Vec3 kVec;
	kStream.ReadAlignedBytes((unsigned char*)&kVec,sizeof(kVec));
	m_pVehicle->setPosition(kVec);

	kStream.ReadAlignedBytes((unsigned char*)&kVec,sizeof(kVec));
	m_pVehicle->setForward(kVec);	
}

