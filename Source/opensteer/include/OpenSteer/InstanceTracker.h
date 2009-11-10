#ifndef __INSTANCETRACKER_H__
#define __INSTANCETRACKER_H__

//-----------------------------------------------------------------------------
namespace OpenSteer	{

//-----------------------------------------------------------------------------
	class InstanceTracker
	{
	public:
		InstanceTracker(): m_nConstructed(0), m_nDestructed(0), m_nOnCreateCalls(0), m_nAssigned(0)
		{
		}

		virtual ~InstanceTracker() {}

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
				return 0;
			}
			return this->m_nConstructed - this->m_nDestructed;
		}

		void Reset()
		{
			m_nConstructed = m_nDestructed = m_nAssigned = m_nOnCreateCalls = 0;
		}

	private:
		size_t m_nConstructed;
		size_t m_nDestructed;
		size_t m_nAssigned;
		size_t m_nOnCreateCalls;
	};


} // namespace EduNet

#endif // __INSTANCECOUNT_H__