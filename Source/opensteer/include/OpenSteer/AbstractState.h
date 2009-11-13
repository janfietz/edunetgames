#ifndef __ABSTRACTSTATE_H__
#define __ABSTRACTSTATE_H__

/*
 *  AbstractState.h
 *  OpenSteer
 *
 *  Created by Cyrus Preuss on 11/9/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include <vector>
#include "OpenSteer/AbstractUpdated.h"

//-----------------------------------------------------------------------------
namespace OpenSteer {
	
	using namespace OpenSteer;
	
	//-------------------------------------------------------------------------
	class AbstractState : public AbstractUpdated  {
	public:
		virtual ~AbstractState() { /* Nothing to do. */ }
	};
	
	
	//-------------------------------------------------------------------------
	template <class entity_type, 
	typename TypeStateId = int,
	typename TypeAccumTime = osScalar,
	typename TypeDeltaTime = osScalar
	>
	class TState //: public AbstractState
	{
	public:
		
		TState( TypeStateId nStateId = static_cast<TypeStateId> (-1) ):
		m_nStateId(nStateId) 
		{}
		
		virtual ~TState() 
		{}
		
		TypeStateId getStateId( void ) const { return this->m_nStateId; }
		void setStateId( TypeStateId nId ) { this->m_nStateId = nId; }
		
		//this will execute when the state is entered
		virtual void enter( entity_type* ) OS_ABSTRACT;
		
		//this is the states normal update function
		virtual void execute( entity_type*, TypeAccumTime fAccumTime, TypeDeltaTime fDeltaTime ) OS_ABSTRACT;
		
		//this will execute when the state is exited
		virtual void exit( entity_type* ) OS_ABSTRACT;
		
		OS_IMPLEMENT_CLASSNAME( TState );

	protected:
//		typedef std::vector<TState< entity_type, TypeStateId, TypeAccumTime, TypeDeltaTime >::TState> TStateArray;

//		typedef TState TSingleState;
		
//		typedef std::vector<TSingleState> 
//		TStateArray;
//		TStateArray m_kStates;
		
	private:
		TypeStateId m_nStateId;
		
	};

	//-------------------------------------------------------------------------
	
	//-------------------------------------------------------------------------
	template <class entity_type, 
	typename TypeStateId = int,
	typename TypeAccumTime = osScalar,
	typename TypeDeltaTime = osScalar>
	class TStateMachine : public TState< entity_type, TypeStateId, TypeAccumTime, TypeDeltaTime >
	{
	public:
		
		TStateMachine( TypeStateId nStateId = static_cast<TypeStateId> (-1) ):
		TState< entity_type, TypeStateId, TypeAccumTime, TypeDeltaTime >(nStateId) {}
		virtual ~TStateMachine() {}
		
		
		//this will execute when the state is entered
		virtual void enter( entity_type* ) {};//OS_ABSTRACT;
		
		//this is the states normal update function
		virtual void execute( entity_type* pkEntity, TypeAccumTime fAccumTime, TypeDeltaTime fDeltaTime )
		{
			const size_t numStates = m_kStates.size();
			if( numStates > 0 )
			{
				TState< entity_type, TypeStateId, TypeAccumTime, TypeDeltaTime >& kState = 
				m_kStates.back();
				kState.execute( pkEntity, fAccumTime, fDeltaTime );
			}
		}
		
		//this will execute when the state is exited
		virtual void exit( entity_type* ) {};//OS_ABSTRACT;
		
		void pushState( const TState< entity_type, TypeStateId, TypeAccumTime, TypeDeltaTime >& kState )
		{
			m_kStates.push_back( kState );
		}

		void popState( void )
		{
			const size_t numStates = m_kStates.size();
			if( numStates > 0 )
			{
				TState< entity_type, TypeStateId, TypeAccumTime, TypeDeltaTime >& kState = 
				m_kStates.back();
				m_kStates.pop_back();
			}
		}
		
		OS_IMPLEMENT_CLASSNAME( TStateMachine );
	private:
		TStateVector< entity_type, TypeStateId, TypeAccumTime, TypeDeltaTime > m_kStates;
		
	};
	

	// idee: leave definiert target state - implementiert statechange
	//       leaveConition() boolean
	
	
	
	
	
}



#endif //  __ABSTRACTSTATE_H__
