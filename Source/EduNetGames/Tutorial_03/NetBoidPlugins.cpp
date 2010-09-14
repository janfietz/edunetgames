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

#include "NetBoidPlugins.h"

//-----------------------------------------------------------------------------
void EduNetConnect::queryConnectionsSettings( ConnectSettings& kSettings )
{
	kSettings.uiClientStartPort = CLIENT_PORT;
	kSettings.uiServerStartPort = SERVER_PORT;
	kSettings.sessionPassword = "Tutorial3";
	kSettings.uiPortPongCount = 10;
}

NetBoidRenderOfflinePlugin* netBoidRenderOfflinePlugin = NULL;
NetBoidRenderPeerPlugin* netBoidPeerPlugin = NULL;
NetBoidRenderClientPlugin* netBoidRenderClientPlugin = NULL;
NetBoidClientServerPlugin* clientServerPlugin = NULL;

//-----------------------------------------------------------------------------
namespace EduNet
{
	void initializeDynamicPlugins( )
	{
		netBoidRenderOfflinePlugin = ET_NEW NetBoidRenderOfflinePlugin( true );
		netBoidPeerPlugin = ET_NEW NetBoidRenderPeerPlugin( true );
		netBoidRenderClientPlugin = ET_NEW NetBoidRenderClientPlugin( true );
		clientServerPlugin = ET_NEW NetBoidClientServerPlugin( true );
	}
	void shutdownDynamicPlugins( )
	{
		ET_SAFE_DELETE( netBoidRenderOfflinePlugin );
		ET_SAFE_DELETE( netBoidPeerPlugin );
		ET_SAFE_DELETE( netBoidRenderClientPlugin );
		ET_SAFE_DELETE( clientServerPlugin );
	}
}
//-----------------------------------------------------------------------------
