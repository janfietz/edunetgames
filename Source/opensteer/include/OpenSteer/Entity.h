#ifndef __ENTITY_H__
#define __ENTITY_H__

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

#include "OpenSteer/OpenSteerTypes.h"
#include "OpenSteer/OpenSteerMacros.h"
#include "OpenSteer/InstanceTracker.h"

//-----------------------------------------------------------------------------
namespace OpenSteer {



	typedef uint64_t NetworkId; 
	typedef uint64_t EntityClassId; 


	//-------------------------------------------------------------------------
	class AbstractEntity
	{
	public:
		virtual ~AbstractEntity() { /* Nothing to do. */ }

		OS_DECLARE_CLASSNAME

		virtual InstanceTracker::Id getEntityId( void ) const = 0;

		virtual NetworkId getNetworkId( void ) const = 0;
		virtual void setNetworkId( NetworkId ) = 0;

		virtual void setIsRemoteObject( bool bIsRemote ) = 0;
		virtual bool isRemoteObject( void ) const = 0;
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
		InstanceTracker::Id getId( void ) const
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
		OS_IMPLEMENT_CLASSNAME( Super )

		virtual InstanceTracker::Id getEntityId( void ) const
		{
			return this->m_kInstance.getId();
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
	private:
		EntityInstance m_kInstance;
	};

	typedef EntityMixin<AbstractEntity> Entity;

	const Entity gGlobalEntity;

}

#endif // __ENTITY_H__
