#pragma once
#ifndef MasterZonePlugin_h__
#define MasterZonePlugin_h__

//-----------------------------------------------------------------------------
// Copyright (c) 2011, Jan Fietz, Cyrus Preuss
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

#include "OpenSteerUT/ZonePlugin.h"

namespace OpenSteer	{
	class SimpleNetworkVehicle;
}

namespace EduNet	{
	
	class MasterZonePlugin : public OpenSteer::ZonePlugin
	{
		ET_DECLARE_BASE( OpenSteer::ZonePlugin )
	public:
		MasterZonePlugin ( bool bAddToRegistry = false, size_t zoneId = 4 );

		const char* name() const;

		void zoneCheck( const ZonePlugin* zone, OpenSteer::SimpleNetworkVehicle* vehicle );

		//---------------------------------------------------------------------
		virtual void open( void );
		virtual void update( const float currentTime, const float elapsedTime );

		virtual void onSubZoneAdded( ZonePlugin* pkSubZone );

		bool createsContentZone( size_t uiZoneId ) const
		{
			if( uiZoneId < 4 )
			{
				return this->m_bCreateContentZone[uiZoneId];
			}
			return false;
		}

		virtual void setEntityFactory( AbstractEntityFactory* );

		virtual AbstractEntityFactory* getEntityFactory( void ) const;

		virtual void addVehicle (OpenSteer::AbstractVehicle* pkVehicle);
		virtual void removeVehicle (OpenSteer::AbstractVehicle* pkVehicle);

		bool m_bCreateContentZone[4];
		size_t m_uiZoneId;

		size_t getZoneId( void ) const{ return this->m_uiZoneId; };
		void setZoneId( size_t zoneId );
		
		OpenSteer::ZonePlugin* accessOwnSubZone() const;
		AbstractEntityFactory* m_pEntityFactory;				
	};

}
#endif // MasterZonePlugin_h__
