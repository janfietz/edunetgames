#ifndef __NETSOCCERPEERPLUGIN_H__
#define __NETSOCCERPEERPLUGIN_H__
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
#include "NetSoccerPlugin.h"
#include "EduNetConnect/PeerPlugin.h"

#include "EduNetConnect/SerializablePlayer.h"
#include "EduNetConnect/AbstractEntityReplicaConnection.h"
#include "EduNetConnect/SimpleNetworkVehicle.h"

#include "OpenSteer/GlobalData.h"

typedef PeerPlugin<NetSoccerPlugin> TSoccerPeerPlugin;

using namespace OpenSteer;
//-----------------------------------------------------------------------------
class SoccerPeerPlugin : public TSoccerPeerPlugin
{
	ET_DECLARE_BASE(TSoccerPeerPlugin)
public:
 SoccerPeerPlugin ( bool bAddToRegistry = true ) :
            BaseClass ( bAddToRegistry ),
            m_kReplicaManager ( NULL ),
            m_pkNetBoidFactory ( NULL )
    {
		GlobalData::getInstance()->m_bReplicationDataConfig[ESerializeDataType_Forward] = 0;
		GlobalData::getInstance()->m_bReplicationDataConfig[ESerializeDataType_Orientation] = 1;
		GlobalData::getInstance()->m_bReplicationDataConfig[ESerializeDataType_AngularVelocity] = 1;
		GlobalData::getInstance()->m_bReplicationDataConfig[ESerializeDataType_LinearVelocity] = 1;
    }

    OS_IMPLEMENT_CLASSNAME ( SoccerPeerPlugin )
    virtual const char* name() const
    {
        return this->getClassName();
    };

    //-------------------------------------------------------------------------
    virtual void StartNetworkSession ( void );
    virtual void StopNetworkSession ( void );

    virtual void CreateContent ( void );
    virtual void DeleteContent ( void );

    virtual AbstractEntityReplica* allocEntityReplica (
        OpenSteer::AbstractPlugin* pPlugin,
        OpenSteer::EntityClassId classId,
        bool bIsRemoteObject,  bool bClientReplica ) const;

private:
	AbstractEntityReplicaFactory* m_pkNetBoidFactory;
	AbstractEntityReplicaManager* m_kReplicaManager;
};


#endif // __NETSOCCERPEERPLUGIN_H__
