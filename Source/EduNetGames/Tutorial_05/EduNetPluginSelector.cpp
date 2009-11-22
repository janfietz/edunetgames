#include "EduNetPluginSelector.h"


RakNet::Replica3* PluginSelectorClientConnection::AllocReplica(
	RakNet::BitStream *allocationId,
	RakNet::ReplicaManager3 *replicaManager3)
{
	RakNet::RakString typeName;
	allocationId->Read(typeName);
	OpenSteer::EntityClassId kId;
	allocationId->Read( kId );
	if (typeName=="PluginSelector"){
		printf("Create PluginSelector instance.");
		PluginSelector* pkNewInstance = new PluginSelector();
		pkNewInstance->Initialize( this->m_rpc3Inst, this->m_pkPluginHost);
		return pkNewInstance;
	}	
	return 0;
}
//-----------------------------------------------------------------------------

void PluginSelector::Initialize(RakNet::RPC3* rpc3Inst,
	EduNet::PluginHost* pkPluginHost)
{
	
	this->m_rpc3Inst = rpc3Inst;
	RPC3_REGISTER_FUNCTION(rpc3Inst, &PluginSelector::TestRpc);
	RPC3_REGISTER_FUNCTION(rpc3Inst, &PluginSelector::TestRpcAnswer);
	RPC3_REGISTER_FUNCTION(rpc3Inst, &PluginSelector::SelectServerPlugin);
	RPC3_REGISTER_FUNCTION(rpc3Inst, &PluginSelector::SelectPlugin);

	this->m_pkPluginHost = pkPluginHost;
	
};

void PluginSelector::DeserializeConstructionRequestAccepted(
		RakNet::BitStream *serializationBitstream,
		RakNet::Connection_RM3 *acceptingConnection)
{
	if (this->m_rpc3Inst!=NULL)
	{
		this->TestRpc();
		this->SelectServerPlugin();
	}
}

void PluginSelector::PostDeserializeConstruction(
	RakNet::Connection_RM3 *sourceConnection)
{
	if (this->m_rpc3Inst!=NULL)
	{
		this->TestRpc();
		this->SelectServerPlugin();
	}
}

void PluginSelector::TestRpc( RakNet::RPC3 *rpcFromNetwork )
{
	if (rpcFromNetwork==0)
	{
		printf("\nRakNet::TestRpc called locally\n");
		this->m_rpc3Inst->CallCPP("&PluginSelector::TestRpc", GetNetworkID(), rpcFromNetwork);
	}
	else
	{
		this->TestRpcAnswer(NULL);
		printf("\nRakNet::TestRpc called remote\n");
	}
	
}

void PluginSelector::TestRpcAnswer( RakNet::RPC3 *rpcFromNetwork )
{
	if (rpcFromNetwork==0)
	{
		printf("\nRakNet::TestRpcAnswer called locally\n");
		this->m_rpc3Inst->CallCPP("&PluginSelector::TestRpcAnswer", GetNetworkID(), rpcFromNetwork);
	}
	else
	{
		printf("\nRakNet::TestRpcAnswer called remote\n");
	}	
}

void PluginSelector::SelectServerPlugin( RakNet::RPC3 *rpcFromNetwork )
{
	if (rpcFromNetwork==0)
	{
		this->m_rpc3Inst->CallCPP("&PluginSelector::SelectServerPlugin", GetNetworkID(), rpcFromNetwork);
	} else
	{
		RakNet::RakString rs( this->m_pkPluginHost->getCurrentPluginName() );
		this->m_rpc3Inst->CallCPP("&PluginSelector::SelectPlugin", GetNetworkID(), rs,  rpcFromNetwork);
	}
}

void PluginSelector::SelectPlugin( 
	RakNet::RakString kPluginName,
	RakNet::RPC3 *rpcFromNetwork )
{	
	printf("\nRakNet::SelectPlugin %s called from remote\n",
		kPluginName.C_String());
	this->m_pkPluginHost->SelectPluginByName(kPluginName.C_String());
}