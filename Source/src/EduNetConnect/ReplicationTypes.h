#ifndef __REPLICATIONTYPES_H__
#define __REPLICATIONTYPES_H__

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

#define EDUNET_NO_OPENSTEER_INCLUDES 1 // do not include opensteer
#include "EduNetConnect/EduNetConnect.h"

#define CLIENT_PORT  23456
#define SERVER_PORT  12345

#define ET_DEFAULT_REPLICATION_FPS 25
#define ET_DEFAULT_REPLICATION_INTERVAL 1000 / ET_DEFAULT_REPLICATION_FPS

//-----------------------------------------------------------------------------
enum ENetworkSessionType
{
	ENetworkSessionType_Undefined,
	ENetworkSessionType_Client,
	ENetworkSessionType_Peer,
	ENetworkSessionType_Count
};


//-----------------------------------------------------------------------------
typedef struct ReplicationParams_t
{
public:
	ReplicationParams_t():
	  fReplicationFrameRate( ET_DEFAULT_REPLICATION_FPS ),
		  interval(ET_DEFAULT_REPLICATION_INTERVAL){}		
	  RakNet::PRO sendParameter;
	  float fReplicationFrameRate;
	  RakNetTime interval;
} ReplicationParams;

//-----------------------------------------------------------------------------
typedef struct ConnectSettings_t
{
public:
	unsigned int uiServerStartPort;
	unsigned int uiClientStartPort;
	unsigned int uiPortPongCount;
	RakNet::RakString sessionPassword;
}ConnectSettings;


//-----------------------------------------------------------------------------
typedef struct NetworkStats_t
{
	NetworkStats_t():
m_uiBytesReceived(0),
m_uiBytesSend(0)
{
}

void reset()
{
	m_uiBytesReceived =	m_uiBytesSend =	0;
}

size_t m_uiBytesReceived;
size_t m_uiBytesSend;
} NetworkStats;

//-----------------------------------------------------------------------------
typedef struct NetworkAddress_t
{
public:
	NetworkAddress_t():
	  addressString("127.0.0.1"),
		  port(SERVER_PORT){}
	  RakNet::RakString addressString;
	  unsigned short port;
}NetworkAddress;

//-----------------------------------------------------------------------------
typedef struct NetworkSimulatorData_t
{
public:
	NetworkSimulatorData_t():
	  enabled(0),
		  packetloss(0.0f),
		  minExtraPing(0),
		  extraPingVariance(0){}
	  int enabled;
	  float packetloss;
	  int minExtraPing;
	  int extraPingVariance;
}NetworkSimulatorData;


#endif // __REPLICATIONTYPES_H__