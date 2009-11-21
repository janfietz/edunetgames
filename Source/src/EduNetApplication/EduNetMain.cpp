#define EDUNET_SHOW_CONFIG 1
#include "EduNetGames.h"
#include "EduNetApplication.h"

// To include EXIT_SUCCESS
#include <cstdlib>

//-----------------------------------------------------------------------------
int EduNetMain (int argc, char **argv) 
{
	int iExitCode = EXIT_FAILURE;
	EduNet::Application::_SDMInit();
	if( EXIT_SUCCESS == OpenSteer::OpenSteerDemo::options.parseCommandLine( argc, argv ) )
	{
		if( true ==  OpenSteer::OpenSteerDemo::options.continueProcess() )
		{
			// initialize OpenSteerDemo application
			OpenSteer::OpenSteerDemo::initialize ();

			// initialize graphics
			OpenSteer::initializeGraphics (argc, argv);

			// run the main event processing loop
			OpenSteer::runGraphics ();  
		}
		iExitCode = EXIT_SUCCESS;
	}
	EduNet::Application::_SDMShutdown();
	return iExitCode;
}


