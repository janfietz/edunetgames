#ifndef __INSTANCETRACKER_H__
#define __INSTANCETRACKER_H__

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


//! for size_t
#include <cstring>

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

		TypeId Constructor()
		{
			return ++this->m_nConstructed;
		}

		TypeId Destructor()
		{
			return ++this->m_nDestructed;
		}

		TypeId Assign()
		{
			return ++this->m_nAssigned;
		}

		TypeId OnCreate()
		{
			return ++this->m_nOnCreateCalls;
		}

		TypeId GetConstructedCount() const
		{
			return this->m_nConstructed;
		}

		TypeId GetDestructedCount() const
		{
			return this->m_nDestructed;
		}

		TypeId GetAssignCount() const
		{
			return this->m_nAssigned;
		}

		TypeId GetInstanceCount() const
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
//	typedef TInstanceTracker<unsigned int> InstanceTracker;


} //! namespace EduNet

#endif //! __INSTANCECOUNT_H__
