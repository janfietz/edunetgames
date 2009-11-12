#include "SimpleNetworkVehicle.h"


//-----------------------------------------------------------------------------
namespace OpenSteer {
	
	OpenSteer::InstanceTracker SimpleNetworkVehicle::ms_kInstanceCount;

	SimpleNetworkVehicle::SimpleNetworkVehicle():
		m_uiId( SimpleNetworkVehicle::ms_kInstanceCount.Constructor() ) 
	{ 
	}

	SimpleNetworkVehicle::~SimpleNetworkVehicle() 
	{ 
		SimpleNetworkVehicle::ms_kInstanceCount.Destructor();
	}

	size_t SimpleNetworkVehicle::getVehicleId( void ) const
	{
		return this->m_uiId;
	}


} // namespace OpenSteer


//-----------------------------------------------------------------------------
class TestNetworkVehicle : public OpenSteer::SimpleNetworkVehicle
{
public:
	virtual void update(const float,const float)
	{

	}
};

//-----------------------------------------------------------------------------
void testSimpleNetWorkVehicle(  )
{
	TestNetworkVehicle kVehicle;
}