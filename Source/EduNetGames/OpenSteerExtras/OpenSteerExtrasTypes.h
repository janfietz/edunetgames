#ifndef __OPENSTEEREXTRASTYPES_H__
#define __OPENSTEEREXTRASTYPES_H__

//-----------------------------------------------------------------------------
namespace OpenSteer{

	// Include names declared in the OpenSteer namespace into the
	// namespace to search to find names.
	using namespace OpenSteer;

	//-------------------------------------------------------------------------
	typedef OpenSteer::AbstractProximityDatabase<AbstractVehicle*> ProximityDatabase;
	typedef OpenSteer::AbstractTokenForProximityDatabase<AbstractVehicle*> ProximityToken;




	float drawGetWindowHeight (void);
	float drawGetWindowWidth (void);

}

typedef OpenSteer::Vec3 osVector3;
typedef OpenSteer::AbstractVehicle osAbstractVehicle;
typedef OpenSteer::AVGroup osAVGroup;
typedef OpenSteer::ProximityDatabase osProximityDatabase;
typedef OpenSteer::ProximityToken osProximityToken;

#endif //  __OPENSTEEREXTRASTYPES_H__