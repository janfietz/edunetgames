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

#include "SerializablePlayer.h"

using namespace OpenSteer;
//-----------------------------------------------------------------------------
AbstractEntity* ClientSerializablePlayer::cloneEntity( void ) const
{
	return ET_NEW ClientSerializablePlayer();
}
//-----------------------------------------------------------------------------
int ClientSerializablePlayer::serialize( 
	RakNet::SerializeParameters *serializeParameters ) const
{ 
	
	return this->serializeController( serializeParameters );	
}
//-----------------------------------------------------------------------------
int ClientSerializablePlayer::serializeController(
	RakNet::SerializeParameters *serializeParameters)const
{
	RakNet::BitStream& kStream = serializeParameters->outputBitstream[0];
	const osAbstractController* pkController = this->getController();
	unsigned char dataTypes = 5; // how many variables will be send

	kStream.WriteAlignedBytes(&dataTypes,sizeof(unsigned char));

	unsigned char dataType;
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
//-----------------------------------------------------------------------------
void ClientSerializablePlayer::deserialize( 
	RakNet::DeserializeParameters *deserializeParameters )
{
	this->deserializeController( deserializeParameters );
}
//-----------------------------------------------------------------------------
void ClientSerializablePlayer::deserializeController(
	RakNet::DeserializeParameters *deserializeParameters)
{	
	unsigned char controllerAction;
	unsigned char dataTypes = 0; // how many variables to read
	RakNet::BitStream& kStream = deserializeParameters->serializationBitstream[0];
	kStream.ReadAlignedBytes(&dataTypes,sizeof(unsigned char));
	for( unsigned char i = 0; i < dataTypes; ++i )
	{
		Vec3 kVec;
		float fValue;
		unsigned char dataType;
		kStream.ReadAlignedBytes(&dataType,sizeof(unsigned char));
		ESerializeDataType eDataType = static_cast<ESerializeDataType>(dataType);
		switch( eDataType )
		{
		
		case(ESerializeDataType_ControllerAction):
			{
				kStream.ReadAlignedBytes((unsigned char*)&controllerAction,sizeof(unsigned char));
				kStream.ReadAlignedBytes((unsigned char*)&fValue,sizeof(float));

				printf( "Read ctrl val: %d : %f\n",
					controllerAction, fValue);
			}
			break;
		default:
			{
				assert( true == false );
			}
			break;
		}

		
		switch( eDataType )
		{
		case(ESerializeDataType_ControllerAction):
			{					
				osAbstractController* pkController = this->accessController();
				pkController->setActionValue( (EControllerAction)controllerAction, fValue );				
			}
			break;

		}	
	}

}
//-----------------------------------------------------------------------------
void ClientSerializablePlayer::serializeConstruction(
	RakNet::BitStream *constructionBitstream)
{

}
//-----------------------------------------------------------------------------
bool ClientSerializablePlayer::deserializeConstruction(
	RakNet::BitStream *constructionBitstream ) 
{
	return true;
}