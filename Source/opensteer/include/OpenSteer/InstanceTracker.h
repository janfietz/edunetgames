#ifndef __INSTANCETRACKER_H__
#define __INSTANCETRACKER_H__

//-----------------------------------------------------------------------------
namespace OpenSteer	{

//-----------------------------------------------------------------------------
	template < 
		class TypeId
	>
	class TInstanceTracker
	{
	public:
		TInstanceTracker(): 
		  m_nConstructed(TypeId(0)), 
		  m_nDestructed(TypeId(0)),
		  m_nOnCreateCalls(TypeId(0)), 
		  m_nAssigned(TypeId(0))
		{
		}

		virtual ~TInstanceTracker() {}

		size_t Constructor()
		{
			return ++this->m_nConstructed;
		}

		size_t Destructor()
		{
			return ++this->m_nDestructed;
		}

		size_t Assign()
		{
			return ++this->m_nAssigned;
		}

		size_t OnCreate()
		{
			return ++this->m_nOnCreateCalls;
		}

		size_t GetConstructedCount() const
		{
			return this->m_nConstructed;
		}

		size_t GetDestructedCount() const
		{
			return this->m_nDestructed;
		}

		size_t GetAssignCount() const
		{
			return this->m_nAssigned;
		}

		size_t GetInstanceCount() const
		{
			if (this->m_nDestructed > this->m_nConstructed)
			{
				return TypeId(0);
			}
			return this->m_nConstructed - this->m_nDestructed;
		}

		void Reset()
		{
			m_nConstructed = m_nDestructed = m_nAssigned = m_nOnCreateCalls = TypeId(0);
		}
		typedef TypeId Id;

	private:
		TypeId m_nConstructed;
		TypeId m_nDestructed;
		TypeId m_nAssigned;
		TypeId m_nOnCreateCalls;
	};

	typedef TInstanceTracker<size_t> InstanceTracker;


} // namespace EduNet

#endif // __INSTANCECOUNT_H__