#ifndef __OSREPLICA_H__
#define __OSREPLICA_H__

#include "EduNetCommon/EduNetCommon.h"

class OSObjectReplica : public RakNet::Replica3
{
public:
	virtual RakNet::RakString GetName(void) const=0;
	virtual void WriteAllocationID(RakNet::BitStream *allocationIdBitstream) const {
		allocationIdBitstream->Write(GetName());
	}
	void PrintOutput(RakNet::BitStream *bs)
	{
		if (bs->GetNumberOfBitsUsed()==0)
			return;
		RakNet::RakString rakString;
		bs->Read(rakString);
		printf("Receive: %s\n", rakString.C_String());
	}
	virtual void SerializeConstruction(RakNet::BitStream *constructionBitstream, RakNet::Connection_RM3 *destinationConnection)	{
		constructionBitstream->Write(GetName() + RakNet::RakString(" SerializeConstruction"));
	}
	virtual bool DeserializeConstruction(RakNet::BitStream *constructionBitstream, RakNet::Connection_RM3 *sourceConnection) {
		PrintOutput(constructionBitstream);
		return true;
	}
	virtual void SerializeDestruction(RakNet::BitStream *destructionBitstream, RakNet::Connection_RM3 *destinationConnection)	{
		destructionBitstream->Write(GetName() + RakNet::RakString(" SerializeDestruction"));
	}
	virtual bool DeserializeDestruction(RakNet::BitStream *destructionBitstream, RakNet::Connection_RM3 *sourceConnection) {
		PrintOutput(destructionBitstream);
		return true;
	}
	virtual void DeallocReplica(RakNet::Connection_RM3 *sourceConnection) {
		delete this;
	}
	virtual RakNet::RM3SerializationResult Serialize(RakNet::SerializeParameters *serializeParameters)	{
		RakNetTime time = RakNet::GetTime()/1000;

		if (time%2) // Swap channels randomly for testing
			serializeParameters->outputBitstream[0].Write(GetName() + RakNet::RakString(" Channel 0. Serialize. Time = %i", time));
		else
			serializeParameters->outputBitstream[1].Write(GetName() + RakNet::RakString(" Channel 1. Serialize. Time = %i", time));
		return RakNet::RM3SR_BROADCAST_IDENTICALLY;
	}
	virtual void Deserialize(RakNet::DeserializeParameters *deserializeParameters) {
		PrintOutput(&deserializeParameters->serializationBitstream[0]);
		PrintOutput(&deserializeParameters->serializationBitstream[1]);
	}

	virtual void SerializeConstructionRequestAccepted(RakNet::BitStream *serializationBitstream, RakNet::Connection_RM3 *requestingConnection)	{
		serializationBitstream->Write(GetName() + RakNet::RakString(" SerializeConstructionRequestAccepted"));
	}
	virtual void DeserializeConstructionRequestAccepted(RakNet::BitStream *serializationBitstream, RakNet::Connection_RM3 *acceptingConnection) {
		PrintOutput(serializationBitstream);
	}
	virtual void SerializeConstructionRequestRejected(RakNet::BitStream *serializationBitstream, RakNet::Connection_RM3 *requestingConnection)	{
		serializationBitstream->Write(GetName() + RakNet::RakString(" SerializeConstructionRequestRejected"));
	}
	virtual void DeserializeConstructionRequestRejected(RakNet::BitStream *serializationBitstream, RakNet::Connection_RM3 *rejectingConnection) {
		PrintOutput(serializationBitstream);
	}

};


#endif //__OSREPLICA_H__