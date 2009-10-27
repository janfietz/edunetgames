#include "AbstractVehicleGroup.h"
#include "OpenSteer/OpenSteerDemo.h"
#include <algorithm>

namespace OpenSteer {

//-----------------------------------------------------------------------------
AbstractVehicleGroup::AbstractVehicleGroup( AVGroup& kAVGroup ):
m_kVehicles(kAVGroup)
{
	
}

//-----------------------------------------------------------------------------
void AbstractVehicleGroup::update (const float currentTime, const float elapsedTime)
{
	AVIterator iter = m_kVehicles.begin();
	AVIterator last = m_kVehicles.end();
	while( iter != last )
	{
		(*iter)->update( currentTime, elapsedTime );
		++iter;
	}
}

//-----------------------------------------------------------------------------
void AbstractVehicleGroup::redraw (const float currentTime, const float elapsedTime)
{
	AVIterator iter = m_kVehicles.begin();
	AVIterator last = m_kVehicles.end();
	while( iter != last )
	{
		(*iter)->draw( currentTime, elapsedTime );
		++iter;
	}
}

//-----------------------------------------------------------------------------
void AbstractVehicleGroup::addVehicle( AbstractVehicle* pkVehicle )
{
	m_kVehicles.push_back( pkVehicle );
 	if( m_kVehicles.size() == 1 )
	{
 		OpenSteerDemo::selectedVehicle = pkVehicle;
	}
}

//-----------------------------------------------------------------------------
void AbstractVehicleGroup::removeVehicle( AbstractVehicle* pkVehicle )
{
	AVGroup::iterator kIter = this->findVehicle( pkVehicle );
	if(kIter != m_kVehicles.end())
	{
		m_kVehicles.erase( kIter );
 		// if it is OpenSteerDemo's selected vehicle, unselect it
 		if( pkVehicle == OpenSteerDemo::selectedVehicle )
		{
 			OpenSteerDemo::selectedVehicle = NULL;
		}
	}       
}

//-----------------------------------------------------------------------------
AVGroup::iterator AbstractVehicleGroup::findVehicle( const AbstractVehicle* pkVehicle ) const
{
	return std::find( m_kVehicles.begin(), m_kVehicles.end(), pkVehicle );
}


} // namespace OpenSteer