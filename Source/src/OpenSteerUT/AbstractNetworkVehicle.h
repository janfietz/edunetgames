#ifndef __ABSTRACTNETWORKVEHICLE_H__
#define __ABSTRACTNETWORKVEHICLE_H__

//-----------------------------------------------------------------------------
#include "EduNet/common/EduNetCommon.h"
#include "OpenSteerExtrasTypes.h"

//-----------------------------------------------------------------------------
namespace OpenSteer {

	class AbstractNetworkVehicle
	{
	public:

//		virtual ~AbstractNetworkVehicle();

		virtual void testFunction() = 0;
		virtual size_t getVehicleId( void ) const = 0;
		virtual void setIsRemoteObject( bool bIsRemote ) = 0;
		virtual bool isRemoteObject( void ) const = 0;

	};
} // namespace OpenSteer




#endif // 