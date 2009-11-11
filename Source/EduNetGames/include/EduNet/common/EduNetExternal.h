#ifndef __EDUNETEXTERNAL_H__
#define __EDUNETEXTERNAL_H__

// include configuration
#include "EduNetConfig.h"


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
#include "SocketLayer.h"
#include "Rpc3/RPC3.h"

#endif // EDUNET_HAVE_RAKNET

//-----------------------------------------------------------------------------
// opensteer
#if EDUNET_HAVE_OPENSTEER

#include "OpenSteer/Vec3.h"
#include "OpenSteer/Clock.h"
#include "OpenSteer/Plugin.h"
#include "OpenSteer/Camera.h"
//#include "OpenSteer/Utilities.h"
#include "OpenSteer/AbstractVehicle.h"
#include "OpenSteer/SimpleVehicle.h"
#include "OpenSteer/Annotation.h"
#include "OpenSteer/Color.h"

#include "OpenSteer/Obstacle.h"
#include "OpenSteer/Proximity.h"
#include "OpenSteer/PolylineSegmentedPathwaySingleRadius.h"

#include "OpenSteerExtras/OpenSteerExtrasTypes.h"

#endif // EDUNET_HAVE_OPENSTEER


#endif // __EDUNETEXTERNAL_H__