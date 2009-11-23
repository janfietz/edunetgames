#ifndef __NETBOIDPLUGIN_H__
#define __NETBOIDPLUGIN_H__
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

#include "EduNetConnect/ClientPlugin.h"
#include "EduNetConnect/PeerPlugin.h"

#include "NetBoidReplicaConnection.h"
#include "BoidsPlugin.h"

//-----------------------------------------------------------------------------
class NetPeerBoidPlugin : public PeerPlugin<OpenSteer::BoidsPlugin>
{
	ET_DECLARE_BASE(PeerPlugin<OpenSteer::BoidsPlugin>);
public:
	NetPeerBoidPlugin(bool bAddToRegistry = true);
	virtual ~NetPeerBoidPlugin(){};

	virtual const char* name (void) const {return "NetPeerBoidPlugin";};

	 bool requestInitialSelection (void) const {return true;}
	 virtual float selectionOrderSortKey (void) const { return 2.0f ;}
	 
	virtual bool needRedraw ( void ) const { return false; }
	
	void handleFunctionKeys (int keyNumber);
	virtual void initGui( void* pkUserdata );


	virtual void StartNetworkSession( void );
	virtual void CreateContent( void );
	virtual void DeleteContent( void );
	
	virtual void onChangedReplicationParams(
		const ReplicationParams& kParams );

private:
	
	class NetBoidReplicaFactory* m_pkBoidFactory;
	class NetBoidConditionReplica* m_pkConditionReplic;	
	NetBoidReplicaManager m_kReplicaManager;

	
};

//-----------------------------------------------------------------------------
class NetClientBoidPlugin : public ClientPlugin<OpenSteer::BoidsPlugin>
{
	ET_DECLARE_BASE(ClientPlugin<OpenSteer::BoidsPlugin>);
public:
	NetClientBoidPlugin(bool bAddToRegistry = true);
	virtual ~NetClientBoidPlugin(){};

	virtual const char* name (void) const {return "NetClientBoidPlugin";};

	virtual float selectionOrderSortKey (void) const { return 3.0f ;}

	virtual void StartNetworkSession( void );
	virtual void CreateContent( void );
	virtual void DeleteContent( void );

private:
	
	class NetBoidConditionReplica* m_pkConditionReplic;
	NetBoidReplicaManager m_kReplicaManager;
	

};

#endif //__NETBOIDPLUGIN_H__