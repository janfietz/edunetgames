#include "SimpleNetworkVehicle.h"


using namespace OpenSteer;

//-------------------------------------------------------------------------
#pragma warning(push)
#pragma warning(disable: 4355) // warning C4355: 'this' : used in base member initializer list
SimpleNetworkVehicle::SimpleNetworkVehicle()
{ 
}
#pragma warning(pop)

//-----------------------------------------------------------------------------
SimpleNetworkVehicle::~SimpleNetworkVehicle() 
{ 
}

//-----------------------------------------------------------------------------
int SimpleNetworkVehicle::serialize( RakNet::SerializeParameters *serializeParameters ) const
{
	RakNet::BitStream& kStream = serializeParameters->outputBitstream[0];

	kStream.WriteAlignedBytes((const unsigned char*)&this->position(),sizeof(OpenSteer::Vec3));
	kStream.WriteAlignedBytes((const unsigned char*)&this->forward(),sizeof(OpenSteer::Vec3));
	kStream.WriteAlignedBytes((const unsigned char*)&this->lastSteeringForce(),sizeof(OpenSteer::Vec3));

	return RakNet::RM3SR_BROADCAST_IDENTICALLY;
}

//-----------------------------------------------------------------------------
void SimpleNetworkVehicle::deserialize( RakNet::DeserializeParameters *deserializeParameters )
{
	RakNet::BitStream& kStream = deserializeParameters->serializationBitstream[0];

	OpenSteer::Vec3 kVec;
	kStream.ReadAlignedBytes((unsigned char*)&kVec,sizeof(kVec));
	this->setPosition(kVec);

	kStream.ReadAlignedBytes((unsigned char*)&kVec,sizeof(kVec));
	this->setForward(kVec);	
	this->regenerateOrthonormalBasisUF( kVec );			

	kStream.ReadAlignedBytes((unsigned char*)&kVec,sizeof(kVec));
	this->setLastSteeringForce(kVec);	
}

