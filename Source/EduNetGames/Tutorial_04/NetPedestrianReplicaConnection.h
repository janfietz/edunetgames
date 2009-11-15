#ifndef __NETPEDESTRIANREPLICACONNECTION_H__
#define __NETPEDESTRIANREPLICACONNECTION_H__

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

#include "OpenSteerUT/AbstractPluginUtilities.h"

#include "NetPedestrianFactory.h"

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
class NetPedestrianReplicaConnection : public RakNet::Connection_RM3 , public OpenSteer::TPluginAccessor
{
public:
	NetPedestrianReplicaConnection(SystemAddress _systemAddress, RakNetGUID _guid,
	class OpenSteer::AbstractPlugin* pkAbstractPlugin) :
		RakNet::Connection_RM3( _systemAddress, _guid )
	{
		this->setPlugin( pkAbstractPlugin );
	};
	virtual ~NetPedestrianReplicaConnection() {}

	virtual RakNet::Replica3 *AllocReplica(RakNet::BitStream *allocationId,
		RakNet::ReplicaManager3 *replicaManager3);
};

//-----------------------------------------------------------------------------
typedef OpenSteer::PluginAccessorMixin<RakNet::ReplicaManager3> TPluginReplicaManager;
//-----------------------------------------------------------------------------
class NetPedestrianReplicaManager : public TPluginReplicaManager
{
	// replica manager interface
	virtual RakNet::Connection_RM3* AllocConnection(SystemAddress systemAddress, RakNetGUID rakNetGUID) const 
	{
		return new NetPedestrianReplicaConnection( systemAddress,rakNetGUID, this->getPlugin() );
	}
	virtual void DeallocConnection(RakNet::Connection_RM3 *connection) const {
		delete connection;
	}	
};

//-----------------------------------------------------------------------------
class NetPedestrianReplicaFactory : public NetPedestrianFactory
{
	ET_DECLARE_BASE( NetPedestrianFactory );
public:
	NetPedestrianReplicaFactory(NetPedestrianReplicaManager* pkManager):
	  m_pkReplicaManager(pkManager){}
	  
	  virtual OpenSteer::AbstractVehicle* createVehicle( OpenSteer::EntityClassId classId, OpenSteer::ProximityDatabase* pkProximityDatabase ) const;
	  virtual void destroyVehicle( OpenSteer::AbstractVehicle* pkVehicle ) const;

private:

	class NetPedestrianReplicaManager* m_pkReplicaManager;
	mutable DataStructures::Map<OpenSteer::InstanceTracker::Id, RakNet::Replica3* > m_uidMap;
};


#endif // __NETPEDESTRIANREPLICACONNECTION_H__