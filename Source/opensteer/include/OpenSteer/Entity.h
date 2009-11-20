#ifndef __ENTITY_H__
#define __ENTITY_H__

//-----------------------------------------------------------------------------
//! Copyright (c) 2009, Jan Fietz, Cyrus Preuss
//! All rights reserved.
//! 
//! Redistribution and use in source and binary forms, with or without modification, 
//! are permitted provided that the following conditions are met:
//! 
//! * Redistributions of source code must retain the above copyright notice, 
//!   this list of conditions and the following disclaimer.
//! * Redistributions in binary form must reproduce the above copyright notice, 
//!   this list of conditions and the following disclaimer in the documentation 
//!   and/or other materials provided with the distribution.
//! * Neither the name of EduNetGames nor the names of its contributors
//!   may be used to endorse or promote products derived from this software
//!   without specific prior written permission.
//! 
//! THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
//! ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
//! WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
//! IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
//! INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES 
//! (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
//! LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON 
//! ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
//! (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, 
//! EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//-----------------------------------------------------------------------------

#include "OpenSteer/OpenSteerTypes.h"
#include "OpenSteer/OpenSteerMacros.h"
#include "OpenSteer/InstanceTracker.h"

//-----------------------------------------------------------------------------
namespace OpenSteer {

	using namespace OpenSteer;

	typedef uint64_t NetworkId; 
	typedef uint64_t EntityClassId; 

	//-------------------------------------------------------------------------
	// implement entity class id
	static const EntityClassId g_clasId_Unknown(0);	

	static const EntityClassId g_clasId_Entity(1);

	static const EntityClassId g_clasId_LocalSpace(2);

	// player control interface ids
	static const EntityClassId g_clasId_Player(3);
	static const EntityClassId g_clasId_PlayerController(4);


	// obstacle interface ids
	static const EntityClassId g_clasId_Obstacle(5);
	static const EntityClassId g_clasId_SphereObstacle(6);
	static const EntityClassId g_clasId_BoxObstacle(7);
	static const EntityClassId g_clasId_PlaneObstacle(8);
	static const EntityClassId g_clasId_RectangleObstacle(9);
	static const EntityClassId g_clasId_LastReserved(1000);
}

#define OS_CID_UNKNOWN OpenSteer::g_clasId_Unknown
#define OS_CID_ENTITY OpenSteer::g_clasId_Entity
#define OS_CID_LOCALSPACE OpenSteer::g_clasId_LocalSpace

#define OS_CID_PLAYER OpenSteer::g_clasId_Player
#define OS_CID_PLAYERCONTROLLER OpenSteer::g_clasId_PlayerController

#define OS_CID_OBSTACLE OpenSteer::g_clasId_Obstacle
#define OS_CID_SPHEREOBSTACLE OpenSteer::g_clasId_SphereObstacle
#define OS_CID_BOXOBSTACLE OpenSteer::g_clasId_BoxObstacle
#define OS_CID_PLANEOBSTACLE OpenSteer::g_clasId_PlaneObstacle
#define OS_CID_RECTANGLEOBSTACLE OpenSteer::g_clasId_RectangleObstacle

#define OS_CID_LASTRESERVED OpenSteer::g_clasId_LastReserved


//-----------------------------------------------------------------------------
namespace OpenSteer {

	//-------------------------------------------------------------------------
	class AbstractEntity
	{
	public:
		virtual ~AbstractEntity() { /* Nothing to do. */ }

		OS_DECLARE_CLASSNAME

		//! return a pointer to a cloned instance of this entity
		virtual AbstractEntity* cloneEntity( void ) const = 0;

		//! return the unique class id of this object
		virtual EntityClassId getClassId( void ) const = 0;

		//! return the unique instance id of this object
		virtual InstanceTracker::Id getEntityId( void ) const = 0;

		//! return the unique network id of this object
		virtual NetworkId getNetworkId( void ) const = 0;

		//! set the unique network id of this object
		virtual void setNetworkId( NetworkId ) = 0;

		//! set if this object is a remote object or not
		virtual void setIsRemoteObject( bool bIsRemote ) = 0;

		//! return if this object is a remote object
		virtual bool isRemoteObject( void ) const = 0;

		//! return a pointer to this instance's character string name
		virtual const char* name (void) const = 0;

	};

	//-------------------------------------------------------------------------
	class EntityInstance
	{
	public:
		EntityInstance():
		  m_uiId( ms_InstanceTracker.Constructor() ),
		  m_netWorkId( NetworkId(0) ),
		  m_bIsRemoteObject( false )
		{

		}

		virtual ~EntityInstance()
		{
		  ms_InstanceTracker.Destructor();
		}

		virtual EntityClassId getClassId( void ) const
		{
		  return EntityClassId( 0 );
		}

		InstanceTracker::Id getEntityId( void ) const
		{
			return m_uiId; 
		}

		NetworkId getNetworkId( void ) const
		{
			return m_netWorkId;
		}

		void setNetworkId( NetworkId id )
		{
			m_netWorkId = id;
		}

		void setIsRemoteObject( bool bIsRemote )
		{
			m_bIsRemoteObject = bIsRemote;
		}

		bool isRemoteObject( void ) const
		{
			return m_bIsRemoteObject;
		}

		virtual const char* name (void) const
		{
			return "Unknown";
		}
	private:
		InstanceTracker::Id m_uiId;
		NetworkId m_netWorkId;
		bool m_bIsRemoteObject;

		static InstanceTracker ms_InstanceTracker;
	};

	//-------------------------------------------------------------------------
	template <class Super>
	class EntityMixin : public Super
	{
	public:
		virtual ~EntityMixin()
		{
		}

		OS_IMPLEMENT_CLASSNAME( Super )

		virtual AbstractEntity* cloneEntity( void ) const
		{
			return NULL;
		}

		virtual EntityClassId getClassId( void ) const
		{
			return this->m_kInstance.getClassId( );
		}

		virtual InstanceTracker::Id getEntityId( void ) const
		{
			return this->m_kInstance.getEntityId();
		}

		virtual NetworkId getNetworkId( void ) const
		{
			return this->m_kInstance.getNetworkId();
		}

		virtual void setNetworkId( NetworkId id )
		{
			this->m_kInstance.setNetworkId( id );
		}

		virtual void setIsRemoteObject( bool bIsRemote )
		{
			this->m_kInstance.setIsRemoteObject( bIsRemote );
		}

		virtual bool isRemoteObject( void ) const
		{
			return this->m_kInstance.isRemoteObject();
		}

		virtual const char* name (void) const
		{
			return this->m_kInstance.name();
		}
	private:
		EntityInstance m_kInstance;
	};

	//-------------------------------------------------------------------------
	template <class Super, EntityClassId classId = 0>
	class EntityClassIdMixin : public Super
	{
	public:
		EntityClassIdMixin()
		{
		}

		virtual ~EntityClassIdMixin()
		{
		}

		virtual EntityClassId getClassId( void ) const
		{
			static EntityClassId sClassId = classId;
			return sClassId;
		}
	private:

	};

	typedef EntityMixin<AbstractEntity> Entity;

	const Entity gGlobalEntity;

}

#endif //! __ENTITY_H__
