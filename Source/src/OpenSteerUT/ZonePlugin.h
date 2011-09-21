#ifndef __ZONEPLUGIN_H__
#define __ZONEPLUGIN_H__

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
#include "EduNetCommon/EduNetCommon.h"
#include "OpenSteer/AABBox.h"

namespace OpenSteer
{
	//-------------------------------------------------------------------------
	class ZonePlugin : public OpenSteer::PluginArray
	{
		ET_DECLARE_BASE( OpenSteer::PluginArray )
	public:
		ZonePlugin ( bool bAddToRegistry = false );

		OS_IMPLEMENT_CLASSNAME( ZonePlugin )
		//----------------------------------------------------------------------------
		// OpenSteer::Plugin interface
		virtual void initGui( void* pkUserdata );
		// required methods:
		const char* name( void ) const { return this->getClassName(); }
		virtual void prepareOpen (void);
		virtual void open( void );
		virtual void update( const float currentTime, const float elapsedTime );
		virtual void redraw( const float currentTime, const float elapsedTime );
		virtual void close( void );
		virtual const AVGroup& allVehicles( void ) const { return m_kVehicles; }
		virtual AVGroup& allVehicles( void ) { return m_kVehicles; }
		virtual bool queryVehicleColor( const OpenSteer::AbstractVehicle& kVehicle, osColor& kColor ) const;


		// optional methods (see comments in AbstractPlugin for explanation):
		virtual void reset (void); // default is to reset by doing close-then-open
		virtual float selectionOrderSortKey( void ) const {return 1000000;}
		virtual bool requestInitialSelection( void ) const {return false;}
		virtual void handleFunctionKeys( int keyNumber ) { } // fkeys reserved for Plugins
		virtual void printMiniHelpForFunctionKeys( void ) const { } // if fkeys are used

		virtual void addVehicle (OpenSteer::AbstractVehicle* pkVehicle);
		virtual void removeVehicle (OpenSteer::AbstractVehicle* pkVehicle);

		// extende interface
		virtual void onSubZoneAdded( ZonePlugin* pkSubZone ){};

		void setBorderWidth( osScalar value )
		{
			this->m_fBorderWidth = value;
			this->updateZoneAABBox();
		}

		void setZoneCenter( const osVector3& kGridCenter )
		{
			this->setPosition( kGridCenter );
			this->updateZoneAABBox();
		}

		void setZoneExtent( const osVector3& kExtent )
		{
			this->m_kZoneExtent = kExtent;
			this->updateZoneAABBox();
		}

		void setZoneColor( const osColor& kColor )
		{
			this->m_kZoneColor = kColor;
			this->m_kBorderColor = kColor;
			this->m_kBorderColor *= 0.5f;
		}

		osScalar getBorderWidth( void ) const
		{
			return this->m_fBorderWidth;
		}

		const osVector3& getZoneExtent( void ) const { return this->m_kZoneExtent; }
		const osVector3& getZoneCenter( void ) const { return this->position(); }
		const osColor& getZoneColor( void ) const { return this->m_kZoneColor; }
		const osColor& getBorderColor( void ) const { return this->m_kBorderColor; }

		size_t getZoneId( void ) const { return this->m_zoneId; }

		bool isVehicleInside( const OpenSteer::AbstractVehicle& kVehicle ) const;

		int m_iSolid;
	private:

		void updateZoneAABBox( void )
		{
			osVector3 kCheckZoneExtent = this->m_kZoneExtent;
			kCheckZoneExtent.x += this->m_fBorderWidth;
//			kCheckZoneExtent.y += this->m_fBorderWidth;
			kCheckZoneExtent.z += this->m_fBorderWidth;
			this->m_kZoneAABBox.initializeWithCenterAndExtent( this->position(), kCheckZoneExtent );
		}
		void zoneUtility( void );
		void addSubZones( void );

		osColor m_kBorderColor;
		osColor m_kZoneColor;
		osScalar (m_fBorderWidth);
		size_t m_zoneId;

		AVGroup m_kVehicles;
		osVector3 m_kZoneExtent;

		OpenSteer::AABBox m_kZoneAABBox;

		ET_IMPLEMENT_CLASS_NO_COPY(ZonePlugin)

	};

}


#endif  // __ZONEPLUGIN_H__
