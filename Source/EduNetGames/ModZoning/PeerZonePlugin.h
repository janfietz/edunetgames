#ifndef __PEERZONEPLUGIN_H__
#define __PEERZONEPLUGIN_H__
//-----------------------------------------------------------------------------
// Copyright (c) 2011, Jan Fietz, Cyrus Preuss
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

#include "EduNetConnect/PeerPlugin.h"
#include "EduNetConnect/AbstractEntityReplicaConnection.h"
#include "EduNetGames/Pedestrians/NetPedestrianFactory.h"
#include "MasterZonePlugin.h"

namespace OpenSteer	{
	class SimpleNetworkVehicle;
}

//-----------------------------------------------------------------------------
typedef PeerPlugin<EduNet::MasterZonePlugin> PeerZonePlugin_t;

//-----------------------------------------------------------------------------
class PeerZonePlugin : public PeerZonePlugin_t
{
	ET_DECLARE_BASE(PeerZonePlugin_t)
public:
	PeerZonePlugin( bool bAddToRegistry = false );

	virtual const char* name() const;

	virtual void prepareOpen( void ); 

	void setZoneId( size_t zoneId ); 
	
	virtual void CreateContent( void );
	virtual void DeleteContent( void );

	virtual AbstractEntityReplica* allocEntityReplica(
		OpenSteer::AbstractPlugin* pPlugin,
		OpenSteer::EntityClassId classId,
		bool bIsRemoteObject,  bool bClientReplica ) const;
	
	virtual OpenSteer::AbstractEntityFactory* getGamePluginEntityFactory( void ) const;
private:
	void initializeReplication();
	void shutdownReplication();
	AbstractEntityReplicaFactory* m_pkNetPedestrianFactory;
	AbstractEntityReplicaManager* m_pkReplicaManager;
	// factory to create pedestrian vehicles
	mutable NetPedestrianFactory m_kOfflinePedestrianFactory;
};


#endif //__PEERZONEPLUGIN_H__
