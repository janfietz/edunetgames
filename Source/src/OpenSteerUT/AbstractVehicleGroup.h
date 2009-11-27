#ifndef __ABSTRACTVEHICLEGROUP_H__
#define __ABSTRACTVEHICLEGROUP_H__

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

#include "EduNetCommon/EduNetCommon.h"
#include "OpenSteerUT/AbstractEntityQuery.h"

namespace OpenSteer {


//-----------------------------------------------------------------------------
// a utility class to handle common vehicle related topics
class AbstractVehicleGroup : public OpenSteer::AbstractUpdated
{
public:
	AbstractVehicleGroup( AVGroup& kAVGroup );
	AbstractVehicleGroup( const AVGroup& kAVGroup );
	virtual ~AbstractVehicleGroup( void ){};

	//-------------------------------------------------------------------
	// interface AbstractUpdated
	virtual void updateCustom( AbstractUpdated* pkParent, const osScalar currentTime, const osScalar elapsedTime );
	virtual void update (const osScalar currentTime, const osScalar elapsedTime);

	virtual void setCustomUpdated( AbstractUpdated* pkUpdated )
	{
		this->m_pkCustomUpdated = pkUpdated;
	}
	virtual AbstractUpdated* getCustomUpdated( void ) const
	{
		return this->m_pkCustomUpdated;
	}

	virtual bool isEnabled( void ) const { return this->m_bEnabled; }; 
	virtual void setEnabled( bool bEnabled ){ this->m_bEnabled = bEnabled; }; 

	//-------------------------------------------------------------------------
	// vehicle group tools
	void addVehicleToPlugin( AbstractVehicle* pkVehicle, AbstractPlugin* pkPlugin );
	void removeVehicleFromPlugin( const AbstractVehicle* pkVehicle );

	void redraw (const float currentTime, const float elapsedTime);
	void reset( void );
	void allocateProximityToken( ProximityDatabase* pd );

 	AVGroup::iterator findVehicle( const AbstractVehicle* pkVehicle ) const;
	AVGroup::iterator findNetworkVehicle( NetworkId networkId ) const;

	AVGroup::iterator begin() { return m_kVehicles.begin(); }
	AVGroup::iterator end() { return m_kVehicles.end(); }
	size_t population() const { return m_kVehicles.size(); }
private:
#if 0
	void addVehicle( AbstractVehicle* pkVehicle, ProximityDatabase* pkProximityDatabase = NULL );
#endif
	AbstractVehicleGroup( void );
	AVGroup& m_kVehicles;
	AbstractUpdated* m_pkCustomUpdated;
	bool m_bEnabled;

};

} // namespace OpenSteer

#endif //  __ABSTRACTVEHICLEGROUP_H__
