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

#include "NetCtfEntityFactory.h"
#include "NetCtfVehicles.h"

//-----------------------------------------------------------------------------
class NetCtfBaseEntityFactory : public OpenSteer::TEntityFactory<TNetCtfBaseVehicle>
{
	ET_DECLARE_BASE( OpenSteer::TEntityFactory<TNetCtfBaseVehicle> );
public:
	NetCtfBaseEntityFactory(){};
	virtual ~NetCtfBaseEntityFactory(){};
};

//-----------------------------------------------------------------------------
class NetCtfSeekerEntityFactory : public OpenSteer::TEntityFactory<TNetCtfSeekerVehicle>
{
	ET_DECLARE_BASE( OpenSteer::TEntityFactory<TNetCtfSeekerVehicle> );
public:
	NetCtfSeekerEntityFactory(){};
	virtual ~NetCtfSeekerEntityFactory(){};
};

//-----------------------------------------------------------------------------
class NetCtfEnemyEntityFactory : public OpenSteer::TEntityFactory<TNetCtfEnemyVehicle>
{
	ET_DECLARE_BASE( OpenSteer::TEntityFactory<TNetCtfSeekerVehicle> );
public:
	NetCtfEnemyEntityFactory(){};
	virtual ~NetCtfEnemyEntityFactory(){};
};


//-----------------------------------------------------------------------------
NetCtfEntityFactory::NetCtfEntityFactory()
{
	//-------------------------------------------------------------------------
	// now 3 global vehicle factories
	static NetCtfBaseEntityFactory gNetCtfBaseEntityFactory;
	static NetCtfSeekerEntityFactory gNetCtfSeekerEntityFactory;
	static NetCtfEnemyEntityFactory gNetCtfEnemyEntityFactory;
	this->addEntityFactory( &gNetCtfBaseEntityFactory );
	this->addEntityFactory( &gNetCtfSeekerEntityFactory );
	this->addEntityFactory( &gNetCtfEnemyEntityFactory );
#ifdef ET_DEBUG
	// test the 3 vehicle classes
	OpenSteer::AbstractVehicle* pkVehicle = NULL;
	pkVehicle = this->createVehicle( ET_CID_CTF_BASE_VEHICLE );
	this->destroyVehicle( pkVehicle );
	pkVehicle = this->createVehicle( ET_CID_CTF_ENEMY_VEHICLE );
	this->destroyVehicle( pkVehicle );
	pkVehicle = this->createVehicle( ET_CID_CTF_SEEKER_VEHICLE );
	this->destroyVehicle( pkVehicle );
#endif
}

//-----------------------------------------------------------------------------
NetCtfEntityFactory::~NetCtfEntityFactory()
{

}

