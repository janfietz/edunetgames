#ifndef __EDUNETMACROS_H__
#define __EDUNETMACROS_H__


#define ET_DECLARE_BASE( classname ) typedef classname BaseClass;

//-----------------------------------------------------------------------------
//  macros spit out 'clickable' file and line number
//-----------------------------------------------------------------------------
// with line number
#ifndef ET_STRING
# define ET_STRING2(x) #x
# define ET_STRING(x) ET_STRING2(x)
#endif //  KK_STRING

// sample 1
//#pragma message (__FILE__ "(" ET_STRING(__LINE__) "): USE Macros ")
#define ET_SOURCE_MESSAGE __FILE__ "(" ET_STRING(__LINE__) "):"
// sample 2
//#pragma message( ET_SOURCE_MESSAGE "Warning: Unicode disabled! Check your project settings!")


//-----------------------------------------------------------------------------
//! implement an empty private copy constructor and a private assignment
#define ET_IMPLEMENT_CLASS_NO_COPY( classname ) private:\
	classname( const classname& );\
	classname& operator=( const classname& );

#endif // __EDUNETMACROS_H__