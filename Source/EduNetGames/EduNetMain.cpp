#define EDUNET_SHOW_CONFIG 1
#include "EduNetGames.h"

// To include EXIT_SUCCESS
#include <cstdlib>

//-----------------------------------------------------------------------------
int EduNetMain (int argc, char **argv) 
{
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
		return EXIT_SUCCESS;
	}
	return EXIT_FAILURE;
}


