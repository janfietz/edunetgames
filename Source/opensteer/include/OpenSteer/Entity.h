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
#include "OpenSteer/AbstractEntity.h"
#include "OpenSteer/AbstractPlayer.h"



//-----------------------------------------------------------------------------
namespace OpenSteer {


	AbstractPlayer* CastToAbstractPlayer( AbstractEntity* pkEntity );

	//-------------------------------------------------------------------------
	template <class Super>
	class EntityPossessionMixin : public Super
	{
	public:
		EntityPossessionMixin():
			m_pkPossessor(NULL),
			m_pkPossessed(NULL)
		{
		}

		virtual ~EntityPossessionMixin()
		{
			if( NULL != this->getControlledEntity() )
			{
				this->play( NULL );
			}
			AbstractPlayer* pkPlayer = this->getPlayer();
			if( NULL != pkPlayer )
			{
				pkPlayer->play( NULL );
			}
		}

		//---------------------------------------------------------------------
		// AbstractEntity interface
		virtual void play( AbstractEntity* pkEntity )
		{
			if( false == this->isPlayer() )
			{
				// not a player
				return;
			}
			AbstractEntity* pkThisEntity = dynamic_cast<AbstractEntity*>( this );
			AbstractPlayer* pkThis = CastToAbstractPlayer( pkThisEntity );
			if( NULL != pkEntity )
			{
				pkEntity->possessBy( pkThisEntity );
				AbstractPlayer* pkPlayer = pkEntity->getPlayer();
				if( pkPlayer == pkThis )
				{
					// play succeeded
					this->m_pkPossessed = pkEntity;
				}
			}
			else
			{
				this->m_pkPossessed = NULL;
			}
		};

		virtual void possessBy( AbstractEntity* pkEntity )
		{
			if( NULL != this->m_pkPossessor )
			{
				this->m_pkPossessor->play( NULL );
			}
			this->m_pkPossessor = pkEntity;
		}

		virtual AbstractPlayer* getPlayer( void ) const
		{
			return CastToAbstractPlayer( this->m_pkPossessor );
		}

		virtual AbstractEntity* getControlledEntity( void ) const
		{
			return this->m_pkPossessed;
		}

		virtual bool isPossessed( void ) const 
		{
			return (NULL != this->getPlayer());
		}

		virtual bool isPlayer( void ) const
		{
			EntityPossessionMixin* pkThis = (EntityPossessionMixin*)this;
			AbstractEntity* pkThisEntity = dynamic_cast<AbstractEntity*>( pkThis );
			AbstractPlayer* pkThisPlayer = OpenSteer::CastToAbstractPlayer( pkThisEntity );
			return ( NULL != pkThisPlayer );
		}
	private:
		AbstractEntity* m_pkPossessor;
		AbstractEntity* m_pkPossessed;

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
		  return OS_CID_UNKNOWN;
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

//	typedef EntityPossessionMixin<EntityInstance_0> EntityInstance;

	//-------------------------------------------------------------------------
	template <class Super>
	class EntityMixin : public EntityPossessionMixin<Super>
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
/*
		virtual void play( AbstractEntity* pkEntity )
		{
			this->m_kInstance.play( pkEntity );
		}

		virtual void possessBy( AbstractEntity* pkEntity )
		{
			this->m_kInstance.possessBy( pkEntity );
		}

		virtual AbstractPlayer* getPlayer( void ) const
		{
			return this->m_kInstance.getPlayer( );
		}

		virtual AbstractEntity* getControlledEntity( void ) const
		{
			return this->m_kInstance.getControlledEntity( );
		}
*/
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

		//---------------------------------------------------------------------
		// AbstractEntity interface
		virtual AbstractEntity* cloneEntity( void ) const
		{
			return new EntityClassIdMixin();
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
