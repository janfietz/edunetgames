#include "SimpleNetworkVehicle.h"


using namespace OpenSteer;

//-----------------------------------------------------------------------------
void SimpleNetworkVehicleUpdate::updateCustom( AbstractUpdated* pkParent, const osScalar currentTime, const osScalar elapsedTime )
{
	SimpleNetworkVehicle* pkNetworkVehicle = dynamic_cast<SimpleNetworkVehicle*>(pkParent);
	SimpleProxyVehicle& kProxy = pkNetworkVehicle->accessProxyVehicle();
	if( true == kProxy.m_bHasNewData )
	{
		// now read the proxy and apply the data to the scene vehicle
		// position interpolation
		float fFactor = 0.5;
		osVector3 kNewPosition = interpolate( fFactor, pkNetworkVehicle->position(), kProxy.getLocalSpaceData()._position);

		osVector3 kNewForward = interpolate( fFactor, pkNetworkVehicle->forward(), kProxy.getLocalSpaceData()._forward);
		kNewForward.normalize();
		kProxy.accessLocalSpaceData()._forward = kNewForward;
		kProxy.accessLocalSpaceData()._position = kNewPosition;

		pkNetworkVehicle->setLocalSpaceData( kProxy.getLocalSpaceData() );

		pkNetworkVehicle->setLastSteeringForce( kProxy.lastSteeringForce() );
		kProxy.m_bHasNewData = false;
	}
//	pkNetworkVehicle->update( currentTime, elapsedTime ); 
}

//-------------------------------------------------------------------------
void SimpleNetworkVehicleUpdate::update( const osScalar currentTime, const osScalar elapsedTime )
{

}

//-------------------------------------------------------------------------
#pragma warning(push)
#pragma warning(disable: 4355) // warning C4355: 'this' : used in base member initializer list
SimpleNetworkVehicle::SimpleNetworkVehicle():m_kNetworkVehicleUpdate(this)
{ 
//	this->setCustomUpdated( &this->m_kNetworkVehicleUpdate );
}
#pragma warning(pop)

//-----------------------------------------------------------------------------
SimpleNetworkVehicle::~SimpleNetworkVehicle() 
{ 
}

//-----------------------------------------------------------------------------
void SimpleNetworkVehicle::update (const float currentTime, const float elapsedTime)
{
	// in case the custom updater decides to call the base class
	// prevent infinite recursion, store the custom updater locally
	// and restore it once done with the update
	AbstractUpdated* pkCustomUpdated = this->getCustomUpdated();
	this->setCustomUpdated( NULL );
	this->m_kNetworkVehicleUpdate.updateCustom( this, currentTime, elapsedTime );
	BaseClass::update( currentTime, elapsedTime );
	this->setCustomUpdated( pkCustomUpdated );
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
	AbstractVehicle* pkSerializeTarget = &this->m_kProxyVehicle;
	this->m_kProxyVehicle.m_bHasNewData = true;
	OpenSteer::Vec3 kVec;
	unsigned char dataTypes;
	kStream.ReadAlignedBytes(&dataTypes,sizeof(unsigned char));
	for( unsigned char i = 0; i < dataTypes; ++i )
	{
		unsigned char dataType;
		kStream.ReadAlignedBytes(&dataType,sizeof(unsigned char));
		ESerializeDataType eDataType = static_cast<ESerializeDataType>(dataType);
		switch( eDataType )
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

		if( false == NetworkEntitySerializer::setLocalSpaceVariable( eDataType, pkSerializeTarget, kVec ) )
		{
			switch( eDataType )
			{
			case(ESerializeDataType_Force):
				{
					pkSerializeTarget->setLastSteeringForce( kVec );	
				}
				break;
			}
		}
	}
}

