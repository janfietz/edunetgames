#define EDUNET_SHOW_CONFIG 1
#include "EduNetGames.h"
#include "EduNetApplication.h"
#include "EduNetCommon/EduNetOptions.h"

// To include EXIT_SUCCESS
#include <cstdlib>

void etMemoryDebugBegin();
void etMemoryDebugEnd();

//-----------------------------------------------------------------------------
class etMemoryDebug
{
public:
	etMemoryDebug( void )
	{
		etMemoryDebugBegin();
	}
	virtual ~etMemoryDebug( void )
	{
		etMemoryDebugEnd();
	}

	void exitMain( void )
	{
		printf( "exit main." );
	}
};

// install memory debugging facilities
etMemoryDebug g_MemoryDebug;

//-----------------------------------------------------------------------------
EF_FORCEINLINE
void etMemoryDebugBegin()
{
#ifdef ET_DEBUG
#ifdef WIN32
	int tmpDbgFlag;
	tmpDbgFlag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
	tmpDbgFlag |= _CRTDBG_ALLOC_MEM_DF;
	tmpDbgFlag |= _CRTDBG_LEAK_CHECK_DF;
	_CrtSetDbgFlag(tmpDbgFlag);
	// uncomment this line to see a sample leak output in the output
	// console at program exit
	// int* piaLeak = new int[10];
	//_CrtSetBreakAlloc(5207700);
#endif
#endif
}

//-----------------------------------------------------------------------------
EF_FORCEINLINE
void etMemoryDebugEnd()
{
#ifdef ET_DEBUG
#ifdef WIN32
	_CrtSetReportMode( _CRT_WARN, _CRTDBG_MODE_DEBUG);
	_CrtSetReportMode( _CRT_ERROR, _CRTDBG_MODE_DEBUG );

	//	_CrtDumpMemoryLeaks();
#endif
#endif
}

//-----------------------------------------------------------------------------
int EduNetMain (int argc, char **argv)
{
	int iExitCode = EXIT_FAILURE;
	EduNet::Application::_SDMInit();
	if( EXIT_SUCCESS == EduNetOptions::accessOptions().parseCommandLine( argc, argv ) )
	{
		if( true ==  EduNetOptions::accessOptions().continueProcess() )
		{
			//EduNet::Application* pApp = EduNet::Application::accessInstance();

			// TODO: @JF @CP this is bad design
			//       lets talk about this
			EduNet::initializeStaticPlugins( );
			// load modules from working dir
			/*pApp->loadModules( "./" );
			pApp->createPluginsFromModules();*/

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
	g_MemoryDebug.exitMain();
	return iExitCode;
}


