#ifndef __ABSTRACTUPDATED_H__
#define __ABSTRACTUPDATED_H__

/*
 *  AbstractUpdated.h
 *  OpenSteer
 *
 *  Created by Cyrus Preuss on 11/9/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "OpenSteer/OpenSteerMacros.h"
#include "OpenSteer/OpenSteerTypes.h"

//-----------------------------------------------------------------------------
namespace OpenSteer {
	
	//-------------------------------------------------------------------------
	class AbstractUpdated {
    public:
        virtual ~AbstractUpdated( void ) { /* Nothing to do. */ }
		virtual void updateCustom( AbstractUpdated* pkParent, const osScalar /*currentTime*/, const osScalar /*elapsedTime*/ ) OS_ABSTRACT;
		virtual void update( const osScalar /*currentTime*/, const osScalar /*elapsedTime*/ ) OS_ABSTRACT;
		virtual void setCustomUpdated( AbstractUpdated* ) OS_ABSTRACT;
		virtual AbstractUpdated* getCustomUpdated( void ) const OS_ABSTRACT; 
		virtual bool isEnabled( void ) const OS_ABSTRACT; 
		virtual void setEnabled( bool bEnabled ) OS_ABSTRACT; 
	};

	//-------------------------------------------------------------------------
	template <class Super>
	class AbstractUpdatedMixin : public Super {
	public:
		AbstractUpdatedMixin(  ): m_pkCustomUpdated(0),m_bEnabled(true)
		{
		}
		virtual ~AbstractUpdatedMixin(){}

		//-------------------------------------------------------------------
		// interface AbstractUpdated
		//---------------------------------------------------------------------
		virtual void updateCustom( AbstractUpdated* /*pkParent*/, const osScalar /*currentTime*/, const osScalar /*elapsedTime*/ )
		{
			// nothing to do here
			return;
		}
		virtual void update( const osScalar currentTime, const osScalar elapsedTime )
		{
			if( 0 != this->m_pkCustomUpdated )
			{
				// in case the custom updater decides to call the base class
				// prevent infinite recursion, store the custom updater locally
				// and restore it once done with the update
				AbstractUpdated* pkCustomUpdated = this->m_pkCustomUpdated;
				this->m_pkCustomUpdated = 0;
				pkCustomUpdated->updateCustom( this, currentTime, elapsedTime );
				this->m_pkCustomUpdated = pkCustomUpdated;
			}
		}

		virtual void setCustomUpdated( AbstractUpdated* pkUpdated )
		{
			this->m_pkCustomUpdated = pkUpdated;
		}

		virtual AbstractUpdated* getCustomUpdated( void ) const
		{
			return this->m_pkCustomUpdated;
		}

		virtual bool isEnabled( void ) const { return this->m_bEnabled; }; 
		virtual void setEnabled( bool bEnabled ){ this->m_bEnabled = bEnabled; }; 

	protected:
		AbstractUpdated* m_pkCustomUpdated;
		bool m_bEnabled;

	};

}

#endif  //! __ABSTRACTUPDATED_H__
