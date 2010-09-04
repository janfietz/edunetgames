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

#include "NetSoccerModule.h"
#include "NetSoccerPlugins.h"

//-----------------------------------------------------------------------------
NetSoccerPluginFactory::NetSoccerPluginFactory()
{

}
//-----------------------------------------------------------------------------
NetSoccerPluginFactory::~NetSoccerPluginFactory()
{

}
//-----------------------------------------------------------------------------
void NetSoccerPluginFactory::fillStringArrayWithPluginName( EdutNetStringList& kNames ) const
{
	kNames.push_back("SoccerOffline");
	kNames.push_back("SoccerPeer");
	kNames.push_back("SoccerClient");
	kNames.push_back("SoccerClientServer");
}

//-----------------------------------------------------------------------------
OpenSteer::AbstractPlugin* NetSoccerPluginFactory::createPluginByNameInternal(
	const char* pszName ) const
{
	std::string kName(pszName);
	if (0 == kName.compare("SoccerOffline"))
	{
		return ET_NEW NetSoccerRenderOfflinePlugin(false);
	}
	if (0 == kName.compare("SoccerPeer"))
	{
		return ET_NEW RenderSoccerPeerPlugin(false);
	}
	if (0 == kName.compare("SoccerClient"))
	{
		return ET_NEW RenderSoccerClientPlugin(false);
	}
	if (0 == kName.compare("SoccerClientServer"))
	{
		return ET_NEW SoccerClientServerPlugin(false);
	}
	return NULL;
}

//-----------------------------------------------------------------------------
const char* NetSoccerModuleFactory::getName( void ) const
{
	return "netsoccer";
}

//-----------------------------------------------------------------------------
const char* NetSoccerModuleFactory::getAbout( void ) const
{
	return "Provides plugins fo a distributed soccer game";
}

//-----------------------------------------------------------------------------
EduNetPluginFactory* NetSoccerModuleFactory::createPluginFactory( void ) const
{
	return ET_NEW NetSoccerPluginFactory();
}
ET_IMPLEMENT_MODULE_ENTRYFUNC(ModuleEntry_NetSoccer, NetSoccerModuleFactory)