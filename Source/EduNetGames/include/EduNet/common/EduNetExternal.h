#ifndef __EDUNETEXTERNAL_H__
#define __EDUNETEXTERNAL_H__

// include configuration
#include "EduNetConfig.h"

// windows

//-----------------------------------------------------------------------------
#ifdef WIN32
#pragma message( " - windows build" )

#define __ET_WINDOWS_SPEEDUPS__ 1
// some windows speedups
#	if __ET_WINDOWS_SPEEDUPS__

#		define WIN32_LEAN_AND_MEAN
#		define WIN32_EXTRA_LEAN

#		define NOGDICAPMASKS
#		define OEMRESOURCE
#		define NOATOM
#ifndef __ET_WINDOWS_USECLIPBOARD__
#		define NOCLIPBOARD
#endif
//#		define NOCTLMGR // windows utility functions like DialogBox, 
#		define NOMEMMGR
#		define NOMETAFILE
#		define NOOPENFILE
#		define NOSERVICE
#		define NOSOUND
#		define NOCOMM
#		define NOKANJI
#		define NOHELP
#		define NOPROFILER
#		define NODEFERWINDOWPOS
#		define NOMCX
// #define NONLS
// #define NOMB
# define WIN32_LEAN_AND_MEAN
# define WIN32_EXTRA_LEAN

#	endif // __ET_WINDOWS_SPEEDUPS__


# ifndef _WIN32_WINNT
#  define _WIN32_WINNT 0x0502 // Windows XP SP1
# endif
//# endif


#if _MSC_VER >= 1400

#endif

#endif  // WIN32



//-----------------------------------------------------------------------------
// raknet
#if EDUNET_HAVE_RAKNET

#include "ReplicaManager3.h"
#include "FullyConnectedMesh2.h"
#include "ConnectionGraph2.h"
#include "BitStream.h"

#include "MessageIdentifiers.h"
#include "RakNetworkFactory.h"
#include "RakPeerInterface.h"
#include "RakPeerInterface.h"
#include "RakNetTypes.h"
#include "GetTime.h"
#include "BitStream.h"
#include "NetworkIDManager.h"
//#include <RakNetTypes.h>

//#define _WINSOCK2API_
//#define _WINSOCKAPI_   /* Prevent inclusion of winsock.h in windows.h */
#include "SocketLayer.h"

#endif // EDUNET_HAVE_RAKNET

//-----------------------------------------------------------------------------
// opensteer
#if EDUNET_HAVE_OPENSTEER

#include "OpenSteer/Vec3.h"
#include "OpenSteer/Clock.h"
#include "OpenSteer/PlugIn.h"
#include "OpenSteer/Camera.h"
//#include "OpenSteer/Utilities.h"
#include "OpenSteer/AbstractVehicle.h"
#include "OpenSteer/SimpleVehicle.h"
#include "OpenSteer/Annotation.h"
#include "OpenSteer/Color.h"

#include "OpenSteer/Obstacle.h"
#include "OpenSteer/Proximity.h"

#endif // EDUNET_HAVE_OPENSTEER


#endif // __EDUNETEXTERNAL_H__