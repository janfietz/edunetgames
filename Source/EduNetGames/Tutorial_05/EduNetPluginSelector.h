#ifndef __EDUNETPLUGINSELECTOR_H__
#define __EDUNETPLUGINSELECTOR_H__

#include "EduNet/common/EduNetCommon.h"
#include "EduNet/network/OSReplica.h"
#include "plugins/EduNetPluginHost.h"

#include "Rpc3/RPC3.h"

//-----------------------------------------------------------------------------
class PluginSelector: public OSObjectReplica
{
public:
	virtual RakNet::RakString GetName(void) const
	{
		return "PluginSelector";
	};

	PluginSelector(){};

	virtual RakNet::RM3ConstructionState QueryConstruction(
		RakNet::Connection_RM3 *destinationConnection,
		RakNet::ReplicaManager3 *replicaManager3)
	{
		return QueryConstruction_ServerConstruction(destinationConnection);
	}
	
	virtual bool QueryRemoteConstruction(RakNet::Connection_RM3 *sourceConnection)
	{
		return QueryRemoteConstruction_ServerConstruction(sourceConnection);
	}	

	virtual RakNet::RM3QuerySerializationResult QuerySerialization(
		RakNet::Connection_RM3 *destinationConnection)
	{
		return RakNet::RM3QSR_NEVER_CALL_SERIALIZE;
	}

	virtual RakNet::RM3ActionOnPopConnection QueryActionOnPopConnection(
		RakNet::Connection_RM3 *droppedConnection) const
	{
		return QueryActionOnPopConnection_Server(droppedConnection);
	}

	virtual void PostDeserializeConstruction(RakNet::Connection_RM3 *sourceConnection);

	void TestRpc( RakNet::RPC3 *rpcFromNetwork  = 0);
	void TestRpcAnswer( RakNet::RPC3 *rpcFromNetwork  = 0);

	void SelectServerPlugin( RakNet::RPC3 *rpcFromNetwork = 0);
	void SelectPlugin( RakNet::RakString kPluginName,
		RakNet::RPC3 *rpcFromNetwork = 0);

	void Initialize(RakNet::RPC3* rpc3Inst,
		EduNetGames::PluginHost* pkPluginHost);

private:
	RakNet::RPC3* m_rpc3Inst;
	EduNetGames::PluginHost* m_pkPluginHost;
	
};

// ----------------------------------------------------------------------------
class PluginSelectorClientConnection : public RakNet::Connection_RM3
{
public:
	PluginSelectorClientConnection(RakNet::RPC3* rpc3Inst,
		EduNetGames::PluginHost* pkPluginHost,
		SystemAddress _systemAddress, RakNetGUID _guid) :
		Connection_RM3(_systemAddress, _guid)
	{
		this->m_pkPluginHost = pkPluginHost;
		this->m_rpc3Inst = rpc3Inst;
	};
	virtual ~PluginSelectorClientConnection() {}
	
	virtual RakNet::Replica3 *AllocReplica(RakNet::BitStream *allocationId,
		RakNet::ReplicaManager3 *replicaManager3);
private:
	RakNet::RPC3* m_rpc3Inst;
	EduNetGames::PluginHost* m_pkPluginHost;
};

// ----------------------------------------------------------------------------
class PluginSelectorConnection : public RakNet::Connection_RM3
{
public:
	PluginSelectorConnection( SystemAddress _systemAddress, RakNetGUID _guid) :
		Connection_RM3(_systemAddress, _guid)
	{
		
	};
	virtual ~PluginSelectorConnection() {}
	
	virtual RakNet::Replica3 *AllocReplica(RakNet::BitStream *allocationId,
		RakNet::ReplicaManager3 *replicaManager3)
	{
		return NULL;
	};
};

// ----------------------------------------------------------------------------
class PluginSelectorReplicaManager : public RakNet::ReplicaManager3
{
	virtual RakNet::Connection_RM3* AllocConnection(SystemAddress systemAddress, RakNetGUID rakNetGUID) const 
	{
		if(true == this->m_bIsClient)
		{
			return new PluginSelectorClientConnection(
							this->m_rpc3Inst,
							this->m_pkPluginHost,
							systemAddress,
							rakNetGUID);
		}
		return new PluginSelectorConnection(systemAddress,rakNetGUID);
	}
	virtual void DeallocConnection(RakNet::Connection_RM3 *connection) const {
		delete connection;
	}
public:
	PluginSelectorReplicaManager( bool bIsClient = false):
	  m_bIsClient(bIsClient), m_rpc3Inst(NULL), m_pkPluginHost(NULL){}

	 void Initialize(RakNet::RPC3* rpc3Inst,
		  EduNetGames::PluginHost* pkPluginHost,
		  bool bIsClient)
	  {
		  this->m_pkPluginHost = pkPluginHost;
		  this->m_rpc3Inst = rpc3Inst;
		  this->m_bIsClient = bIsClient;
	  }
private:
	bool m_bIsClient;
	RakNet::RPC3* m_rpc3Inst;
	EduNetGames::PluginHost* m_pkPluginHost;
};

#endif // __EDUNETPLUGINSELECTOR_H__