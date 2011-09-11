/*
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
*/

#ifndef NETBOIDGAMELOGIC_H
#define NETBOIDGAMELOGIC_H
#include "EduNetCommon/EduNetCommon.h"
#include "OpenSteerUT/AbstractVehicleUpdate.h"
#include "BoidsPlugin.h"

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
class NetBoidGameLogic : public OpenSteer::AbstractVehicleUpdate
{
    ET_DECLARE_BASE ( OpenSteer::AbstractVehicleUpdate );
public:
    NetBoidGameLogic ( osAbstractVehicle* pkVehicle = NULL );
    virtual ~NetBoidGameLogic();

    OS_IMPLEMENT_CLASSNAME ( NetBoidGameLogic )

    //-------------------------------------------------------------------------
    // interface AbstractUpdated
    //-------------------------------------------------------------------
    // interface AbstractUpdated
    virtual void updateCustom ( AbstractUpdated* /*pkParent*/,
                                const osScalar /*currentTime*/, const osScalar /*elapsedTime*/ )
    {
        // nothing to do here
        return;
    }
    virtual void update ( const float currentTime, const float elapsedTime );

    //-------------------------------------------------------------------------
    void setPlugin ( OpenSteer::NetBoidsPlugin* pkPlugin )
    {
        this->m_pkPlugin = pkPlugin;
    }

protected:
    OpenSteer::NetBoidsPlugin* m_pkPlugin;

};

#endif // NETBOIDGAMELOGIC_H
