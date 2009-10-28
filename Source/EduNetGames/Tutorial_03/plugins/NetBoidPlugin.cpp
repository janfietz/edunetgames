#include "NetBoidPlugin.h"
#include "EduNet/network/OSReplicaTypes.h"
#include "OpenSteerExtras/AbstractVehicleGroup.h"

NetPeerBoidPlugin gNetPeerBoidPlugin;
NetClientBoidPlugin gNetClientBoidPlugin;

//-----------------------------------------------------------------------------
RakNet::Replica3* BoidReplicaConnection::AllocReplica(
	RakNet::BitStream *allocationId,
	RakNet::ReplicaManager3 *replicaManager3)
{
	RakNet::RakString typeName;
	allocationId->Read(typeName);
	if (typeName=="BoidReplica"){
		BoidReplica* pkNewReplica = new BoidReplica( this->m_pBoidPlugin  );
		OpenSteer::AbstractVehicleGroup kVG( this->m_pBoidPlugin->allVehicles() );
		kVG.addVehicle( pkNewReplica->AccessVehicle() );
//		this->m_pBoidPlugin->AddBoidToFlock( pkNewReplica->AccessVehicle() );
		return pkNewReplica; 
	}
	if (typeName=="BoidCondition"){
		BoidConditionReplica* pkNewReplica = new BoidConditionReplica( this->m_pBoidPlugin  );
		return pkNewReplica; 
	}
	return 0;
}

//-----------------------------------------------------------------------------
OpenSteer::Boid* BoidReplicaFactory::CreateBoid( 
	OpenSteer::ProximityDatabase& pd )	
{	
	BoidReplica* pkNewReplica = new BoidReplica( pd );		
	this->m_pkReplicaManager->Reference(pkNewReplica);

	this->m_uidMap.Set((unsigned int)pkNewReplica->AccessVehicle(), pkNewReplica);
	return pkNewReplica->AccessVehicle();
}
//-----------------------------------------------------------------------------
void BoidReplicaFactory::DestroyBoid( const OpenSteer::Boid* pkBoid)
{	
	unsigned int uiBoidAddress = (unsigned int)pkBoid;	
	if(true == this->m_uidMap.Has(uiBoidAddress))
	{
		RakNet::Replica3* pReplicaObject = this->m_uidMap.Get( uiBoidAddress);		
		this->m_pkReplicaManager->BroadcastDestruction( pReplicaObject, UNASSIGNED_SYSTEM_ADDRESS);
		this->m_uidMap.Set(uiBoidAddress, NULL);
		delete pReplicaObject;
	}

}
//-----------------------------------------------------------------------------
RakNet::RM3SerializationResult BoidConditionReplica::Serialize(
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
void BoidConditionReplica::Deserialize(
		RakNet::DeserializeParameters *deserializeParameters)
{
	int i;
	RakNet::BitStream& kStream = deserializeParameters->serializationBitstream[0];
	kStream.Read(i);
	this->m_pBoidPlugin->SetCurrentBoundaryCondition(
		(OpenSteer::EBoidConstraintType)i, false);
}
void BoidConditionReplica::SerializeConstructionExisting(
		RakNet::BitStream *constructionBitstream, 
		RakNet::Connection_RM3 *destinationConnection)
{
	constructionBitstream->Write(GetName() + RakNet::RakString(" SerializeConstructionExisting"));
}


void BoidConditionReplica::DeserializeConstructionExisting(
		RakNet::BitStream *constructionBitstream,
		RakNet::Connection_RM3 *sourceConnection)
{
	PrintOutput(constructionBitstream);
}

void BoidConditionReplica::SerializeConstructionRequestAccepted(
		RakNet::BitStream *serializationBitstream,
		RakNet::Connection_RM3 *requestingConnection)
{	
	serializationBitstream->Write((int)this->m_pBoidPlugin->GetCurrentBoundaryCondition() );
}

void BoidConditionReplica::DeserializeConstructionRequestAccepted(
		RakNet::BitStream *serializationBitstream,
		RakNet::Connection_RM3 *acceptingConnection)
{
	int i;
	serializationBitstream->Read(i);
	this->m_pBoidPlugin->SetCurrentBoundaryCondition(
		(OpenSteer::EBoidConstraintType)i, false);
}

RakNet::RM3QuerySerializationResult BoidConditionReplica::QuerySerialization(
		RakNet::Connection_RM3 *destinationConnection)
{
	if(false == this->m_pBoidPlugin->WasBoundaryConditionChangedLocally())
	{
		return RakNet::RM3QSR_DO_NOT_CALL_SERIALIZE;
	}
	return RakNet::RM3QSR_CALL_SERIALIZE;
}

//-----------------------------------------------------------------------------
NetPeerBoidPlugin::NetPeerBoidPlugin()
{
	this->m_kReplicaManager.SetPlugin(&this->m_kGamePlugIn);

	this->m_pkBoidFactory = new BoidReplicaFactory(&this->m_kReplicaManager);	
	this->m_kGamePlugIn.SetBoidFactory( this->m_pkBoidFactory );
}
//-----------------------------------------------------------------------------
void NetPeerBoidPlugin::StartNetworkSession( void )
{
	PeerPlugin<OpenSteer::BoidsPlugIn>::StartNetworkSession();
	this->m_pNetInterface->AttachPlugin(&this->m_kReplicaManager);
}

void NetPeerBoidPlugin::CreateContent( void )
{
	PeerPlugin<OpenSteer::BoidsPlugIn>::CreateContent();

	m_pkConditionReplic = 
		new BoidConditionReplica(&this->m_kGamePlugIn);
	m_kReplicaManager.Reference(m_pkConditionReplic);
}

void NetPeerBoidPlugin::DeleteContent( void )
{	
	m_kReplicaManager.Dereference(m_pkConditionReplic);
	delete m_pkConditionReplic;	
	PeerPlugin<OpenSteer::BoidsPlugIn>::DeleteContent();
}
//-----------------------------------------------------------------------------
NetClientBoidPlugin::NetClientBoidPlugin()
{
	this->m_kReplicaManager.SetPlugin(&this->m_kGamePlugIn);

	this->m_pkBoidFactory = new BoidDummyFactory(&this->m_kReplicaManager);	
	this->m_kGamePlugIn.SetBoidFactory( this->m_pkBoidFactory );
}
//-----------------------------------------------------------------------------
void NetClientBoidPlugin::StartNetworkSession( void )
{
	ClientPlugin<OpenSteer::BoidsPlugIn>::StartNetworkSession();
	this->m_pNetInterface->AttachPlugin(&this->m_kReplicaManager);
}

void NetClientBoidPlugin::CreateContent( void )
{
	ClientPlugin<OpenSteer::BoidsPlugIn>::CreateContent();

	m_pkConditionReplic = 
		new BoidConditionReplica(&this->m_kGamePlugIn);
	m_kReplicaManager.Reference(m_pkConditionReplic);
}

void NetClientBoidPlugin::DeleteContent( void )
{	
	m_kReplicaManager.Dereference(m_pkConditionReplic);
	delete m_pkConditionReplic;	
	ClientPlugin<OpenSteer::BoidsPlugIn>::DeleteContent();
}