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

#include "OpenSteerUT/AbstractVehicleFactory.h"
#include "EduNetCommon/EduNetCommon.h"

using namespace OpenSteer;

//-----------------------------------------------------------------------------
OpenSteer::AbstractVehicle* VehicleFactory::createVehicle( OpenSteer::EntityClassId classId, OpenSteer::ProximityDatabase* pkProximityDatabase ) const
{
	OpenSteer::AbstractVehicle* pkMasterVehicle = this->accessMasterVehicle( classId );
	if( NULL != pkMasterVehicle )
	{
		return pkMasterVehicle->cloneVehicle( pkProximityDatabase );
	}
	return NULL;
}

//-----------------------------------------------------------------------------
OpenSteer::AbstractVehicle* VehicleFactory::createVehicle( OpenSteer::ProximityDatabase* pkProximityDatabase ) const
{
	if( NULL != this->m_pkMasterVehicle )
	{
		// for debugging
		const OpenSteer::EntityClassId classId = this->m_pkMasterVehicle->getClassId();
		return this->m_pkMasterVehicle->cloneVehicle( pkProximityDatabase );
	}
	return NULL;
};

//-----------------------------------------------------------------------------
void VehicleFactory::destroyVehicle( OpenSteer::AbstractVehicle* pkVehicle ) const
{
	ET_SAFE_DELETE( pkVehicle );
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void VehicleFactoryArray::addVehicleFactory( AbstractVehicleFactory* pkFactory )
{
	OpenSteer::EntityClassId classId(0);
	// force initialize
	OpenSteer::AbstractVehicle* pkMasterVehicle = NULL;
	OpenSteer::AbstractVehicle* pkFactoryMasterVehicle = pkFactory->accessMasterVehicle( classId );
	if( NULL == pkFactoryMasterVehicle )
	{
		pkMasterVehicle = pkFactory->getMasterVehicle( );
	}
	else
	{
		pkMasterVehicle = pkFactoryMasterVehicle;
	}
	if( NULL != pkMasterVehicle )
	{
		if( NULL == this->findVehicleFactory( pkMasterVehicle->getClassId() ) )
		{
			this->push_back( pkFactory );
		}
	}
	return;
}

//-----------------------------------------------------------------------------
AbstractVehicleFactory* VehicleFactoryArray::findVehicleFactory( OpenSteer::EntityClassId classId ) const
{
	//			TVehicleFactoryArray::const_iterator kFound = std::find( this->begin(), this->end(), classId );
	TVehicleFactoryArray::const_iterator kIter = this->begin();
	TVehicleFactoryArray::const_iterator kEnd = this->end();
	while( kIter != kEnd )
	{
		OpenSteer::AbstractVehicle* pkMasterVehicle = (*kIter)->accessMasterVehicle( classId );
		if( NULL != pkMasterVehicle )
		{
			return (*kIter);
		}
		++kIter;
	}
	return NULL;
}

//-----------------------------------------------------------------------------
void VehicleFactoryArray::setMasterVehicle( OpenSteer::AbstractVehicle* pkVehicle ) const
{
	AbstractVehicleFactory* pkFactory = this->findVehicleFactory( pkVehicle->getClassId() );
	if( NULL != pkFactory )
	{
		return pkFactory->setMasterVehicle( pkVehicle );
	}
}

//-----------------------------------------------------------------------------
OpenSteer::AbstractVehicle* VehicleFactoryArray::createVehicle( OpenSteer::EntityClassId classId, OpenSteer::ProximityDatabase* pkProximityDatabase ) const
{
	OpenSteer::AbstractVehicle* pkMasterVehicle = this->accessMasterVehicle( classId );
	if( NULL != pkMasterVehicle )
	{
		return pkMasterVehicle->cloneVehicle( pkProximityDatabase );
	}
	return NULL;
}

//-----------------------------------------------------------------------------
OpenSteer::AbstractVehicle* VehicleFactoryArray::createVehicle( OpenSteer::ProximityDatabase* pkProximityDatabase ) const
{
	return NULL;
};

//-----------------------------------------------------------------------------
void VehicleFactoryArray::destroyVehicle( OpenSteer::AbstractVehicle* pkVehicle ) const
{
	ET_SAFE_DELETE( pkVehicle );
}
