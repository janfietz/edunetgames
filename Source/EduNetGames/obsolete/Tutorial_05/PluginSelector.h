#ifndef __EDUNETPLUGINSELECTOR_H__
#define __EDUNETPLUGINSELECTOR_H__

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

#include "EduNetCommon/EduNetCommon.h"
#include "EduNetConnect/AbstractReplica.h"


//-----------------------------------------------------------------------------
class PluginSelector: public EduNet::AbstractReplica
{
	ET_DECLARE_BASE( EduNet::AbstractReplica )
public:
	virtual RakNet::RakString GetName(void) const
	{
		return "PluginSelector";
	};

	virtual OpenSteer::EntityClassId getClassId( void ) const
	{
		return OS_CID_UNKNOWN;
	}

	PluginSelector(){};

	virtual RakNet::RM3ConstructionState QueryConstruction(
		RakNet::Connection_RM3 *destinationConnection,
		RakNet::ReplicaManager3 *replicaManager3)
	{
		return QueryConstruction_ClientConstruction( destinationConnection );
	}
	
	virtual bool QueryRemoteConstruction( RakNet::Connection_RM3 *sourceConnection )
	{
		return QueryRemoteConstruction_ClientConstruction( sourceConnection );
	}	

	virtual RakNet::RM3QuerySerializationResult QuerySerialization(
		RakNet::Connection_RM3 *destinationConnection)
	{
		return QuerySerialization_ClientSerializable( destinationConnection );
	}

	virtual RakNet::RM3ActionOnPopConnection QueryActionOnPopConnection(
		RakNet::Connection_RM3 *droppedConnection) const
	{
		return QueryActionOnPopConnection_Client( droppedConnection );
	}	

	virtual void PostDeserializeConstruction( RakNet::Connection_RM3 *sourceConnection );

	virtual void DeserializeConstructionRequestAccepted(
		RakNet::BitStream *serializationBitstream,
		RakNet::Connection_RM3 *acceptingConnection);

	void TestRpc( RakNet::RPC3 *rpcFromNetwork  = 0);
	void TestRpcAnswer( RakNet::RPC3 *rpcFromNetwork  = 0);

	void SelectServerPlugin( RakNet::RPC3 *rpcFromNetwork = 0);
	void SelectPlugin( RakNet::RakString kPluginName,
		RakNet::RPC3 *rpcFromNetwork = 0);

	void Initialize(RakNet::RPC3* rpc3Inst,
		OpenSteer::VirtualPluginSelector* pluginSelector);

private:
	RakNet::RPC3* m_rpc3Inst;
	OpenSteer::VirtualPluginSelector* m_pluginSelector;
	
};

//-----------------------------------------------------------------------------
class PluginSelectorClientConnection : public RakNet::Connection_RM3
{
public:
	PluginSelectorClientConnection(RakNet::RPC3* rpc3Inst,
		OpenSteer::VirtualPluginSelector* pluginSelector,
		SystemAddress _systemAddress, RakNetGUID _guid) :
		Connection_RM3(_systemAddress, _guid)
	{
		this->m_pluginSelector = pluginSelector;
		this->m_rpc3Inst = rpc3Inst;
	};
	virtual ~PluginSelectorClientConnection() {}
	
	virtual RakNet::Replica3 *AllocReplica(RakNet::BitStream *allocationId,
		RakNet::ReplicaManager3 *replicaManager3);
private:
	RakNet::RPC3* m_rpc3Inst;
	OpenSteer::VirtualPluginSelector* m_pluginSelector;
};

//-----------------------------------------------------------------------------
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

//-----------------------------------------------------------------------------
class PluginSelectorReplicaManager : public RakNet::ReplicaManager3
{
	virtual RakNet::Connection_RM3* AllocConnection(SystemAddress systemAddress, RakNetGUID rakNetGUID) const 
	{
		if(false == m_bIsClient)
		{
			return ET_NEW PluginSelectorClientConnection(
						this->m_rpc3Inst,
						this->m_pluginSelector,
						systemAddress,
						rakNetGUID);
		}
		return ET_NEW PluginSelectorConnection(
						systemAddress,
						rakNetGUID);
		
	}
	virtual void DeallocConnection(RakNet::Connection_RM3 *connection) const {
		delete connection;
	}
public:
	PluginSelectorReplicaManager( bool bIsClient = false):
	  m_bIsClient(bIsClient), m_rpc3Inst(NULL), m_pluginSelector(NULL){}

	 void Initialize(RakNet::RPC3* rpc3Inst,
		  OpenSteer::VirtualPluginSelector* pluginSelector,
		  bool bIsClient)
	  {
		  this->m_pluginSelector = pluginSelector;
		  this->m_rpc3Inst = rpc3Inst;
		  this->m_bIsClient = bIsClient;
	  }
private:
	bool m_bIsClient;
	RakNet::RPC3* m_rpc3Inst;
	OpenSteer::VirtualPluginSelector* m_pluginSelector;
};

#endif // __EDUNETPLUGINSELECTOR_H__