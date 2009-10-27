#ifndef __EDUNETMACROS_H__
#define __EDUNETMACROS_H__




//-----------------------------------------------------------------------------
//  macros spit out 'clickable' file and line number
//-----------------------------------------------------------------------------
// with line number
#ifndef RK_STRING
# define RK_STRING2(x) #x
# define RK_STRING(x) RK_STRING2(x)
#endif //  KK_STRING

// sample 1
//#pragma message (__FILE__ "(" RK_STRING(__LINE__) "): USE KernelMacros ")
#define RK_SOURCE_MESSAGE __FILE__ "(" RK_STRING(__LINE__) "):"
// sample 2
//#pragma message( RK_SOURCE_MESSAGE "Warning: Unicode disabled! Check your project settings!")



#endif // __EDUNETMACROS_H__