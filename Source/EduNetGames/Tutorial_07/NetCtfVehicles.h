#ifndef __NETCTFVEHICLES_H__
#define __NETCTFVEHICLES_H__
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

#include "OpenSteerUT/SimpleNetworkVehicle.h"
#include "OpenSteerUT/AbstractVehicleUpdate.h"
#include "OpenSteerUT/AbstractVehicleUtilities.h"
#include "OpenSteerUT/VehicleClassIds.h"

//-----------------------------------------------------------------------------
// short names for STL vectors (iterators) of SphereObstacle pointers
// (obsolete? replace with ObstacleGroup/ObstacleIterator ?)

namespace OpenSteer {

typedef std::vector<OpenSteer::SphereObstacle*> SOG;  // SphereObstacle group
typedef SOG::const_iterator SOI;           // SphereObstacle iterator

}

//-----------------------------------------------------------------------------
class NetCtfBaseVehicle : public OpenSteer::SimpleNetworkVehicle
{
	ET_DECLARE_BASE( OpenSteer::SimpleNetworkVehicle )
public:

	NetCtfBaseVehicle();
	NetCtfBaseVehicle( OpenSteer::ProximityDatabase& pd );
	virtual ~NetCtfBaseVehicle();

	OS_IMPLEMENT_CLASSNAME( NetCtfBaseVehicle )

	// create a clone
	virtual AbstractVehicle* cloneVehicle( OpenSteer::ProximityDatabase* ) const;

	// reset state
	virtual void reset( void );

	virtual void update( const float currentTime, const float elapsedTime );
	// draw this character/vehicle into the scene
	virtual void draw( const float currentTime, const float elapsedTime );

	// annotate when actively avoiding obstacles
	virtual void annotateAvoidObstacle( const float minDistanceToCollision );

	void drawHomeBase( void );

	void randomizeStartingPositionAndHeading ( void );
	enum seekerState {running, tagged, atGoal};

	// for draw method
	osColor bodyColor;

	// xxx store steer sub-state for anotation
	bool avoiding;

	// dynamic obstacle registry
	static void initializeObstacles( void );
	static void addOneObstacle( void );
	static void removeOneObstacle( void );
	float minDistanceToObstacle( const osVector3& point );
	static int obstacleCount;
	static const int maxObstacleCount;
	static OpenSteer::SOG allObstacles;
private:
	OpenSteer::SteeringForceVehicleUpdate m_kSteeringForceUpdate;
	OpenSteer::EulerVehicleUpdate m_kEulerUpdate;

	ET_IMPLEMENT_CLASS_NO_COPY( NetCtfBaseVehicle )

};

//-----------------------------------------------------------------------------
class NetCtfSeekerVehicle : public NetCtfBaseVehicle
{
	ET_DECLARE_BASE( NetCtfBaseVehicle )
public:

	// constructor
	NetCtfSeekerVehicle () {reset();}
	NetCtfSeekerVehicle( OpenSteer::ProximityDatabase& pd ):BaseClass( pd ){reset();};

	OS_IMPLEMENT_CLASSNAME( NetCtfSeekerVehicle )

	// create a clone
	virtual AbstractVehicle* cloneVehicle( OpenSteer::ProximityDatabase* ) const;

	// reset state
	virtual void reset( void );

	// per frame simulation update
	virtual void update( const float currentTime, const float elapsedTime );
	virtual osVector3 determineCombinedSteering( const float elapsedTime );

	// is there a clear path to the goal?
	bool clearPathToGoal( void );

	osVector3 steeringForSeeker( void );
	void updateState( const float currentTime );
	void draw( const float currentTime, const float elapsedTime );
	osVector3 steerToEvadeAllDefenders( void );
	osVector3 XXXsteerToEvadeAllDefenders( void );
	void adjustObstacleAvoidanceLookAhead( const bool clearPath );
	void clearPathAnnotation( const float threshold,
		const float behindcThreshold,
		const osVector3& goalDirection );

	const char* getSeekerStateString( void ) const;

	osAVGroup& accessEnemies( void )
	{
		return m_kEnemies;
	}

	seekerState state;
	bool evading; // xxx store steer sub-state for anotation
	float lastRunningTime; // for auto-reset
private:
	osAVGroup m_kEnemies;

	void drawHomeBase( void ) const;

	ET_IMPLEMENT_CLASS_NO_COPY( NetCtfSeekerVehicle )
};

//-----------------------------------------------------------------------------
class NetCtfEnemyVehicle : public NetCtfBaseVehicle
{
	ET_DECLARE_BASE( NetCtfBaseVehicle )
public:

	// constructor
	NetCtfEnemyVehicle ():m_pkSeeker( NULL ) {reset ();}
	NetCtfEnemyVehicle( OpenSteer::ProximityDatabase& pd ):BaseClass( pd ){reset();};

	OS_IMPLEMENT_CLASSNAME( NetCtfEnemyVehicle )

	// create a clone
	virtual AbstractVehicle* cloneVehicle( OpenSteer::ProximityDatabase* ) const;
	
	// reset state
	virtual void reset( void );

	// per frame simulation update
	virtual void update( const float currentTime, const float elapsedTime );
	virtual osVector3 determineCombinedSteering( const float elapsedTime );

	void setSeeker( NetCtfSeekerVehicle* pkSeeker )
	{
		this->m_pkSeeker = pkSeeker;
	}
private:
	NetCtfSeekerVehicle* m_pkSeeker;
	ET_IMPLEMENT_CLASS_NO_COPY( NetCtfEnemyVehicle )
};


typedef OpenSteer::VehicleClassIdMixin<NetCtfBaseVehicle, ET_CID_CTF_BASE_VEHICLE> TNetCtfBaseVehicle;
typedef OpenSteer::VehicleClassIdMixin<NetCtfSeekerVehicle, ET_CID_CTF_SEEKER_VEHICLE> TNetCtfSeekerVehicle;
typedef OpenSteer::VehicleClassIdMixin<NetCtfEnemyVehicle, ET_CID_CTF_ENEMY_VEHICLE> TNetCtfEnemyVehicle;


#endif // __NETCTFVEHICLES
