//-----------------------------------------------------------------------------
// Copyright (c) 2009, Jan Fietz, Cyrus Preuss
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without modification, 
// are permitted provided that the following conditions are met:
// 
// * Redistributions of source code must retain the above copyright notice, 
//   this list of conditions and the following disclaimer.
// * Redistributions in binary form must reproduce the above copyright notice, 
//   this list of conditions and the following disclaimer in the documentation 
//   and/or other materials provided with the distribution.
// * Neither the name of EduNetGames nor the names of its contributors
//   may be used to endorse or promote products derived from this software
//   without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
// IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES 
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON 
// ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, 
// EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//-----------------------------------------------------------------------------

#include "AbstractVehicleGroup.h"
#include "AbstractVehicleUpdate.h"

#include "OpenSteer/SimpleVehicle.h"

#include <algorithm>

namespace OpenSteer {

using namespace OpenSteer;

//-----------------------------------------------------------------------------
AbstractVehicleGroup::AbstractVehicleGroup( AVGroup& kAVGroup ):
m_kVehicles(kAVGroup),
m_pkCustomUpdated( NULL )
{
	
}

//-----------------------------------------------------------------------------
AbstractVehicleGroup::AbstractVehicleGroup( const AVGroup& kAVGroup ):
m_kVehicles((AVGroup&)kAVGroup),
m_pkCustomUpdated( NULL )
{

}

//-----------------------------------------------------------------------------
void AbstractVehicleGroup::allocateProximityToken( ProximityDatabase* pd )
{
	AVIterator iter = m_kVehicles.begin();
	AVIterator last = m_kVehicles.end();
	while( iter != last )
	{
		(*iter)->allocateProximityToken( pd );
		++iter;
	}
}

//-----------------------------------------------------------------------------
void AbstractVehicleGroup::updateCustom( 
	AbstractUpdated* pkParent, const osScalar currentTime, const osScalar elapsedTime )
{
	assert( true == false );
}

//-----------------------------------------------------------------------------
void AbstractVehicleGroup::update( const float currentTime, const float elapsedTime )
{
	AVIterator iter = m_kVehicles.begin();
	AVIterator last = m_kVehicles.end();
	while( iter != last )
	{
		AbstractVehicle* pkVehicle = (*iter);
		AbstractUpdated* pkVehicleCustomUpdate = pkVehicle->getCustomUpdated();
		pkVehicle->setCustomUpdated( this->getCustomUpdated() );
		AbstractUpdated* pkCustomUpdate = pkVehicle->getCustomUpdated();
		if( NULL != pkCustomUpdate )
		{
			AbstractVehicleUpdate* pkVehicleUpdate = dynamic_cast<AbstractVehicleUpdate*>( pkCustomUpdate );
			if( NULL != pkVehicleUpdate )
			{
				pkVehicleUpdate->setVehicle( pkVehicle );
				pkVehicleUpdate->update( currentTime, elapsedTime );
				pkVehicleUpdate->setVehicle( NULL );
			}
			else
			{
				pkVehicle->update( currentTime, elapsedTime );
			}
		}
		else
		{
			pkVehicle->setCustomUpdated( pkVehicleCustomUpdate );
			pkVehicle->update( currentTime, elapsedTime );
		}
		pkVehicle->setCustomUpdated( pkVehicleCustomUpdate );
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
void AbstractVehicleGroup::reset( void )
{
	AVIterator iter = m_kVehicles.begin();
	AVIterator last = m_kVehicles.end();
	while( iter != last )
	{
		(*iter)->reset( );
		++iter;
	}
}

//-----------------------------------------------------------------------------
void AbstractVehicleGroup::addVehicle( AbstractVehicle* pkVehicle, ProximityDatabase* pkProximityDatabase )
{
	// do not add NULL vehicles
	if( NULL == pkVehicle )
	{
		return;
	}
	// allocate a proximity token in case there is a proximity database
	pkVehicle->allocateProximityToken( pkProximityDatabase );
	m_kVehicles.push_back( pkVehicle );
 	if( m_kVehicles.size() == 1 )
	{
 		SimpleVehicle::selectedVehicle = pkVehicle;
	}
	else
	{
		if( NULL == SimpleVehicle::selectedVehicle )
		{
			SimpleVehicle::selectedVehicle = pkVehicle;
		}
	}
}

//-----------------------------------------------------------------------------
void AbstractVehicleGroup::removeVehicle( const AbstractVehicle* pkVehicle )
{
	AVGroup::iterator kIter = this->findVehicle( pkVehicle );
	if(kIter != m_kVehicles.end())
	{
		// release the proximity token in case allocated
		(*kIter)->allocateProximityToken( NULL );
		m_kVehicles.erase( kIter );
 		// if it is SimpleVehicle's selected vehicle, unselect it
 		if( pkVehicle == SimpleVehicle::selectedVehicle )
		{
 			SimpleVehicle::selectedVehicle = NULL;
		}
	}       
}

//-----------------------------------------------------------------------------
AVGroup::iterator AbstractVehicleGroup::findVehicle( const AbstractVehicle* pkVehicle ) const
{
	EntityPointerQuery kEntityPointerQuery(pkVehicle);
	AVQuery kQuery( this->m_kVehicles, &kEntityPointerQuery );
	return kQuery.find( );
//	return std::find( m_kVehicles.begin(), m_kVehicles.end(), pkVehicle );
}

//-----------------------------------------------------------------------------
AVGroup::iterator AbstractVehicleGroup::findNetworkVehicle( NetworkId networkId ) const
{
	NetworkEntityQuery kNetworkEntityQuery(networkId);
	AVQuery kQuery( this->m_kVehicles, &kNetworkEntityQuery );
	return kQuery.find( );
}

} // namespace OpenSteer
