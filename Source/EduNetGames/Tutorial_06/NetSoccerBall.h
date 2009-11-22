#ifndef __NETSOCCERBALL_H__
#define __NETSOCCERBALL_H__
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

//-----------------------------------------------------------------------------
#include "OpenSteerUT/AbstractVehicleUpdate.h"
#include "OpenSteerUT/AbstractVehicleUtilities.h"
#include "OpenSteerUT/VehicleClassIds.h"
#include "EduNetConnect/SimpleNetworkVehicle.h"

//-----------------------------------------------------------------------------
namespace NetSoccerGame {

using namespace OpenSteer;
;


//-----------------------------------------------------------------------------

// a box object for the field and the goals.
class AABBox {
public:
    AABBox(Vec3 &min, Vec3& max): m_min(min), m_max(max) {}
    AABBox(Vec3 min, Vec3 max): m_min(min), m_max(max) {}
    bool    InsideX(const Vec3 p) {
        if (p.x < m_min.x || p.x > m_max.x)        return false;
        return true;
    }
    bool    InsideZ(const Vec3 p) {
        if (p.z < m_min.z || p.z > m_max.z)        return false;
        return true;
    }
    void    draw() {
        Vec3 b,c;
        b = Vec3(m_min.x, 0, m_max.z);
        c = Vec3(m_max.x, 0, m_min.z);
        Color color(1.0f,1.0f,0.0f);
        drawLineAlpha(m_min, b, color, 1.0f);
        drawLineAlpha(b, m_max, color, 1.0f);
        drawLineAlpha(m_max, c, color, 1.0f);
        drawLineAlpha(c,m_min, color, 1.0f);
    }
private:
    Vec3 m_min;
    Vec3 m_max;
};
};

//-----------------------------------------------------------------------------
class NetSoccerBall : public OpenSteer::SimpleNetworkVehicle
{
    ET_DECLARE_BASE ( OpenSteer::SimpleNetworkVehicle )
public:

    NetSoccerBall();
    NetSoccerBall ( NetSoccerGame::AABBox *bbox );
    virtual ~NetSoccerBall();

    // create a clone
    virtual AbstractVehicle* cloneVehicle ( void ) const;
    // reset state
    virtual void reset ( void );

    virtual void update ( const float currentTime, const float elapsedTime );
    // draw this character/vehicle into the scene
    virtual void draw ( const float currentTime, const float elapsedTime );

    void kick ( OpenSteer::Vec3 dir, const float elapsedTime );

    NetSoccerGame::AABBox *m_bbox;
private:
    ET_IMPLEMENT_CLASS_NO_COPY( NetSoccerBall )
};

#endif // __NETSOCCERBALL_H__
