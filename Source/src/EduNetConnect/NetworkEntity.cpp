//-----------------------------------------------------------------------------
// Copyright (c) 2009, Jan Fietz, Cyrus Preuss
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without modification, 
// are permitted provided that the following conditions are met:
// 
// * Redistributions of source code must retain the above copyright notice, 
//   this list of conditions and the following disclaimer.
// * Redistributions in binary form must reproduce the above copyright notice, 
//   this list of conditions and the following disclaimer in the documentation 
//   and/or other materials provided with the distribution.
// * Neither the name of EduNetGames nor the names of its contributors
//   may be used to endorse or promote products derived from this software
//   without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
// IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES 
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON 
// ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, 
// EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//-----------------------------------------------------------------------------

#include "NetworkEntity.h"
#include "OpenSteerUT/OpenSteerUTTypes.h"
#include "OpenSteer/Obstacle.h"
#include "OpenSteer/AbstractPlayer.h"

using namespace OpenSteer;


//-----------------------------------------------------------------------------
NetworkEntitySerializer::NetworkEntitySerializer( OpenSteer::AbstractEntity* pkEntity ):m_pkEntity(pkEntity)
{
	this->m_pkVehicle = dynamic_cast<OpenSteer::AbstractVehicle*>( this->m_pkEntity );
}

//-----------------------------------------------------------------------------
NetworkEntitySerializer::NetworkEntitySerializer( OpenSteer::AbstractVehicle* pkVehicle ):m_pkVehicle(pkVehicle)
{
	this->m_pkEntity = dynamic_cast<OpenSteer::AbstractEntity*>( this->m_pkVehicle );
}

//-----------------------------------------------------------------------------
void NetworkEntitySerializer::queryVehicleSendParameters( const AbstractNetSerializable* pkVehicle, RakNet::PRO &kPro ) const
{
	// TODO: query the send parameters from the vehicle itself
	kPro.priority = LOW_PRIORITY;
	kPro.reliability = UNRELIABLE;
	kPro.orderingChannel = 1;
}

//-----------------------------------------------------------------------------
int NetworkEntitySerializer::serialize( RakNet::SerializeParameters *serializeParameters ) const
{
	const AbstractNetSerializable* pkNetworkVehicle = dynamic_cast<AbstractNetSerializable*>(this->m_pkEntity);
	if( NULL != pkNetworkVehicle )
	{
		RakNet::PRO& kPro = *serializeParameters->pro;
		this->queryVehicleSendParameters( pkNetworkVehicle, kPro );
		return pkNetworkVehicle->serialize( serializeParameters );
	}
	else
	{
		RakNet::BitStream& kStream = serializeParameters->outputBitstream[0];

		unsigned char dataType;
		// sphere obstacle
		const SphereObstacle* pkSphere = dynamic_cast<const SphereObstacle*>(this->m_pkEntity);
		if( NULL != pkSphere )
		{
			unsigned char dataTypes = 2; // how many variables will be send

			kStream.WriteAlignedBytes(&dataTypes,sizeof(unsigned char));

			dataType = ESerializeDataType_Position;
			kStream.WriteAlignedBytes(&dataType,sizeof(unsigned char));
			kStream.WriteAlignedBytes((const unsigned char*)&pkSphere->center,sizeof(OpenSteer::Vec3));

			dataType = ESerializeDataType_Radius;
			kStream.WriteAlignedBytes(&dataType,sizeof(unsigned char));
			kStream.WriteAlignedBytes((const unsigned char*)&pkSphere->radius,sizeof(float));
			return RakNet::RM3SR_BROADCAST_IDENTICALLY;
		}

		// player
		const osAbstractPlayer* pkPlayer = dynamic_cast<const osAbstractPlayer*>(this->m_pkEntity);
		if( NULL != pkPlayer )
		{
			const osAbstractController* pkController = pkPlayer->getController();
			unsigned char dataTypes = 5; // how many variables will be send
			kStream.WriteAlignedBytes(&dataTypes,sizeof(unsigned char));

			unsigned char controllerAction;
			float fValue;

			dataType = ESerializeDataType_ControllerAction;
			kStream.WriteAlignedBytes(&dataType,sizeof(unsigned char));

			controllerAction = EControllerAction_Left;
			fValue = pkController->getActionValue( EControllerAction_Left );
			kStream.WriteAlignedBytes(&controllerAction,sizeof(unsigned char));
			kStream.WriteAlignedBytes((const unsigned char*)&fValue,sizeof(float));

			dataType = ESerializeDataType_ControllerAction;
			kStream.WriteAlignedBytes(&dataType,sizeof(unsigned char));

			controllerAction = EControllerAction_Right;
			fValue = pkController->getActionValue( EControllerAction_Right );
			kStream.WriteAlignedBytes(&controllerAction,sizeof(unsigned char));
			kStream.WriteAlignedBytes((const unsigned char*)&fValue,sizeof(float));

			dataType = ESerializeDataType_ControllerAction;
			kStream.WriteAlignedBytes(&dataType,sizeof(unsigned char));

			controllerAction = EControllerAction_Backward;
			fValue = pkController->getActionValue( EControllerAction_Backward );
			kStream.WriteAlignedBytes(&controllerAction,sizeof(unsigned char));
			kStream.WriteAlignedBytes((const unsigned char*)&fValue,sizeof(float));

			dataType = ESerializeDataType_ControllerAction;
			kStream.WriteAlignedBytes(&dataType,sizeof(unsigned char));

			controllerAction = EControllerAction_Forward;
			fValue = pkController->getActionValue( EControllerAction_Forward );
			kStream.WriteAlignedBytes(&controllerAction,sizeof(unsigned char));
			kStream.WriteAlignedBytes((const unsigned char*)&fValue,sizeof(float));

			dataType = ESerializeDataType_ControllerAction;
			kStream.WriteAlignedBytes(&dataType,sizeof(unsigned char));

			controllerAction = EControllerAction_Kick;
			fValue = pkController->getActionValue( EControllerAction_Kick );
			kStream.WriteAlignedBytes(&controllerAction,sizeof(unsigned char));
			kStream.WriteAlignedBytes((const unsigned char*)&fValue,sizeof(float));

			return RakNet::RM3SR_BROADCAST_IDENTICALLY;

		}
	}
	return -1;
}

//-----------------------------------------------------------------------------
void NetworkEntitySerializer::deserialize( RakNet::DeserializeParameters *deserializeParameters )
{
	AbstractNetSerializable* pkNetworkVehicle = dynamic_cast<AbstractNetSerializable*>(this->m_pkEntity);
	if( NULL != pkNetworkVehicle )
	{
		pkNetworkVehicle->deserialize( deserializeParameters );
	}
	else
	{
		SphereObstacle* pkSphere = dynamic_cast<SphereObstacle*>(this->m_pkEntity);
		AbstractLocalSpace* pkLocalSpace = dynamic_cast<AbstractLocalSpace*>(this->m_pkEntity);

		unsigned char controllerAction;
		unsigned char dataTypes = 0; // how many variables to read
		RakNet::BitStream& kStream = deserializeParameters->serializationBitstream[0];
		kStream.ReadAlignedBytes(&dataTypes,sizeof(unsigned char));
		for( unsigned char i = 0; i < dataTypes; ++i )
		{
			OpenSteer::Vec3 kVec;
			float fValue;
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
			case(ESerializeDataType_AngularVelocity):
			case(ESerializeDataType_LinearVelocity):
				{
					kStream.ReadAlignedBytes((unsigned char*)&kVec,sizeof(kVec));
				}
				break;
			case(ESerializeDataType_Radius):
				{
					kStream.ReadAlignedBytes((unsigned char*)&fValue,sizeof(float));
				}
				break;
			case(ESerializeDataType_Speed):
				{
					kStream.ReadAlignedBytes((unsigned char*)&fValue,sizeof(float));
				}
				break;
			case(ESerializeDataType_ControllerAction):
				{
					kStream.ReadAlignedBytes((unsigned char*)&controllerAction,sizeof(unsigned char));
					kStream.ReadAlignedBytes((unsigned char*)&fValue,sizeof(float));
				}
				break;
			default:
				{
					assert( true == false );
				}
				break;
			}

			if( false == NetworkEntitySerializer::setLocalSpaceVariable( eDataType, pkLocalSpace, kVec ) )
			{
				switch( eDataType )
				{
				case(ESerializeDataType_Radius):
					{
						if( NULL != pkSphere )
						{
							pkSphere->radius = fValue;
						}
					}
					break;
				case(ESerializeDataType_ControllerAction):
					{
						osAbstractPlayer* pkPlayer = dynamic_cast<osAbstractPlayer*>(this->m_pkEntity);
						if( NULL != pkPlayer )
						{
							osAbstractController* pkController = pkPlayer->accessController();
							pkController->setActionValue( (EControllerAction)controllerAction, fValue );
						}
					}
					break;

				}
			}
		}

	}
}

//-----------------------------------------------------------------------------
bool NetworkEntitySerializer::setLocalSpaceVariable( 
	ESerializeDataType eVariable, AbstractLocalSpace* pkLocalSpace, const osVector3& kValue )
{
	if( NULL == pkLocalSpace )
	{
		return false;
	}
	LocalSpaceData& kLocalSpace = pkLocalSpace->accessLocalSpaceData();
	if( true == NetworkEntitySerializer::setLocalSpaceDataVariable( eVariable, kLocalSpace, kValue ) )
	{
		switch( eVariable )
		{
		case(ESerializeDataType_Forward):
		case(ESerializeDataType_Side):
		case(ESerializeDataType_Up):
			{
				AbstractVehicle* pkVehicle = dynamic_cast<AbstractVehicle*>(pkLocalSpace);
				if( NULL != pkVehicle )
				{
					pkVehicle->regenerateLocalSpace( pkLocalSpace->forward(), 0 );
				}
			}
			break;
		}
		return true;
	}
	return false;
}

//-----------------------------------------------------------------------------
bool NetworkEntitySerializer::setLocalSpaceDataVariable( 
	ESerializeDataType eVariable, LocalSpaceData& kLocalSpace, const osVector3& kValue )
{
	switch( eVariable )
	{
	case(ESerializeDataType_Position):
		{
			kLocalSpace._position = kValue;
			return true;
		}
		break;
	case(ESerializeDataType_Forward):
		{
			kLocalSpace._forward = kValue;	
			return true;
		}
		break;
	case(ESerializeDataType_Side):
		{
			kLocalSpace._side = kValue;	
			return true;
		}
		break;
	case(ESerializeDataType_Up):
		{
			kLocalSpace._up = kValue;	
			return true;
		}
		break;
	case(ESerializeDataType_AngularVelocity):
		{
			kLocalSpace._angularVelocity = kValue;	
			return true;
		}
		break;
	case(ESerializeDataType_LinearVelocity):
		{
			kLocalSpace._linearVelocity = kValue;	
			return true;
		}
		break;
	}
	return false;
}
//-----------------------------------------------------------------------------
void NetworkEntitySerializer::serializeConstruction(
	RakNet::BitStream *constructionBitstream)
{
	AbstractNetSerializable* pkNetworkVehicle = dynamic_cast<AbstractNetSerializable*>(this->m_pkEntity);
	if( NULL != pkNetworkVehicle )
	{
		pkNetworkVehicle->serializeConstruction( constructionBitstream );
	}
}
//-----------------------------------------------------------------------------
bool NetworkEntitySerializer::deserializeConstruction(
	RakNet::BitStream *constructionBitstream )
{
	AbstractNetSerializable* pkNetworkVehicle = dynamic_cast<AbstractNetSerializable*>(this->m_pkEntity);
	if( NULL != pkNetworkVehicle )
	{
		return pkNetworkVehicle->deserializeConstruction( constructionBitstream );
	}
	return true;
}
