#ifndef __NETPEDESTRIAN_H__
#define __NETPEDESTRIAN_H__

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

//#include "EduNetCommon/EduNetCommon.h"

#include "OpenSteerUT/OpenSteerUTTypes.h"
#include "OpenSteerUT/AbstractVehicleUtilities.h"
#include "OpenSteerUT/VehicleClassIds.h"
#include "EduNetConnect/SimpleNetworkVehicle.h"

//-----------------------------------------------------------------------------
class NetPedestrian : public OpenSteer::SimpleNetworkVehicle
{
	ET_DECLARE_BASE( SimpleNetworkVehicle )
public:

	NetPedestrian();
	virtual ~NetPedestrian();

	OS_IMPLEMENT_CLASSNAME( NetPedestrian )

	virtual void reset( void );
	virtual void update( const float currentTime, const float elapsedTime);
	virtual osVector3 determineCombinedSteering( const float elapsedTime );
	virtual void draw( const float currentTime, const float elapsedTime );

	virtual void annotatePathFollowing( const osVector3& future,
		const osVector3& onPath,
		const osVector3& target,
		const float outside );

	virtual void annotateAvoidCloseNeighbor( const AbstractVehicle& other,
		const float /*additionalDistance*/);

	virtual void annotateAvoidNeighbor (const AbstractVehicle& threat,
		const float /*steer*/,
		const osVector3& ourFuture,
		const osVector3& threatFuture );

	virtual void annotateAvoidObstacle( const float minDistanceToCollision );
	virtual AbstractVehicle* cloneVehicle( void ) const;


	void annotationUtility( void ) const;

	static bool gWanderSwitch;
	static bool gUseDirectedPathFollowing;

private:


	OpenSteer::AVGroup m_kNeighbors;

	// path to be followed by this pedestrian
	// XXX Ideally this should be a generic Pathway, but we use the
	// XXX getTotalPathLength and radius methods (currently defined only
	// XXX on PolylinePathway) to set random initial positions.  Could
	// XXX there be a "random position inside path" method on Pathway?
	OpenSteer::PolylineSegmentedPathwaySingleRadius* path;

	// direction for path following (upstream or downstream)
	int pathDirection;


};

OpenSteer::PolylineSegmentedPathwaySingleRadius* getTestPath (void);

typedef OpenSteer::VehicleClassIdMixin<NetPedestrian, ET_CID_NETPEDESTRIAN> TNetPedestrian;


#endif //  __NETPEDESTRIAN_H__
