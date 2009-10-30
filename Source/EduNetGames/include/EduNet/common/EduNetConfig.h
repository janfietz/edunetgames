#ifndef __EDUNETCONFIG_H__
#define __EDUNETCONFIG_H__

#include "EduNetTutorialConfig.h"

// raknet
#ifndef EDUNET_HAVE_RAKNET
#define EDUNET_HAVE_RAKNET 1
#endif


// opensteer
#ifndef EDUNET_HAVE_OPENSTEER
#define EDUNET_HAVE_OPENSTEER 1
#endif

#if EDUNET_SHOW_CONFIG
#pragma message( "-------------------------------------------------------------------------------" )
#pragma message( " Build Configuration" )
#pragma message( "-------------------------------------------------------------------------------" )

#if EDUNET_HAVE_RAKNET
#pragma message( " - EDUNET_HAVE_RAKNET" )
#endif

#if EDUNET_HAVE_OPENSTEER
#pragma message( " - EDUNET_HAVE_OPENSTEER" )
#endif

#pragma message( "-------------------------------------------------------------------------------" )
#endif

#endif // __EDUNETCONFIG_H__
