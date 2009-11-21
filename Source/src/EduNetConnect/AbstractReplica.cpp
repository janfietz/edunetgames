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

#include "AbstractReplica.h"

using namespace EduNet;

//-----------------------------------------------------------------------------
void AbstractReplica::WriteAllocationID( RakNet::BitStream *allocationIdBitstream ) const 
{
	allocationIdBitstream->Write( GetName() );
}

//-----------------------------------------------------------------------------
void AbstractReplica::PrintOutput( RakNet::BitStream *bs )
{
	if (bs->GetNumberOfBitsUsed()==0)
		return;
	RakNet::RakString rakString;
	bs->Read( rakString );
	printf("Receive: %s\n", rakString.C_String());
}

//-----------------------------------------------------------------------------
void AbstractReplica::SerializeConstruction(RakNet::BitStream *constructionBitstream, RakNet::Connection_RM3 *destinationConnection)	
{
	constructionBitstream->Write( GetName() + RakNet::RakString(" SerializeConstruction") );
}

//-----------------------------------------------------------------------------
bool AbstractReplica::DeserializeConstruction(RakNet::BitStream *constructionBitstream, RakNet::Connection_RM3 *sourceConnection) 
{
	PrintOutput( constructionBitstream );
	return true;
}

//-----------------------------------------------------------------------------
void AbstractReplica::SerializeDestruction(RakNet::BitStream *destructionBitstream, RakNet::Connection_RM3 *destinationConnection)	
{
	destructionBitstream->Write( GetName() + RakNet::RakString(" SerializeDestruction") );
}

//-----------------------------------------------------------------------------
bool AbstractReplica::DeserializeDestruction(RakNet::BitStream *destructionBitstream, RakNet::Connection_RM3 *sourceConnection) 
{
	PrintOutput(destructionBitstream);
	return true;
}

//-----------------------------------------------------------------------------
void AbstractReplica::DeallocReplica(RakNet::Connection_RM3 *sourceConnection) 
{
	ET_DELETE this;
}

//-----------------------------------------------------------------------------
RakNet::RM3SerializationResult AbstractReplica::Serialize(RakNet::SerializeParameters *serializeParameters)	
{
	RakNetTime time = RakNet::GetTime()/1000;

	if (time%2) // Swap channels randomly for testing
		serializeParameters->outputBitstream[0].Write( GetName() + RakNet::RakString(" Channel 0. Serialize. Time = %i", time));
	else
		serializeParameters->outputBitstream[1].Write( GetName() + RakNet::RakString(" Channel 1. Serialize. Time = %i", time));
	return RakNet::RM3SR_BROADCAST_IDENTICALLY;
}

//-----------------------------------------------------------------------------
void AbstractReplica::Deserialize(RakNet::DeserializeParameters *deserializeParameters) 
{
	PrintOutput( &deserializeParameters->serializationBitstream[0] );
	PrintOutput( &deserializeParameters->serializationBitstream[1] );
}

//-----------------------------------------------------------------------------
void AbstractReplica::SerializeConstructionRequestAccepted(RakNet::BitStream *serializationBitstream, RakNet::Connection_RM3 *requestingConnection)	
{
	serializationBitstream->Write(GetName() + RakNet::RakString(" SerializeConstructionRequestAccepted"));
}

//-----------------------------------------------------------------------------
void AbstractReplica::DeserializeConstructionRequestAccepted(RakNet::BitStream *serializationBitstream, RakNet::Connection_RM3 *acceptingConnection) 
{
	PrintOutput(serializationBitstream);
}

//-----------------------------------------------------------------------------
void AbstractReplica::SerializeConstructionRequestRejected(RakNet::BitStream *serializationBitstream, RakNet::Connection_RM3 *requestingConnection)	
{
	serializationBitstream->Write(GetName() + RakNet::RakString(" SerializeConstructionRequestRejected"));
}

//-----------------------------------------------------------------------------
void AbstractReplica::DeserializeConstructionRequestRejected(RakNet::BitStream *serializationBitstream, RakNet::Connection_RM3 *rejectingConnection) 
{
	PrintOutput(serializationBitstream);
}

