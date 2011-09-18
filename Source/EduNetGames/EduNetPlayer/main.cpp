//-----------------------------------------------------------------------------
// Copyright (c) Jan Fietz, Cyrus Preuss
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

#include "EduNetApplication/EduNetMain.h"
#include "EduNetPlayerPlugin.h"
#include "EduNetCommon/EduNetOptions.h"

namespace EduNet
{	
	ModulePluginLoader* gLoadPlugin = NULL;
	Player::HostPlugin* gHost= NULL;
	void initializeDynamicPlugins( )
	{
		
		EduNet::Options& etOptions = EduNet::Options::accessOptions();
		if (etOptions.Variables().count("module")) 
		{
			etOptions.accessModuleNameList() = etOptions.Variables()["module"].as< enStringArray_t >();				
		}

		if (etOptions.Variables().count("create") && !(EduNet::Options::accessOptions().ListModules()) )
		{
			gHost = ET_NEW Player::HostPlugin(true);
			etOptions.setSelectedPlugin(gHost->name());
		}
		else
		{
			ModuleManager kModuleManager;
			gLoadPlugin = ET_NEW ModulePluginLoader();
			gLoadPlugin->loadModules( kModuleManager.getCurrentModulePath() );
			gLoadPlugin->createPluginsFromModules();
		}		
	}

	void shutdownDynamicPlugins( )
	{	
		if (gLoadPlugin != NULL)
		{
			gLoadPlugin->unloadModules();
		}

		ET_SAFE_DELETE( gLoadPlugin );		 
		ET_SAFE_DELETE( gHost ); 
	}
}
using namespace boost;
//-----------------------------------------------------------------------------
int main (int argc, char **argv)
{
	EduNet::Options& etOptions = EduNet::Options::accessOptions();	
	etOptions.addDefaultOptions();

	program_options::options_description playerOptions("EduNetPlayer options");

	playerOptions.add_options()
		("list,L", "available modules and plugins")
		("module,M", program_options::value< enStringArray_t >(), "specify one or more modules to load")
		("create,C", program_options::value< enStringArray_t  >(), "specify one or more modules which are created on startup")
		;	

	etOptions.descriptions().add(playerOptions);
	return ::EduNetMain( argc, argv );
}



