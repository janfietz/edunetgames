#ifndef __ABSTRACTENTITYFACTORY_H__
#define __ABSTRACTENTITYFACTORY_H__
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

// TODO OpenSteerCollector include
#include "OpenSteerUT/OpenSteerUTTypes.h"

//-----------------------------------------------------------------------------
namespace OpenSteer
{

	//-------------------------------------------------------------------------
	class AbstractEntityFactory
	{
	public:
		virtual ~AbstractEntityFactory() {}

		virtual OpenSteer::AbstractEntity* accessMasterEntity( OpenSteer::EntityClassId ) const ET_ABSTRACT;
		
		virtual OpenSteer::AbstractEntity* getMasterEntity( void ) const ET_ABSTRACT;
		virtual void setMasterEntity( OpenSteer::AbstractEntity* pkEntity ) const ET_ABSTRACT;

		virtual OpenSteer::AbstractEntity* createEntity( OpenSteer::EntityClassId ) const ET_ABSTRACT;
		virtual OpenSteer::AbstractVehicle* createVehicle( OpenSteer::EntityClassId ) const ET_ABSTRACT;
		virtual OpenSteer::AbstractVehicle* createVehicle( void ) const ET_ABSTRACT;

		virtual bool destroyEntity( OpenSteer::AbstractEntity* pkEntity ) const ET_ABSTRACT;
		virtual bool destroyVehicle( OpenSteer::AbstractVehicle* pkVehicle ) const ET_ABSTRACT;
	protected:
	};

	//-------------------------------------------------------------------------
	class EntityFactory : public AbstractEntityFactory
	{
		ET_DECLARE_BASE( AbstractEntityFactory );
	public:
		EntityFactory():m_pkMasterEntity(NULL)
		  {
		  }

		  virtual ~EntityFactory() {}

		  virtual OpenSteer::AbstractEntity* accessMasterEntity( OpenSteer::EntityClassId classId ) const
		  {
			  if( ( NULL != this->m_pkMasterEntity ) && ( classId == this->m_pkMasterEntity->getClassId() ) )
			  {
				  return this->m_pkMasterEntity;
			  }
			  return NULL;
		  }
		  
		  virtual OpenSteer::AbstractEntity* getMasterEntity( void ) const
		  {
			  return this->m_pkMasterEntity;
		  }

		  virtual void setMasterEntity( OpenSteer::AbstractEntity* pkEntity ) const
		  {
			  this->m_pkMasterEntity = pkEntity;
		  }

		  virtual OpenSteer::AbstractEntity* createEntity( OpenSteer::EntityClassId ) const;
		  virtual OpenSteer::AbstractVehicle* createVehicle( OpenSteer::EntityClassId classId ) const;
		  virtual OpenSteer::AbstractVehicle* createVehicle( void ) const;
		  virtual bool destroyEntity( OpenSteer::AbstractEntity* pkEntity ) const;
		  virtual bool destroyVehicle( OpenSteer::AbstractVehicle* pkVehicle ) const;
	protected:


		mutable OpenSteer::AbstractEntity* m_pkMasterEntity;
	};

	//-------------------------------------------------------------------------
	// TODO: use map ?
	typedef std::vector<AbstractEntityFactory*> TEntityFactoryArray;

	//-------------------------------------------------------------------------
	class EntityFactoryArray : protected TEntityFactoryArray, public AbstractEntityFactory
	{
		ET_DECLARE_BASE( AbstractEntityFactory );
	public:
		EntityFactoryArray()
		{
		}

		virtual ~EntityFactoryArray() {}

		void addEntityFactory( AbstractEntityFactory* pkFactory );

		AbstractEntityFactory* findEntityFactory( OpenSteer::EntityClassId classId ) const;

		virtual OpenSteer::AbstractEntity* accessMasterEntity( OpenSteer::EntityClassId classId ) const
		{
			AbstractEntityFactory* pkFactory = this->findEntityFactory( classId );
			if( NULL != pkFactory )
			{
				return pkFactory->accessMasterEntity( classId );
			}
			return NULL;
		}

		virtual OpenSteer::AbstractEntity* getMasterEntity( void ) const
		{
			// a vehicle factory array does not have an own master entity
			return NULL;
		}

		virtual void setMasterEntity( OpenSteer::AbstractEntity* pkEntity ) const
		{

		}

		virtual OpenSteer::AbstractEntity* createEntity( OpenSteer::EntityClassId ) const;

		virtual OpenSteer::AbstractVehicle* createVehicle( OpenSteer::EntityClassId classId ) const;

		virtual OpenSteer::AbstractVehicle* createVehicle( void ) const;

		virtual bool destroyEntity( OpenSteer::AbstractEntity* pkEntity ) const;
		virtual bool destroyVehicle( OpenSteer::AbstractVehicle* pkVehicle ) const;
	protected:
	};

	//-------------------------------------------------------------------------
	typedef EntityFactory NullEntityFactory;

	//-------------------------------------------------------------------------
	template <class VehicleClass>
	class TEntityFactory : public EntityFactory
	{
		ET_DECLARE_BASE( EntityFactory )
	public:
		TEntityFactory()
		{
		};

		virtual ~TEntityFactory() 
		{

		};

		virtual OpenSteer::AbstractEntity* accessMasterEntity( OpenSteer::EntityClassId classId ) const
		{
			static VehicleClass kMasterVehicle;
			this->m_pkMasterEntity = &kMasterVehicle;
			return BaseClass::accessMasterEntity( classId );
		}

	};

}


#endif // __ABSTRACTENTITYFACTORY_H__
