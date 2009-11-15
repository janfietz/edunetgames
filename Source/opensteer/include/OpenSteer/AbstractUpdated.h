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
		virtual void update( const osScalar /*currentTime*/, const osScalar /*elapsedTime*/ ) OS_ABSTRACT;
	};
}

#endif  //! __ABSTRACTUPDATED_H__
