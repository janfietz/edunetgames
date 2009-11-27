#include "NetBoidConditionReplica.h"


//-----------------------------------------------------------------------------
RakNet::RM3SerializationResult NetBoidConditionReplica::Serialize(
		RakNet::SerializeParameters *serializeParameters)
{	
	if(false == this->m_pBoidPlugin->WasBoundaryConditionChangedLocally())
	{
		return RakNet::RM3SR_DO_NOT_SERIALIZE;
	}
	RakNet::BitStream& kStream = serializeParameters->outputBitstream[0];
	kStream.Write((int)this->m_pBoidPlugin->GetCurrentBoundaryCondition() );
	return RakNet::RM3SR_BROADCAST_IDENTICALLY;
}

//-----------------------------------------------------------------------------
void NetBoidConditionReplica::Deserialize(
		RakNet::DeserializeParameters *deserializeParameters)
{
	int i;
	RakNet::BitStream& kStream = deserializeParameters->serializationBitstream[0];
	kStream.Read(i);
	this->m_pBoidPlugin->SetCurrentBoundaryCondition(
		(OpenSteer::EBoidConstraintType)i, false);
}
//-----------------------------------------------------------------------------
void NetBoidConditionReplica::SerializeConstructionExisting(
		RakNet::BitStream *constructionBitstream, 
		RakNet::Connection_RM3 *destinationConnection)
{
	constructionBitstream->Write(GetName() + RakNet::RakString(" SerializeConstructionExisting"));
}

//-----------------------------------------------------------------------------
void NetBoidConditionReplica::DeserializeConstructionExisting(
		RakNet::BitStream *constructionBitstream,
		RakNet::Connection_RM3 *sourceConnection)
{
	PrintOutput(constructionBitstream);
}

//-----------------------------------------------------------------------------
void NetBoidConditionReplica::SerializeConstructionRequestAccepted(
		RakNet::BitStream *serializationBitstream,
		RakNet::Connection_RM3 *requestingConnection)
{	
	serializationBitstream->Write((int)this->m_pBoidPlugin->GetCurrentBoundaryCondition() );
}

//-----------------------------------------------------------------------------
void NetBoidConditionReplica::DeserializeConstructionRequestAccepted(
		RakNet::BitStream *serializationBitstream,
		RakNet::Connection_RM3 *acceptingConnection)
{
	int i;
	serializationBitstream->Read(i);
	this->m_pBoidPlugin->SetCurrentBoundaryCondition(
		(OpenSteer::EBoidConstraintType)i, false);
}

//-----------------------------------------------------------------------------
RakNet::RM3QuerySerializationResult NetBoidConditionReplica::QuerySerialization(
		RakNet::Connection_RM3 *destinationConnection)
{
	if(false == this->m_pBoidPlugin->WasBoundaryConditionChangedLocally())
	{
		return RakNet::RM3QSR_DO_NOT_CALL_SERIALIZE;
	}
	return RakNet::RM3QSR_CALL_SERIALIZE;
}
