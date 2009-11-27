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

#include "OpenSteerUT/AbstractEntityFactory.h"
#include "EduNetCommon/EduNetCommon.h"

using namespace OpenSteer;

//-----------------------------------------------------------------------------
OpenSteer::AbstractEntity* EntityFactory::createEntity( OpenSteer::EntityClassId classId ) const
{
	OpenSteer::AbstractEntity* pkEntity = OpenSteer::Plugin::createSystemEntity( classId );
	if( NULL == pkEntity )
	{
		pkEntity = this->createVehicle( classId );
	}
	return pkEntity;
}

//-----------------------------------------------------------------------------
OpenSteer::AbstractVehicle* EntityFactory::createVehicle( OpenSteer::EntityClassId classId ) const
{
	OpenSteer::AbstractVehicle* pkMasterVehicle = this->accessMasterVehicle( classId );
	if( NULL != pkMasterVehicle )
	{
		return pkMasterVehicle->cloneVehicle(  );
	}
	return NULL;
}

//-----------------------------------------------------------------------------
OpenSteer::AbstractVehicle* EntityFactory::createVehicle(  ) const
{
	if( NULL != this->m_pkMasterVehicle )
	{
		// for debugging
		const OpenSteer::EntityClassId classId = this->m_pkMasterVehicle->getClassId();
		return this->m_pkMasterVehicle->cloneVehicle( );
	}
	return NULL;
};

//-----------------------------------------------------------------------------
void EntityFactory::destroyEntity( OpenSteer::AbstractEntity* pkEntity ) const
{
	ET_SAFE_DELETE( pkEntity );
}

//-----------------------------------------------------------------------------
void EntityFactory::destroyVehicle( OpenSteer::AbstractVehicle* pkVehicle ) const
{
	ET_SAFE_DELETE( pkVehicle );
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void EntityFactoryArray::addEntityFactory( AbstractEntityFactory* pkFactory )
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
		if( NULL == this->findEntityFactory( pkMasterVehicle->getClassId() ) )
		{
			this->push_back( pkFactory );
		}
	}
	return;
}

//-----------------------------------------------------------------------------
AbstractEntityFactory* EntityFactoryArray::findEntityFactory( OpenSteer::EntityClassId classId ) const
{
	//			TEntityFactoryArray::const_iterator kFound = std::find( this->begin(), this->end(), classId );
	TEntityFactoryArray::const_iterator kIter = this->begin();
	TEntityFactoryArray::const_iterator kEnd = this->end();
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
void EntityFactoryArray::setMasterVehicle( OpenSteer::AbstractVehicle* pkVehicle ) const
{
	AbstractEntityFactory* pkFactory = this->findEntityFactory( pkVehicle->getClassId() );
	if( NULL != pkFactory )
	{
		return pkFactory->setMasterVehicle( pkVehicle );
	}
}

//-----------------------------------------------------------------------------
OpenSteer::AbstractEntity* EntityFactoryArray::createEntity( OpenSteer::EntityClassId classId ) const
{
	OpenSteer::AbstractEntity* pkEntity = OpenSteer::Plugin::createSystemEntity( classId );
	if( NULL == pkEntity )
	{
		pkEntity = this->createVehicle( classId );
	}
	return pkEntity;
}

//-----------------------------------------------------------------------------
OpenSteer::AbstractVehicle* EntityFactoryArray::createVehicle( OpenSteer::EntityClassId classId ) const
{
	OpenSteer::AbstractVehicle* pkMasterVehicle = this->accessMasterVehicle( classId );
	if( NULL != pkMasterVehicle )
	{
		return pkMasterVehicle->cloneVehicle(  );
	}
	return NULL;
}

//-----------------------------------------------------------------------------
OpenSteer::AbstractVehicle* EntityFactoryArray::createVehicle(  ) const
{
	return NULL;
};

//-----------------------------------------------------------------------------
void EntityFactoryArray::destroyEntity( OpenSteer::AbstractEntity* pkEntity ) const
{
	ET_SAFE_DELETE( pkEntity );
}

//-----------------------------------------------------------------------------
void EntityFactoryArray::destroyVehicle( OpenSteer::AbstractVehicle* pkVehicle ) const
{
	ET_SAFE_DELETE( pkVehicle );
}
