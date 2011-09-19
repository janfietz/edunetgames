#define EDUNET_SHOW_CONFIG 1
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
		printf( "exit main.\n" );
	}
};


//-----------------------------------------------------------------------------
int __cdecl g_DebugAllocHook(
	int      nAllocType,
	void   * pvData,
	size_t   nSize,
	int      nBlockUse,
	long     lRequest,
	const unsigned char * szFileName,
	int      nLine
	)
{
	if( nSize == 8 )
	{
		bool bTest = true;
		bTest = false;
	}
	else if( nSize == 32 )
	{
		bool bTest = true;
		bTest = false;
	}
	else if( nSize == 48 )
	{
		bool bTest = true;
		bTest = false;
	}
	else if( nSize == 408 )
	{
		bool bTest = true;
		bTest = false;
	}
	else if( nSize == 588 )
	{
		bool bTest = true;
		bTest = false;
	}
	else if( nSize == 1200 )
	{
		bool bTest = true;
		bTest = false;
	}
	else if( nSize == 120 )
	{
		bool bTest = true;
		bTest = false;
	}
	else if( nSize == 108 )
	{
		bool bTest = true;
		bTest = false;
	}
	else if( nSize == 720 )
	{
		bool bTest = true;
		bTest = false;
	}
	return( TRUE );         // Allow the memory operation to proceed
}

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
//	int* piaLeak = ET_NEW int[10];
	// break on a specific memory allocation
//	_CrtSetBreakAlloc(48);
	_CrtSetAllocHook( g_DebugAllocHook );

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
	if( EXIT_SUCCESS == EduNet::Options::accessOptions().parseCommandLine( argc, argv ) )
	{
		if( true == EduNet::Options::accessOptions().continueProcess() )
		{
			EduNet::Application::Run( argc, argv );
		}
		else
		{
			iExitCode = EXIT_SUCCESS;
		}
	}
	EduNet::Application::_SDMShutdown();
	g_MemoryDebug.exitMain();
	return iExitCode;
}


