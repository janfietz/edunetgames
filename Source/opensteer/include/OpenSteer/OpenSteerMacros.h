#ifndef __OPENSTEERMACROS_H__
#define __OPENSTEERMACROS_H__

/*
 *  OpenSteerMacros.h
 *  OpenSteer
 *
 *  Created by Cyrus Preuss on 11/9/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#define OS_ABSTRACT = 0


#define OS_DECLARE_CLASSNAME virtual const char* getClassName( void ) const OS_ABSTRACT;


#define OS_IMPLEMENT_CLASSNAME( className ) virtual const char* getClassName( void ) const { return #className; }



#endif //  __OPENSTEERMACROS_H__
