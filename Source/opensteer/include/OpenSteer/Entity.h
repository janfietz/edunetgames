#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "InstanceTracker.h"

//-----------------------------------------------------------------------------
namespace OpenSteer {


	//-------------------------------------------------------------------------
	class AbstractEntity
	{
	public:
		virtual ~AbstractEntity() { /* Nothing to do. */ }

		virtual size_t getEntityId( void ) const = 0;
	};

	//-------------------------------------------------------------------------
	class EntityInstance
	{
	public:
		EntityInstance():m_uiId( ms_InstanceTracker.Constructor() )
		{

		}
		virtual ~EntityInstance()
		{
			ms_InstanceTracker.Destructor();
		}
		size_t getId( void ) const
		{
			return m_uiId; 
		}
	private:
		size_t m_uiId;
		static InstanceTracker ms_InstanceTracker;
	};

	//-------------------------------------------------------------------------
	template <class Super>
	class EntityMixin : public Super
	{
	public:
		virtual size_t getEntityId( void ) const
		{
			return this->m_kInstance.getId();
		}
	private:
		EntityInstance m_kInstance;
	};

	typedef EntityMixin<AbstractEntity> Entity;

	const Entity gGlobalEntity;

}

#endif // __ENTITY_H__