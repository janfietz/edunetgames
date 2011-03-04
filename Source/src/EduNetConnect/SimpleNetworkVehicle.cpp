#include "EduNetConnect/SimpleNetworkVehicle.h"

#include "EduNetConnect/NetworkPlugin.h"
#include "EduNetConnect/NetworkEntity.h"

#include "OpenSteer/GlobalData.h"


using namespace OpenSteer;

//-----------------------------------------------------------------------------
void SimpleProxyVehicle::draw( OpenSteer::AbstractRenderer* pRenderer, const float currentTime, const float elapsedTime )
{
//	BaseClass::draw( currentTime, elapsedTime );
	if( this->isRemoteObject() )
	{
		if( 1 == GlobalData::getInstance()->m_bShowClientNetworkTrail )
		{
			LocalSpaceDataCIterator kIter = this->m_kLocalSpaceData.begin();
			LocalSpaceDataCIterator kEnd = this->m_kLocalSpaceData.end();
			Color kColor = gBlue;
			kColor.setR( 0.3f );
			kColor.setG( 0.4f );
			kColor.setB( 0.0f );
			kColor.setA( 0.2f );
			float fUpOffset = 0.01f;
			while( kIter != kEnd )
			{
				const LocalSpaceData& kLocalSpaceData = *kIter;
				pRenderer->drawBasic2dCircularLocalSpace( kLocalSpaceData, kColor, this->radius() * 0.75f, false, fUpOffset );
				fUpOffset += 0.01f;
				++kIter;
				kColor.setA( kColor.a() + 0.05f );
				kColor.setB( kColor.b() + 0.1f );
			}
		}
	}
	else
	{
		if( 1 == GlobalData::getInstance()->m_bShowServerNetworkTrail )
		{

			float fUpOffset = 0.1f;
			// show send data
			Color kColor = gCyan;
			pRenderer->drawBasic2dCircularLocalSpace( this->accessLocalSpaceData(), kColor, this->radius() * 0.75f, false, fUpOffset );

			kColor.setR( 0.0f );
			kColor.setG( 0.4f );
			kColor.setB( 0.3f );
			kColor.setA( 0.2f );
			fUpOffset = 0.01f;
			LocalSpaceDataCIterator kIter = this->m_kLocalSpaceData.begin();
			LocalSpaceDataCIterator kEnd = this->m_kLocalSpaceData.end();
			while( kIter != kEnd )
			{
				const LocalSpaceData& kLocalSpaceData = *kIter;
				pRenderer->drawBasic2dCircularLocalSpace( kLocalSpaceData, kColor, this->radius() * 0.75f, false, fUpOffset );
				fUpOffset += 0.01f;
				++kIter;
				kColor.setA( kColor.a() + 0.05f );
				kColor.setR( kColor.r() + 0.1f );
			}

			// show send data
			kColor = gDarkYellow;
			pRenderer->drawBasic2dCircularLocalSpace( m_kextrapolatedLocalSpace.accessLocalSpaceData(), kColor, this->radius() * 0.75f, false, fUpOffset );
		}
	}
}

//-----------------------------------------------------------------------------
void SimpleProxyVehicle::update (const float currentTime, const float elapsedTime)
{
}


//-------------------------------------------------------------------------
#pragma warning(push)
#pragma warning(disable: 4355) // warning C4355: 'this' : used in base member initializer list
SimpleNetworkVehicle::SimpleNetworkVehicle():
	m_kNetworkVehicleUpdate(this),
	m_bWillSendData( false ),
	m_bHasBeenSerialized( false ),
	m_bSupportZoning( false )
{
	memset( this->m_bIsZoneMember, 0, sizeof(bool) * MAX_ZONES );
}
#pragma warning(pop)

//-----------------------------------------------------------------------------
SimpleNetworkVehicle::~SimpleNetworkVehicle()
{
}

//-----------------------------------------------------------------------------
void SimpleNetworkVehicle::collect3DTextAnnotation( std::ostringstream& kStream )
{
	this->m_bCollectsAnnotations = true;
	BaseClass::collect3DTextAnnotation( kStream );
	this->m_kNetworkVehicleUpdate.collect3DTextAnnotation( kStream );
}

//-----------------------------------------------------------------------------
void SimpleNetworkVehicle::draw( AbstractRenderer* pRenderer, 
	const float currentTime,
	const float elapsedTime )
{
	this->m_bCollectsAnnotations = false;
	BaseClass::draw( pRenderer, currentTime, elapsedTime );
	if( true == this->m_bCollectsAnnotations )
	{
		this->accessProxyVehicle().draw( pRenderer, currentTime, elapsedTime );
	}
}

//-----------------------------------------------------------------------------
void SimpleNetworkVehicle::update (const float currentTime, const float elapsedTime)
{
	// this is a bad hack right now !!!
	static bool bStaticInit = false;
	if( false == bStaticInit )
	{
		bStaticInit = true;
		GlobalData::getInstance()->m_uiReplicationDataBytes[ESerializeDataType_Orientation] = sizeof(btQuaternion) + 1;
		GlobalData::getInstance()->m_uiReplicationDataBytes[ESerializeDataType_CompressedForce] = sizeof(CompressedVector) + 1;
	}

	// pass over important values to the proxy
	SimpleProxyVehicle& kProxy = this->accessProxyVehicle();
	kProxy.setIsRemoteObject( this->isRemoteObject() );
	kProxy.setMaxForce( this->maxForce() );
	kProxy.setMaxSpeed( this->maxSpeed() );
	kProxy.setMass( this->mass() );
	kProxy.setRadius( this->radius() );
	kProxy.setMovesPlanar( this->movesPlanar() );

	// simple send data control
	// note: not connection specific
	bool bRecordNetGraph = false;
	if( false == this->isRemoteObject() )
	{
		if( this->m_bHasBeenSerialized )
		{
			this->m_bWillSendData = false;
			this->m_bHasBeenSerialized = false;
		}
		// set this frequency each update cycle as it might get changed by the gui
		if( this->m_kNetworkVehicleUpdate.serverVehicleUpdate().getServerVehicleMode() == EServerVehicleMode_ExtrapolateProxy){
			this->m_kNetWriteUpdatePeriod.SetPeriodTime(5.0f, true); //Set update time to 5 seconds
		}else{
			this->m_kNetWriteUpdatePeriod.SetPeriodFrequency( GlobalData::getInstance()->m_NetWriteFPS, true );
		}

		size_t uiTicks = this->m_kNetWriteUpdatePeriod.UpdateDeltaTime( elapsedTime );
		if( false == this->m_bWillSendData )
		{
			this->m_kNetworkVehicleUpdate.serverVehicleUpdate().resetExtrapolationData(*this);
			this->m_bWillSendData = ( uiTicks > 0 );
			bRecordNetGraph = this->m_bWillSendData;
		}
	}
	else
	{
		const SimpleProxyVehicle& kProxy = this->getProxyVehicle();
		bRecordNetGraph = kProxy.m_bHasNewData;
	}

	// in case the custom updater decides to call the base class
	// prevent infinite recursion, store the custom updater locally
	// and restore it once done with the update
	AbstractUpdated* pkCustomUpdated = this->getCustomUpdated();
	this->setCustomUpdated( NULL );
	this->m_kNetworkVehicleUpdate.updateCustom( this, currentTime, elapsedTime );
	this->setCustomUpdated( pkCustomUpdated );

	// once the real vehicle update function has been called
	// one has to assume the new data have made their way into the simulation
	// this applies to the server and the client
	kProxy.m_bHasNewData = false;

	// server or client side send dots
	if( true == bRecordNetGraph )
	{
		NetworkPlugin::recordNetUpdate( this, currentTime, elapsedTime );
	}
}

//-----------------------------------------------------------------------------
void SimpleNetworkVehicle::updateBase(const float currentTime, const float elapsedTime)
{
	BaseClass::update( currentTime, elapsedTime );
}

// TODO: implement specialized connection specific QuerySerialization implementations
//
//-----------------------------------------------------------------------------
int SimpleNetworkVehicle::serialize( RakNet::SerializeParameters *serializeParameters ) const
{
	// note: serialize is called for each connection
	//       so here we have loads of optimization possibilities
	if( false == this->m_bWillSendData )
	{
		return RakNet::RM3SR_DO_NOT_SERIALIZE;
	}

	ET_PROFILE( serializeNetworkVehicle );

	// reset the send proxy data
	SimpleProxyVehicle& kProxy = this->accessProxyVehicle();

	if(true == kProxy.m_bHasNewData )
	{
		bool bStop = true;
	}	

	bool bUpdatedProxy = false;
	if( false == this->m_bHasBeenSerialized )
	{
		// serialize is called for the first connection time now
		// zero out all send local space data
		kProxy.accessLocalSpaceData().resetLocalSpaceData();

		// now a special case arises
		// in case the steering force is replicated
		// to create a consistent data set the steering force for the next update
		// has to be computed
		if( ( GlobalData::getInstance()->m_bReplicationDataConfig[ESerializeDataType_Force] != 0 ) ||
			( GlobalData::getInstance()->m_bReplicationDataConfig[ESerializeDataType_CompressedForce] != 0 )
			)
		{
			Vec3 kNextSteeringForce = this->getSteeringForceUpdate().determineCombinedSteering( this->getUpdateElapsedTime() );
			kProxy.accessSteeringForceUpdate().setForce( kNextSteeringForce );
		}
		bUpdatedProxy = true;
	}

	// mark as serialized for the following connections
	this->m_bHasBeenSerialized = true;


	RakNet::BitStream& kStream = serializeParameters->outputBitstream[0];

	unsigned char dataTypes = 0; // how many variables will be send
	unsigned char dataType;

	for( size_t i = 0; i < ESerializeDataType_Count; ++i)
	{
		if( GlobalData::getInstance()->m_bReplicationDataConfig[i] != 0 )
		{
			kProxy.m_bSerializedDataTypes[i] = true;
			++dataTypes;
		}
	}
	kStream.WriteAlignedBytes(&dataTypes,sizeof(unsigned char));

	if( GlobalData::getInstance()->m_bReplicationDataConfig[ESerializeDataType_Position] != 0 )
	{
		dataType = ESerializeDataType_Position;
		kStream.WriteAlignedBytes(&dataType,sizeof(unsigned char));
		kStream.WriteAlignedBytes((const unsigned char*)&this->position(),sizeof(OpenSteer::Vec3));
		if( true == bUpdatedProxy && (false == kProxy.m_bHasNewData) )
		{
			kProxy.setForward( this->_forward );
			kProxy.regenerateLocalSpace( kProxy.forward(), 0 );

			kProxy.setPosition( this->position() );
		}
	}
	if( GlobalData::getInstance()->m_bReplicationDataConfig[ESerializeDataType_Forward] != 0 )
	{
		dataType = ESerializeDataType_Forward;
		kStream.WriteAlignedBytes(&dataType,sizeof(unsigned char));
		kStream.WriteAlignedBytes((const unsigned char*)&this->forward(),sizeof(OpenSteer::Vec3));
		if( true == bUpdatedProxy && (false == kProxy.m_bHasNewData) )
		{
			kProxy.setForward( this->forward() );
			kProxy.regenerateLocalSpace( kProxy.forward(), 0 );
		}
	}
	if( GlobalData::getInstance()->m_bReplicationDataConfig[ESerializeDataType_Side] != 0 )
	{
		dataType = ESerializeDataType_Side;
		kStream.WriteAlignedBytes(&dataType,sizeof(unsigned char));
		kStream.WriteAlignedBytes((const unsigned char*)&this->side(),sizeof(OpenSteer::Vec3));
		if( true == bUpdatedProxy && (false == kProxy.m_bHasNewData) )
		{
			kProxy.setSide( this->side() );
			kProxy.regenerateLocalSpace( kProxy.forward(), 0 );
		}
	}
	if( GlobalData::getInstance()->m_bReplicationDataConfig[ESerializeDataType_Up] != 0 )
	{
		dataType = ESerializeDataType_Up;
		kStream.WriteAlignedBytes(&dataType,sizeof(unsigned char));
		kStream.WriteAlignedBytes((const unsigned char*)&this->up(),sizeof(OpenSteer::Vec3));
		if( true == bUpdatedProxy && (false == kProxy.m_bHasNewData) )
		{
			kProxy.setUp( this->up() );
			kProxy.regenerateLocalSpace( kProxy.forward(), 0 );
		}
	}
	if( GlobalData::getInstance()->m_bReplicationDataConfig[ESerializeDataType_Force] != 0 )
	{
		dataType = ESerializeDataType_Force;
		kStream.WriteAlignedBytes(&dataType,sizeof(unsigned char));

		const SteeringForceVehicleUpdate& kSteeringForceUpdate = kProxy.getSteeringForceUpdate();
		kStream.WriteAlignedBytes((const unsigned char*)&kSteeringForceUpdate.getForce(),sizeof(OpenSteer::Vec3));
		if( true == bUpdatedProxy && (false == kProxy.m_bHasNewData) )
		{
			// nothing to do here se code above
		}
	}
	if( GlobalData::getInstance()->m_bReplicationDataConfig[ESerializeDataType_Radius] != 0 )
	{
		dataType = ESerializeDataType_Radius;
		kStream.WriteAlignedBytes(&dataType,sizeof(unsigned char));
		float fValue = this->radius();
		kStream.WriteAlignedBytes((const unsigned char*)&fValue,sizeof(float));
		if( true == bUpdatedProxy && (false == kProxy.m_bHasNewData) )
		{
			kProxy.setRadius( this->radius() );
		}
	}
	if( GlobalData::getInstance()->m_bReplicationDataConfig[ESerializeDataType_Speed] != 0 )
	{
		dataType = ESerializeDataType_Speed;
		kStream.WriteAlignedBytes(&dataType,sizeof(unsigned char));
		float fValue = this->speed();
		kStream.WriteAlignedBytes((const unsigned char*)&fValue,sizeof(float));
		if( true == bUpdatedProxy && (false == kProxy.m_bHasNewData) )
		{
			kProxy.setSpeed( this->speed() );
		}
	}
	if( GlobalData::getInstance()->m_bReplicationDataConfig[ESerializeDataType_Orientation] != 0 )
	{
		dataType = ESerializeDataType_Orientation;
		kStream.WriteAlignedBytes(&dataType,sizeof(unsigned char));
		btQuaternion kRotation = AbstractVehicleMath::computeQuaternionFromLocalSpace( this->getLocalSpaceData() );
		kStream.WriteAlignedBytes((const unsigned char*)&kRotation,sizeof(btQuaternion));
		if( true == bUpdatedProxy && (false == kProxy.m_bHasNewData) )
		{
			AbstractVehicleMath::writeQuaternionToLocalSpace( kRotation, kProxy.accessLocalSpaceData() );
		}
	}
	if( GlobalData::getInstance()->m_bReplicationDataConfig[ESerializeDataType_CompressedOrientation1] != 0 )
	{
		dataType = ESerializeDataType_CompressedOrientation1;
		kStream.WriteAlignedBytes(&dataType,sizeof(unsigned char));
		btQuaternion kRotation = AbstractVehicleMath::computeQuaternionFromLocalSpace( this->getLocalSpaceData() );
		char wSign = 0;
		osVector3 kCompressedRotation = AbstractVehicleMath::compressQuaternion( kRotation, wSign );
		kStream.WriteAlignedBytes((const unsigned char*)&kCompressedRotation,sizeof(osVector3));
		kStream.WriteAlignedBytes((const unsigned char*)&wSign,sizeof(char));
		if( true == bUpdatedProxy && (false == kProxy.m_bHasNewData) )
		{
			kRotation = AbstractVehicleMath::expandQuaternion( kCompressedRotation, wSign == 1 ? 1.0f : -1.0f );
			AbstractVehicleMath::writeQuaternionToLocalSpace( kRotation, kProxy.accessLocalSpaceData() );
		}
	}
	if( GlobalData::getInstance()->m_bReplicationDataConfig[ESerializeDataType_CompressedOrientation2] != 0 )
	{
		dataType = ESerializeDataType_CompressedOrientation2;
		kStream.WriteAlignedBytes(&dataType,sizeof(unsigned char));
		btQuaternion kRotation = AbstractVehicleMath::computeQuaternionFromLocalSpace( this->getLocalSpaceData() );
		char wSign = 0;
		osVector3 kCompressedRotation = AbstractVehicleMath::compressQuaternion( kRotation, wSign );
		char cVector[3];
		AbstractVehicleMath::compressUnitVector( kCompressedRotation, cVector );
		kStream.WriteAlignedBytes((const unsigned char*)&cVector,sizeof(cVector));
		kStream.WriteAlignedBytes((const unsigned char*)&wSign,sizeof(char));
		if( true == bUpdatedProxy && (false == kProxy.m_bHasNewData) )
		{
			AbstractVehicleMath::expandUnitVector( cVector, kCompressedRotation );
			kRotation = AbstractVehicleMath::expandQuaternion( kCompressedRotation, wSign == 1 ? 1.0f : -1.0f );
			AbstractVehicleMath::writeQuaternionToLocalSpace( kRotation, kProxy.accessLocalSpaceData() );
		}
	}
	if( GlobalData::getInstance()->m_bReplicationDataConfig[ESerializeDataType_CompressedForce] != 0 )
	{
		dataType = ESerializeDataType_CompressedForce;
		kStream.WriteAlignedBytes(&dataType,sizeof(unsigned char));
		const SteeringForceVehicleUpdate& kSteeringForceUpdate = kProxy.getSteeringForceUpdate();
		kStream.WriteAlignedBytes((const unsigned char*)&kSteeringForceUpdate.getCompressedForce(),sizeof(CompressedVector));
		if( true == bUpdatedProxy && (false == kProxy.m_bHasNewData) )
		{
			// nothing to do here ... see code above
		}
	}
	if( GlobalData::getInstance()->m_bReplicationDataConfig[ESerializeDataType_AngularVelocity] != 0 )
	{
		dataType = ESerializeDataType_AngularVelocity;
		kStream.WriteAlignedBytes(&dataType,sizeof(unsigned char));
		kStream.WriteAlignedBytes((const unsigned char*)&this->angularVelocity(),sizeof(OpenSteer::Vec3));
		if( true == bUpdatedProxy && (false == kProxy.m_bHasNewData) )
		{
			kProxy.setAngularVelocity( this->angularVelocity() );
		}
	}
	if( GlobalData::getInstance()->m_bReplicationDataConfig[ESerializeDataType_LinearVelocity] != 0 )
	{
		dataType = ESerializeDataType_LinearVelocity;
		kStream.WriteAlignedBytes(&dataType,sizeof(unsigned char));
		kStream.WriteAlignedBytes((const unsigned char*)&this->linearVelocity(),sizeof(OpenSteer::Vec3));
		if( true == bUpdatedProxy && (false == kProxy.m_bHasNewData) )
		{
			kProxy.setLinearVelocity( this->linearVelocity() );
		}
	}
	if( GlobalData::getInstance()->m_bReplicationDataConfig[ESerializeDataType_UpdateTicks] != 0 )
	{
		dataType = ESerializeDataType_UpdateTicks;
		kStream.WriteAlignedBytes(&dataType,sizeof(unsigned char));
		size_t updateTicks = this->getLocalSpaceData()._updateTicks;
		kStream.WriteAlignedBytes((const unsigned char*)&updateTicks,sizeof(size_t));
		if( true == bUpdatedProxy && (false == kProxy.m_bHasNewData) )
		{
			//kProxy._updateTicks = updateTicks;
		}
	}

	//mark the proxy having new data
	kProxy.m_bHasNewData = true;

	return RakNet::RM3SR_BROADCAST_IDENTICALLY;
}

//-----------------------------------------------------------------------------
void SimpleNetworkVehicle::deserialize( RakNet::DeserializeParameters *deserializeParameters )
{
	ET_PROFILE( deserializeNetworkVehicle );

	RakNet::BitStream& kStream = deserializeParameters->serializationBitstream[0];

	// reset the received data configuration
	SimpleProxyVehicle& kProxy = this->accessProxyVehicle();

	if(kProxy.m_bHasNewData == true)
	{
		bool bStop = true;
		return;
	}

	for( size_t i = 0; i < ESerializeDataType_Count; ++i )
	{
		kProxy.m_bSerializedDataTypes[i] = false;
	}

	// do not directly write into the scene vehicle
	// otherwise there will be no chance to customize the network behaviour
	AbstractVehicle* pkSerializeTarget = &kProxy;

	// store last update tick to check if new data has arrived
	const size_t lastServerUpdateTicks = pkSerializeTarget->accessLocalSpaceData()._updateTicks;

	// zero out all received local space data
	pkSerializeTarget->accessLocalSpaceData().resetLocalSpaceData();

	float fValue;
	osVector3 kVec;
	char wSign;
	btQuaternion kRotation;
	unsigned char dataTypes;
	char cVector[3];
	size_t uiValue;
	CompressedVector kCompressedVector;
	kStream.ReadAlignedBytes(&dataTypes,sizeof(unsigned char));
	for( unsigned char i = 0; i < dataTypes; ++i )
	{
		unsigned char dataType;
		kStream.ReadAlignedBytes(&dataType,sizeof(unsigned char));
		ESerializeDataType eDataType = static_cast<ESerializeDataType>(dataType);
		kProxy.m_bSerializedDataTypes[eDataType] = true;
		switch( eDataType )
		{
		case(ESerializeDataType_Position):
		case(ESerializeDataType_Forward):
		case(ESerializeDataType_Side):
		case(ESerializeDataType_Up):
		case(ESerializeDataType_Force):
		case(ESerializeDataType_AngularVelocity):
		case(ESerializeDataType_LinearVelocity):
			{
				kStream.ReadAlignedBytes((unsigned char*)&kVec,sizeof(osVector3));
			}
			break;
		case(ESerializeDataType_CompressedForce):
			{
				kStream.ReadAlignedBytes((unsigned char*)&kCompressedVector,sizeof(kCompressedVector));
				// expand
				AbstractVehicleMath::expandFixedLengthVector(
					kCompressedVector, this->maxForce(), kVec );
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
		case(ESerializeDataType_UpdateTicks):
			{
				kStream.ReadAlignedBytes((unsigned char*)&uiValue,sizeof(size_t));
			}
			break;
		}

		if( false == NetworkEntitySerializer::setLocalSpaceVariable( eDataType, pkSerializeTarget, kVec ) )
		{
			switch( eDataType )
			{
			case(ESerializeDataType_Orientation):
				{
					AbstractVehicleMath::writeQuaternionToLocalSpace( kRotation, pkSerializeTarget->accessLocalSpaceData() );
				}
				break;
			case(ESerializeDataType_CompressedOrientation1):
				{
					kRotation = AbstractVehicleMath::expandQuaternion( kVec, wSign == 1 ? 1.0f : -1.0f );
					AbstractVehicleMath::writeQuaternionToLocalSpace( kRotation, pkSerializeTarget->accessLocalSpaceData() );
				}
				break;
			case(ESerializeDataType_CompressedOrientation2):
				{
					AbstractVehicleMath::expandUnitVector( cVector, kVec );
					kRotation = AbstractVehicleMath::expandQuaternion( kVec, wSign == 1 ? 1.0f : -1.0f );
					AbstractVehicleMath::writeQuaternionToLocalSpace( kRotation, pkSerializeTarget->accessLocalSpaceData() );
				}
				break;
			case(ESerializeDataType_CompressedForce):
			case(ESerializeDataType_Force):
				{
					SteeringForceVehicleUpdate& kSteeringForceUpdate = kProxy.accessSteeringForceUpdate();
					kSteeringForceUpdate.setForce( kVec );
				}
				break;
			case(ESerializeDataType_Speed):
				{
					pkSerializeTarget->setSpeed( fValue );
				}
				break;
			case(ESerializeDataType_UpdateTicks):
				{
					pkSerializeTarget->accessLocalSpaceData()._updateTicks = uiValue;
				}
				break;
			}
		}
	}

	const size_t newServerUpdateTicks = pkSerializeTarget->getLocalSpaceData()._updateTicks;
	if( newServerUpdateTicks != 0 )
	{
		if( lastServerUpdateTicks < newServerUpdateTicks )
		{
			kProxy.m_bHasNewData = true;
		}
	}
	else
	{
		kProxy.m_bHasNewData = true;
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

//-----------------------------------------------------------------------------
void SimpleNetworkVehicle::setIsZoneMember( size_t zoneId, bool value )
{
	if( zoneId < MAX_ZONES )
	{
		this->m_bIsZoneMember[zoneId] = value;
	}
}

//-----------------------------------------------------------------------------
bool SimpleNetworkVehicle::getIsZoneMember( size_t zoneId ) const
{
	if( zoneId < MAX_ZONES )
	{
		return this->m_bIsZoneMember[zoneId];
	}
	return false;
}

