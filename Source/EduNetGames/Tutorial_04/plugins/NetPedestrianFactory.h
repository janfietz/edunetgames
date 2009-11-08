#ifndef __NETPEDESTRIANFACTORY_H__
#define __NETPEDESTRIANFACTORY_H__

#include "EduNet/common/EduNetCommon.h"


//-----------------------------------------------------------------------------
class NetPedestrianFactory
{
public:

	virtual class NetPedestrian* CreateNetPedestrian( osProximityDatabase& pd );

	virtual void DestroyNetPedestrian( const class NetPedestrian* boid );
};


#endif // __NETPEDESTRIANFACTORY_H__