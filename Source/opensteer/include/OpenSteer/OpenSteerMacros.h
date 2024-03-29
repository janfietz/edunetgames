#ifndef __OPENSTEERMACROS_H__
#define __OPENSTEERMACROS_H__

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


//-----------------------------------------------------------------------------
//! some configuration macros
//! 
#ifdef WIN32
#define OS_HAVE_PROFILE 1
#elif __APPLE__
#define OS_HAVE_PROFILE 1
#else
#define OS_HAVE_PROFILE 0
#endif


//-----------------------------------------------------------------------------
// memory
//-----------------------------------------------------------------------------

#define OS_NEW new
#define OS_DELETE delete

#define OS_MALLOC malloc
#define OS_FREE free

#define OS_SAFE_DELETE( p ) { if( 0 != p ) { OS_DELETE p; p = 0; } }
#define OS_SAFE_DELETE_ARRAY( p ) { if( 0 != p ) { OS_DELETE[] p; p = 0; } }
#define OS_SAFE_FREE( p ) { if( 0 != p ) { OS_FREE( p ); p = 0; } }

//-----------------------------------------------------------------------------

#define OS_ABSTRACT = 0

#define OS_OVERRIDE override

#define OS_DECLARE_BASE( className ) typedef className BaseClass;

#define OS_DECLARE_CLASSNAME virtual const char* getClassName( void ) const OS_ABSTRACT;

#define OS_IMPLEMENT_CLASSNAME( className ) virtual const char* getClassName( void ) const { return #className; }



#endif //!  __OPENSTEERMACROS_H__
