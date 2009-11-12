#ifndef __NETPEDESTRIANFACTORY_H__
#define __NETPEDESTRIANFACTORY_H__

#include "EduNetCommon/EduNetCommon.h"


//-----------------------------------------------------------------------------
class NetPedestrianFactory
{
public:

	virtual class NetPedestrian* CreateNetPedestrian( osProximityDatabase& pd );

	virtual void DestroyNetPedestrian( const class OpenSteer::AbstractVehicle* boid );
};


#endif // __NETPEDESTRIANFACTORY_H__