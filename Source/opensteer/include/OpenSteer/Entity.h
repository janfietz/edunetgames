#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "OpenSteer/OpenSteerTypes.h"
#include "OpenSteer/OpenSteerMacros.h"
#include "OpenSteer/InstanceTracker.h"

//-----------------------------------------------------------------------------
namespace OpenSteer {



	typedef uint64_t NetworkId; 


	//-------------------------------------------------------------------------
	class AbstractEntity
	{
	public:
		virtual ~AbstractEntity() { /* Nothing to do. */ }

		OS_DECLARE_CLASSNAME

		virtual InstanceTracker::Id getEntityId( void ) const = 0;

		virtual NetworkId getNetworkId( void ) const = 0;
		virtual void setNetworkId( NetworkId ) = 0;

	};

	//-------------------------------------------------------------------------
	class EntityInstance
	{
	public:
		EntityInstance():
		  m_uiId( ms_InstanceTracker.Constructor() ),
		  m_netWorkId( NetworkId(0) )
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

	private:
		InstanceTracker::Id m_uiId;
		NetworkId m_netWorkId;
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

	private:
		EntityInstance m_kInstance;
	};

	typedef EntityMixin<AbstractEntity> Entity;

	const Entity gGlobalEntity;

}

#endif // __ENTITY_H__