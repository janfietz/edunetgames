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

#include "EduNetPlayerWx.h"
#include "EduNetPlayerWxFrame.h"

#include "EduNetCommon/EduNetOptions.h"
#include "OpenSteerUT/OpenSteerUT.h"
#include "OpenSteer/PluginRegistry.h"

#include "EduNetApplication/EduNetModulePluginLoader.h"
#include "EduNetApplication/EduNetModuleManager.h"

#include "EduNetApplication/EduNetApplication.h"
#include "EduNetApplication/EduNetProcessProfile.h"
#include "OpenSteer/Draw.h"

#include "wx/dir.h"

namespace EduNet
{
	
	ModulePluginLoader* gLoadPlugin = NULL;
	void initializeDynamicPlugins( )
	{
		ModuleManager kModuleManager;
		gLoadPlugin = ET_NEW ModulePluginLoader();
		gLoadPlugin->loadModules( kModuleManager.getCurrentModulePath() );
		gLoadPlugin->createPluginsFromModules();
	}

	void shutdownDynamicPlugins( )
	{
		gLoadPlugin->unloadModules();
		ET_SAFE_DELETE( gLoadPlugin ); 
	}

	//-----------------------------------------------------------------------------
	bool InitializeGlobals( void )
	{
		static EduNet::ProcessProfile kProfile;
		OpenSteerUTData::_SDMInitApp(&kProfile);

		static OpenSteer::OpenGLRenderer kRenderer;
		OpenSteer::GlobalData::getInstance()->setRenderer( &kRenderer );
		return true;
	}

	bool bGlobalsInitialized = InitializeGlobals();

	//-----------------------------------------------------------------------------
	bool PlayerWx::OnInit()
	{
		if ( !wxApp::OnInit() )
			return false;

		m_spModuleManager = new ModulePluginLoader();
		ModulePluginLoader* pModuleManager = m_spModuleManager.get();
		wxDir dir(wxGetCwd());
		if (dir.IsOpened() == true)
		{
			pModuleManager->loadModules( dir.GetName().c_str() );
		}		
		
		//// register dynamic plugins
		//EduNet::initializeDynamicPlugins( );
		//
		//// check if there is a default plugin
		//const char* pszPluginName = EduNet::Options::accessOptions().getSelectedPlugin();
		//OpenSteer::AbstractPlugin* pkPlugin = Plugin::findByName ( pszPluginName );
		//if ( NULL == pkPlugin )
		//{
		//	pkPlugin = Plugin::findDefault();
		//}

		//if( NULL != pkPlugin )
		//{
		//	Plugin::selectPlugin(pkPlugin);
		//}
		//else
		//{
		//	printf( "no plugin found - shutting down\n" );
		//}		

		// create the main application window
		EduNet::PlayerWxFrame *frame = new EduNet::PlayerWxFrame("EduNet Plugin Player", pModuleManager);
		frame->CreateModuleTree();
		frame->Show(true);
		return true;
	}

	int PlayerWx::OnExit()
	{
		/*EduNet::shutdownDynamicPlugins();

		EduNet::Application::_SDMShutdown();*/

		return wxApp::OnExit();
	}

//-----------------------------------------------------------------------------
	void PlayerWx::run(int argc, char **argv)
	{
		int iExitCode = EXIT_FAILURE;
		EduNet::Application::_SDMInit();
		if( EXIT_SUCCESS == EduNet::Options::accessOptions().parseCommandLine( argc, argv ) )
		{
			if( true == EduNet::Options::accessOptions().continueProcess() )
			{
				if(true == wxEntryStart(argc, argv) )
				{
					wxApp* pApp = static_cast<wxApp*>(wxApp::GetInstance());
					pApp->CallOnInit();
					pApp->OnRun();
					pApp->OnExit();
				}
				wxEntryCleanup();
			}
			else
			{
				iExitCode = EXIT_SUCCESS;
			}
		}
		EduNet::Application::_SDMShutdown();

	}
}
IMPLEMENT_APP_NO_MAIN(EduNet::PlayerWx)