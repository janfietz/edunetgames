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
#include "NetSoccerBall.h"
using namespace OpenSteer;




//-----------------------------------------------------------------------------
NetSoccerBall::NetSoccerBall(): m_bbox ( NULL )
{
	reset();
}

//-----------------------------------------------------------------------------
NetSoccerBall::~NetSoccerBall()
{
}

//-----------------------------------------------------------------------------
// reset state
void NetSoccerBall::reset ( void )
{
    BaseClass::reset (); // reset the vehicle
    setSpeed ( 0.0f );       // speed along Forward direction.
    setMaxForce ( 9.0f );    // steering force is clipped to this magnitude
    setMaxSpeed ( 9.0f );       // velocity is clipped to this magnitude

    setPosition ( 0,0,0 );
    setTrailParameters ( 100, 6000 );
}
//-----------------------------------------------------------------------------
// per frame simulation update
osVector3 NetSoccerBall::determineCombinedSteering (const float elapsedTime)
{
    applyBrakingForce ( 1.5f, elapsedTime );
    // are we now outside the field?
    if ( !m_bbox->insideX ( position() ) )
    {
        Vec3 d = velocity();
        regenerateOrthonormalBasis ( Vec3 ( -d.x, d.y, d.z ) );
		this->setLinearVelocity( this->forward() * d.length() );
    }
    if ( !m_bbox->insideZ ( position() ) )
    {
        Vec3 d = velocity();
        regenerateOrthonormalBasis ( Vec3 ( d.x, d.y, -d.z ) );
		this->setLinearVelocity( this->forward() * d.length() );
   }
//    recordTrailVertex ( currentTime, position() );
	// return steering constrained to global XZ "ground" plane
	Vec3 kSteeringForce = velocity();
	return kSteeringForce.setYtoZero();
}
//-----------------------------------------------------------------------------
void NetSoccerBall::kick ( Vec3 dir, const float elapsedTime )
{
    setSpeed ( dir.length() );
    regenerateOrthonormalBasis ( dir );
}
//-----------------------------------------------------------------------------
// draw this character/vehicle into the scene
void NetSoccerBall::draw ( const float currentTime,
                           const float elapsedTime )
{
	BaseClass::draw( currentTime, elapsedTime );
    drawBasic2dCircularVehicle ( *this, Color ( 0.0f,1.0f,0.0f ) );
    drawTrail ();
}
//-----------------------------------------------------------------------------
AbstractVehicle* NetSoccerBall::cloneVehicle ( void ) const
{
    return ET_NEW NetSoccerBall();
}
