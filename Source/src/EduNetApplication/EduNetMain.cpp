#define EDUNET_SHOW_CONFIG 1
#include "EduNetGames.h"
#include "EduNetApplication.h"
#include "EduNetCommon/EduNetOptions.h"

// To include EXIT_SUCCESS
#include <cstdlib>

void etMemoryDebugBegin();
void etMemoryDebugEnd();

//-----------------------------------------------------------------------------
EF_FORCEINLINE 
void etMemoryDebugBegin()
{
#ifdef _DEBUG
	int tmpDbgFlag;
	tmpDbgFlag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
	tmpDbgFlag |= _CRTDBG_ALLOC_MEM_DF;
	tmpDbgFlag |= _CRTDBG_LEAK_CHECK_DF;
	_CrtSetDbgFlag(tmpDbgFlag);
	// uncomment this line to see a sample leak output in the output
	// console at program exit
	//int* piaLeak = new int[10];
	//_CrtSetBreakAlloc(5207700);
#endif
}

//-----------------------------------------------------------------------------
EF_FORCEINLINE 
void etMemoryDebugEnd()
{
#ifdef _DEBUG
	_CrtSetReportMode( _CRT_WARN, _CRTDBG_MODE_DEBUG);
	_CrtSetReportMode( _CRT_ERROR, _CRTDBG_MODE_DEBUG );
	//	_CrtDumpMemoryLeaks();
#endif
}

//-----------------------------------------------------------------------------
int EduNetMain (int argc, char **argv) 
{
	::etMemoryDebugBegin();
	int iExitCode = EXIT_FAILURE;
	EduNet::Application::_SDMInit();
	if( EXIT_SUCCESS == EduNetOptions::accessOptions().parseCommandLine( argc, argv ) )
	{
		if( true ==  EduNetOptions::accessOptions().continueProcess() )
		{
			// initialize graphics
			OpenSteer::OpenSteerDemo::initializeGraphics (argc, argv);

			// initialize OpenSteerDemo application
			OpenSteer::OpenSteerDemo::initialize ();

			// run the main event processing loop
			OpenSteer::OpenSteerDemo::runGraphics ();  
		}
		iExitCode = EXIT_SUCCESS;
	}
	EduNet::Application::_SDMShutdown();
	::etMemoryDebugEnd();
	return iExitCode;
}


