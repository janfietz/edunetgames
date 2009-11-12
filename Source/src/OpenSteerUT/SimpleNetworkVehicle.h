#ifndef __SIMPLENETWORKVEHICLE_H__
#define __SIMPLENETWORKVEHICLE_H__

#include "NetworkVehicle.h"
#include "EduNet/common/EduNetCommon.h"
#include "EduNet/common/InstanceCount.h"
#include "EduNet/templates/TUpdatePeriod.h"

namespace OpenSteer {


	// ----------------------------------------------------------------------------
	class AbstractSimpleNetworkVehicle : public SimpleVehicle, public AbstractNetworkVehicle
	{
	public:
		AbstractSimpleNetworkVehicle() { /* Nothing to do. */ }
		virtual ~AbstractSimpleNetworkVehicle() { /* Nothing to do. */ }
	};


	// SimpleVehicle_1 adds concrete LocalSpace methods to AbstractVehicle
	typedef NetworkVehicleMixin<AbstractSimpleNetworkVehicle> SimpleNetworkVehicle_1;

	class SimpleNetworkVehicle : public SimpleNetworkVehicle_1
	{
		ET_DECLARE_BASE( SimpleNetworkVehicle_1 )
	public:
		SimpleNetworkVehicle();
		virtual ~SimpleNetworkVehicle();

		OS_IMPLEMENT_CLASSNAME( SimpleNetworkVehicle )

		virtual size_t getVehicleId( void ) const;

	private:
		static OpenSteer::InstanceTracker ms_kInstanceCount;
		size_t m_uiId;


	};


} // namespace OpenSteer

#endif