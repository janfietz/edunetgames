#ifndef __EDUNETDRAWEXTERNAL_H__
#define __EDUNETDRAWEXTERNAL_H__

#if EDUNET_HAVE_GLUT
// Include headers for OpenGL (gl.h), OpenGL Utility Library (glu.h) and
// OpenGL Utility Toolkit (glut.h).
//
// In Mac OS X these headers are located in a different directory.
// Need to revisit conditionalization on operating system.
#if __APPLE__ && __MACH__
    #include <OpenGL/gl.h>   // for Mac OS X
    #include <OpenGL/glu.h>   // for Mac OS X
    #ifndef HAVE_NO_GLUT
        #include <GLUT/glut.h>   // for Mac OS X
    #endif
#else
    #ifdef _MSC_VER
        #include <windows.h>
    #endif
    #include <GL/gl.h>     // for Linux and Windows
    #include <GL/glu.h>     // for Linux and Windows
	#ifdef _MSC_VER
		#ifndef HAVE_NO_GLUT
		#include "GL/glut.h"   // for Windows
		#endif
	#else
		#ifndef HAVE_NO_GLUT
		#include <GL/glut.h>   // for Linux
		#endif
	#endif
#endif


//-----------------------------------------------------------------------------
// undocumented windows hacks
#ifdef WIN32
extern "C" {
	int APIENTRY __glutCreateWindowWithExit(const char *title, void (__cdecl *exitfunc)(int));
	void APIENTRY __glutInitWithExit(int *argcp, char **argv, void (__cdecl *exitfunc)(int));
}
#endif //WIN32

//-----------------------------------------------------------------------------
// GLUI
#if EDUNET_HAVE_GLUI
    #include "glui/GL/glui.h"
#endif

#endif

#endif // __EDUNETDRAWEXTERNAL_H__
