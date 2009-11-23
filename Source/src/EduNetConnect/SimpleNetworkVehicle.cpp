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

	unsigned char dataTypes = 3; // how many variables will be send
	unsigned char dataType;

	kStream.WriteAlignedBytes(&dataTypes,sizeof(unsigned char));

	dataType = ESerializeDataType_Position;
	kStream.WriteAlignedBytes(&dataType,sizeof(unsigned char));
	kStream.WriteAlignedBytes((const unsigned char*)&this->position(),sizeof(OpenSteer::Vec3));

	dataType = ESerializeDataType_Forward;
	kStream.WriteAlignedBytes(&dataType,sizeof(unsigned char));
	kStream.WriteAlignedBytes((const unsigned char*)&this->forward(),sizeof(OpenSteer::Vec3));

	dataType = ESerializeDataType_Force;
	kStream.WriteAlignedBytes(&dataType,sizeof(unsigned char));
	kStream.WriteAlignedBytes((const unsigned char*)&this->lastSteeringForce(),sizeof(OpenSteer::Vec3));

	return RakNet::RM3SR_BROADCAST_IDENTICALLY;
}

//-----------------------------------------------------------------------------
void SimpleNetworkVehicle::deserialize( RakNet::DeserializeParameters *deserializeParameters )
{
	RakNet::BitStream& kStream = deserializeParameters->serializationBitstream[0];

	OpenSteer::Vec3 kVec;
	unsigned char dataTypes;
	kStream.ReadAlignedBytes(&dataTypes,sizeof(unsigned char));
	for( unsigned char i = 0; i < dataTypes; ++i )
	{
		unsigned char dataType;
		kStream.ReadAlignedBytes(&dataType,sizeof(unsigned char));
		switch( dataType )
		{
		case(ESerializeDataType_Position):
		case(ESerializeDataType_Forward):
		case(ESerializeDataType_Side):
		case(ESerializeDataType_Up):
		case(ESerializeDataType_Force):
			{
				kStream.ReadAlignedBytes((unsigned char*)&kVec,sizeof(kVec));
			}
			break;
		}

		switch( dataType )
		{
		case(ESerializeDataType_Position):
			{
				this->setPosition(kVec);
			}
			break;
		case(ESerializeDataType_Forward):
			{
				this->setForward(kVec);	
				this->regenerateLocalSpace( this->forward(), 0 );
			}
			break;
		case(ESerializeDataType_Side):
			{
				this->setSide(kVec);	
				this->regenerateLocalSpace( this->forward(), 0 );
			}
			break;
		case(ESerializeDataType_Up):
			{
				this->setUp(kVec);	
				this->regenerateLocalSpace( this->forward(), 0 );
			}
			break;
		case(ESerializeDataType_Force):
			{
				this->setLastSteeringForce( kVec );	
			}
			break;
		}
	}
}

