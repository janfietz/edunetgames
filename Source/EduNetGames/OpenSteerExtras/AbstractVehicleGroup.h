#ifndef __ABSTRACTVEHICLEGROUP_H__
#define __ABSTRACTVEHICLEGROUP_H__

#include "EduNet/common/EduNetCommon.h"

namespace OpenSteer {

//-----------------------------------------------------------------------------
// a utility class to handle common vehicle related topics
class AbstractVehicleGroup
{
public:
	AbstractVehicleGroup( AVGroup& kAVGroup );
	AbstractVehicleGroup( const AVGroup& kAVGroup );
	virtual ~AbstractVehicleGroup( void ){};

	virtual void update (const float currentTime, const float elapsedTime);

	virtual void redraw (const float currentTime, const float elapsedTime);
	virtual void reset( void );

	virtual void addVehicle( AbstractVehicle* pkVehicle );
 	virtual void removeVehicle( const AbstractVehicle* pkVehicle );
 	virtual AVGroup::iterator findVehicle( const AbstractVehicle* pkVehicle ) const;

	size_t population() const { return m_kVehicles.size(); }
private:
	AbstractVehicleGroup( void );
	AVGroup& m_kVehicles;

};

} // namespace OpenSteer

#endif //  __ABSTRACTVEHICLEGROUP_H__
