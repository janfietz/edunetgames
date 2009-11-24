#include "SimpleNetworkVehicle.h"
#include "OpenSteerUT/AbstractVehicleMath.h"


using namespace OpenSteer;

int SimpleNetworkVehicle::ms_bReplicationDataConfig[ESerializeDataType_Count] =
{
	1, // 	ESerializeDataType_Position,
	1, // 	ESerializeDataType_Forward,
	0, // 	ESerializeDataType_Side,
	0, // 	ESerializeDataType_Up,
	0, // 	ESerializeDataType_Force,
	0, // 	ESerializeDataType_Radius,
	1, // 	ESerializeDataType_Speed,
	0, //   ESerializeDataType_Orientation
	0, // 	ESerializeDataType_CompressedOrientation1,
	0, // 	ESerializeDataType_CompressedOrientation2,
	0, // 	ESerializeDataType_CompressedForce,

};

osScalar SimpleNetworkVehicle::ms_NetWriteFPS = 20.0f;

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
		pkNetworkVehicle->setSpeed( kProxy.speed() );

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
SimpleNetworkVehicle::SimpleNetworkVehicle():
	m_kNetworkVehicleUpdate(this),
	m_bWantsToSendData( false ),
	m_bHasBeenSerialized( false )
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
	// simple send data control
	// note: not connection specific
	if( this->m_bHasBeenSerialized )
	{
		this->m_bWantsToSendData = false;
		this->m_bHasBeenSerialized = false;
	}
	// set this frequency each update cycle as it might get changed by the gui
	this->m_kNetWriteUpdatePeriod.SetPeriodFrequency( SimpleNetworkVehicle::ms_NetWriteFPS, true );
	size_t uiTicks = this->m_kNetWriteUpdatePeriod.UpdateDeltaTime( elapsedTime );
	if( false == this->m_bWantsToSendData )
	{
		this->m_bWantsToSendData = ( uiTicks > 0 );
	}

	// in case the custom updater decides to call the base class
	// prevent infinite recursion, store the custom updater locally
	// and restore it once done with the update
	AbstractUpdated* pkCustomUpdated = this->getCustomUpdated();
	this->setCustomUpdated( NULL );
	this->m_kNetworkVehicleUpdate.updateCustom( this, currentTime, elapsedTime );
	BaseClass::update( currentTime, elapsedTime );
	this->setCustomUpdated( pkCustomUpdated );
}

// TODO: implement specialized connection specific QuerySerialization implementations 
// 
//-----------------------------------------------------------------------------
int SimpleNetworkVehicle::serialize( RakNet::SerializeParameters *serializeParameters ) const
{
	// note: serialize is called for each connection
	//       so here we have loads of optimization possibilities
	if( false == this->m_bWantsToSendData )
	{
		return RakNet::RM3SR_DO_NOT_SERIALIZE;
	}
	this->m_bHasBeenSerialized = true;

	RakNet::BitStream& kStream = serializeParameters->outputBitstream[0];

	unsigned char dataTypes = 0; // how many variables will be send
	unsigned char dataType;

	for( size_t i = 0; i < ESerializeDataType_Count; ++i)
	{
		if( ms_bReplicationDataConfig[i] != 0 )
		{
			++dataTypes;
		}
	}
	kStream.WriteAlignedBytes(&dataTypes,sizeof(unsigned char));

	if( ms_bReplicationDataConfig[ESerializeDataType_Position] != 0 )
	{
		dataType = ESerializeDataType_Position;
		kStream.WriteAlignedBytes(&dataType,sizeof(unsigned char));
		kStream.WriteAlignedBytes((const unsigned char*)&this->position(),sizeof(OpenSteer::Vec3));
	}
	if( ms_bReplicationDataConfig[ESerializeDataType_Forward] != 0 )
	{
		dataType = ESerializeDataType_Forward;
		kStream.WriteAlignedBytes(&dataType,sizeof(unsigned char));
		kStream.WriteAlignedBytes((const unsigned char*)&this->forward(),sizeof(OpenSteer::Vec3));
	}
	if( ms_bReplicationDataConfig[ESerializeDataType_Side] != 0 )
	{
		dataType = ESerializeDataType_Side;
		kStream.WriteAlignedBytes(&dataType,sizeof(unsigned char));
		kStream.WriteAlignedBytes((const unsigned char*)&this->side(),sizeof(OpenSteer::Vec3));
	}
	if( ms_bReplicationDataConfig[ESerializeDataType_Up] != 0 )
	{
		dataType = ESerializeDataType_Up;
		kStream.WriteAlignedBytes(&dataType,sizeof(unsigned char));
		kStream.WriteAlignedBytes((const unsigned char*)&this->up(),sizeof(OpenSteer::Vec3));
	}
	if( ms_bReplicationDataConfig[ESerializeDataType_Force] != 0 )
	{
		dataType = ESerializeDataType_Force;
		kStream.WriteAlignedBytes(&dataType,sizeof(unsigned char));
		kStream.WriteAlignedBytes((const unsigned char*)&this->lastSteeringForce(),sizeof(OpenSteer::Vec3));
	}
	if( ms_bReplicationDataConfig[ESerializeDataType_Radius] != 0 )
	{
		dataType = ESerializeDataType_Radius;
		kStream.WriteAlignedBytes(&dataType,sizeof(unsigned char));
		float fValue = this->radius();
		kStream.WriteAlignedBytes((const unsigned char*)&fValue,sizeof(float));
	}
	if( ms_bReplicationDataConfig[ESerializeDataType_Speed] != 0 )
	{
		dataType = ESerializeDataType_Speed;
		kStream.WriteAlignedBytes(&dataType,sizeof(unsigned char));
		float fValue = this->speed();
		kStream.WriteAlignedBytes((const unsigned char*)&fValue,sizeof(float));
	}
	if( ms_bReplicationDataConfig[ESerializeDataType_Orientation] != 0 )
	{
		dataType = ESerializeDataType_Orientation;
		btQuaternion kRotation = AbstractVehicleMath::readRotation( this->getLocalSpaceData() );
		kStream.WriteAlignedBytes(&dataType,sizeof(unsigned char));
		kStream.WriteAlignedBytes((const unsigned char*)&kRotation,sizeof(btQuaternion));
	}
	if( ms_bReplicationDataConfig[ESerializeDataType_CompressedOrientation1] != 0 )
	{
		dataType = ESerializeDataType_CompressedOrientation1;
		btQuaternion kRotation = AbstractVehicleMath::readRotation( this->getLocalSpaceData() );
		char wSign = 0;
		osVector3 kCompressedRotation = AbstractVehicleMath::compressQuaternion( kRotation, wSign );
		kStream.WriteAlignedBytes(&dataType,sizeof(unsigned char));
		kStream.WriteAlignedBytes((const unsigned char*)&kCompressedRotation,sizeof(osVector3));
		kStream.WriteAlignedBytes((const unsigned char*)&wSign,sizeof(char));
	}
	if( ms_bReplicationDataConfig[ESerializeDataType_CompressedOrientation2] != 0 )
	{
		dataType = ESerializeDataType_CompressedOrientation2;
		btQuaternion kRotation = AbstractVehicleMath::readRotation( this->getLocalSpaceData() );
		char wSign = 0;
		osVector3 kCompressedRotation = AbstractVehicleMath::compressQuaternion( kRotation, wSign );
		char cVector[3];
		AbstractVehicleMath::compressUnitVector( kCompressedRotation, cVector );
		kStream.WriteAlignedBytes(&dataType,sizeof(unsigned char));
		kStream.WriteAlignedBytes((const unsigned char*)&cVector,sizeof(cVector));
		kStream.WriteAlignedBytes((const unsigned char*)&wSign,sizeof(char));
	}
	if( ms_bReplicationDataConfig[ESerializeDataType_CompressedForce] != 0 )
	{
		dataType = ESerializeDataType_CompressedForce;
		kStream.WriteAlignedBytes(&dataType,sizeof(unsigned char));
		kStream.WriteAlignedBytes((const unsigned char*)&this->lastSteeringForce(),sizeof(OpenSteer::Vec3));
	}

	return RakNet::RM3SR_BROADCAST_IDENTICALLY;
}

//-----------------------------------------------------------------------------
void SimpleNetworkVehicle::deserialize( RakNet::DeserializeParameters *deserializeParameters )
{
	RakNet::BitStream& kStream = deserializeParameters->serializationBitstream[0];

	// reset the received data configuration
	for( size_t i = 0; i < ESerializeDataType_Count; ++i )
	{
		this->m_kProxyVehicle.m_bReveivedDataConfig[i] = 0;
	}

	// do not directly write into the scene vehicle
	// otherwise there will be no chance to customize the network behaviour
	AbstractVehicle* pkSerializeTarget = &this->m_kProxyVehicle;
	this->m_kProxyVehicle.m_bHasNewData = true;
	float fValue;
	osVector3 kVec;
	char wSign;
	btQuaternion kRotation;
	unsigned char dataTypes;
	char cVector[3];
	kStream.ReadAlignedBytes(&dataTypes,sizeof(unsigned char));
	for( unsigned char i = 0; i < dataTypes; ++i )
	{
		unsigned char dataType;
		kStream.ReadAlignedBytes(&dataType,sizeof(unsigned char));
		ESerializeDataType eDataType = static_cast<ESerializeDataType>(dataType);
		this->m_kProxyVehicle.m_bReveivedDataConfig[eDataType] = 1;
		switch( eDataType )
		{
		case(ESerializeDataType_Position):
		case(ESerializeDataType_Forward):
		case(ESerializeDataType_Side):
		case(ESerializeDataType_Up):
		case(ESerializeDataType_Force):
		case(ESerializeDataType_CompressedForce):
			{
				kStream.ReadAlignedBytes((unsigned char*)&kVec,sizeof(osVector3));
			}
			break;
		case(ESerializeDataType_CompressedOrientation1):
			{
				kStream.ReadAlignedBytes((unsigned char*)&kVec,sizeof(osVector3));
				kStream.ReadAlignedBytes((unsigned char*)&wSign,sizeof(char));
			}
			break;
		case(ESerializeDataType_CompressedOrientation2):
			{
				kStream.ReadAlignedBytes((unsigned char*)&cVector,sizeof(cVector));
				kStream.ReadAlignedBytes((unsigned char*)&wSign,sizeof(char));
			}
			break;
		case(ESerializeDataType_Orientation):
			{
				kStream.ReadAlignedBytes((unsigned char*)&kRotation,sizeof(btQuaternion));
			}
			break;
		case(ESerializeDataType_Radius):
		case(ESerializeDataType_Speed):
			{
				kStream.ReadAlignedBytes((unsigned char*)&fValue,sizeof(float));
			}
			break;
		}

		if( false == NetworkEntitySerializer::setLocalSpaceVariable( eDataType, pkSerializeTarget, kVec ) )
		{
			switch( eDataType )
			{
			case(ESerializeDataType_Orientation):
				{
					AbstractVehicleMath::writeRotation( kRotation, pkSerializeTarget->accessLocalSpaceData() );
				}
				break;
			case(ESerializeDataType_CompressedOrientation1):
				{
					kRotation = AbstractVehicleMath::expandQuaternion( kVec, wSign == 1 ? 1.0f : -1.0f );
					AbstractVehicleMath::writeRotation( kRotation, pkSerializeTarget->accessLocalSpaceData() );
				}
				break;
			case(ESerializeDataType_CompressedOrientation2):
				{
					AbstractVehicleMath::expandUnitVector( cVector, kVec );
					kRotation = AbstractVehicleMath::expandQuaternion( kVec, wSign == 1 ? 1.0f : -1.0f );
					AbstractVehicleMath::writeRotation( kRotation, pkSerializeTarget->accessLocalSpaceData() );
				}
				break;
			case(ESerializeDataType_CompressedForce):
				{
					// TODO
				}
				break;
			case(ESerializeDataType_Force):
				{
					pkSerializeTarget->setLastSteeringForce( kVec );	
				}
				break;
			case(ESerializeDataType_Speed):
				{
					pkSerializeTarget->setSpeed( fValue );
				}
				break;
			}
		}
	}
}
//-----------------------------------------------------------------------------
void SimpleNetworkVehicle::serializeConstruction(
	RakNet::BitStream *constructionBitstream)
{

}
//-----------------------------------------------------------------------------
bool SimpleNetworkVehicle::deserializeConstruction(
	RakNet::BitStream *constructionBitstream )
{
	return true;
}


