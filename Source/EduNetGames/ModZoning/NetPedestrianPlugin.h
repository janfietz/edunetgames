#ifndef __NETPEDESTRIANPLUGIN_H__
#define __NETPEDESTRIANPLUGIN_H__

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

//-----------------------------------------------------------------------------
#include "EduNetCommon/EduNetCommon.h"
#include "NetPedestrianFactory.h"

namespace OpenSteer {
	class PolylineSegmentedPathwaySingleRadius;
}


//-----------------------------------------------------------------------------
class NetPedestrianPlugin : public OpenSteer::Plugin
{
	ET_DECLARE_BASE(OpenSteer::Plugin);
public:
	NetPedestrianPlugin( bool bAddToRegistry = true, float pathScale = 1.0 );

	virtual ~NetPedestrianPlugin();

	OS_IMPLEMENT_CLASSNAME( NetPedestrianPlugin )
	//-------------------------------------------------------------------------
	// OpenSteer::Plugin interface
	virtual const char* name() const { return this->getClassName(); };

	virtual float selectionOrderSortKey (void) const;

	virtual void open (void);
	virtual void update (const float currentTime, const float elapsedTime);
	virtual void redraw (const float currentTime, const float elapsedTime);

	virtual void close (void);

	virtual void reset (void);

	virtual void handleFunctionKeys (int keyNumber);

	virtual void printMiniHelpForFunctionKeys (void) const;

	virtual osProximityDatabase* accessProximityDataBase( void ) const
	{
		return this->pd;
	}

	virtual const osAVGroup& allVehicles (void) const;
	virtual osAVGroup& allVehicles (void);

	// implement to initialize additional gui functionality
	virtual void initGui( void* pkUserdata );

	void drawPathAndObstacles (void);

	void addPedestrianToCrowd(void);
	void removePedestrianFromCrowd (void);

	// for purposes of demonstration, allow cycling through various
	// types of proximity databases.  this routine is called when the
	// OpenSteerDemo user pushes a function key.
	void nextPD (void);

	// implement to create a vehicle of the specified class
	virtual osAbstractVehicle* createVehicle( osEntityClassId ) const;

	void setPath( OpenSteer::PolylineSegmentedPathwaySingleRadius* path );
	OpenSteer::PolylineSegmentedPathwaySingleRadius* getPath( void ) const
	{
		return this->m_pkPath;
	}

	void setPathColor( const osColor& color )
	{
		this->pathColor = color;
	}
	void setRenderPath( bool bValue )
	{
		this->m_bRenderPath = bValue;
	}

	int m_bWanderSwitch;
	int m_bUseDirectedPathFollowing;
	static OpenSteer::PolylineSegmentedPathwaySingleRadius* createTestPath( float scale );

private:

	NetPedestrianFactory m_kOfflinePedestrianFactory;
	OpenSteer::PolylineSegmentedPathwaySingleRadius* m_pkPath;


	osColor pathColor;

	osAVGroup crowd;

	osVector3 gridCenter;

	// pointer to database used to accelerate proximity queries
	osProximityDatabase* pd;

	// which of the various proximity databases is currently in use
	int cyclePD;

	float m_fLastRenderTime;
	float m_fPathScale;
	bool m_bRenderPath;
};



#endif //  __NETPEDESTRIANPLUGIN_H__