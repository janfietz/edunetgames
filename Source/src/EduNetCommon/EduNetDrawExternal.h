#ifndef __EDUNETDRAWEXTERNAL_H__
#define __EDUNETDRAWEXTERNAL_H__

#if EDUNET_HAVE_GLUT
// Include headers for OpenGL (gl.h), OpenGL Utility Library (glu.h) and
// OpenGL Utility Toolkit (glut.h).
//
// In Mac OS X these headers are located in a different directory.
// Need to revisit conditionalization on operating system.
#if __APPLE__ && __MACH__
#include <GLUT/glut.h>   // for Mac OS X
#else
#include <GL/glut.h>     // for Linux and Windows
#endif

//-----------------------------------------------------------------------------
// GLUI
#if EDUNET_HAVE_GLUI
  #if __WIN32__
    #include "glui/GL/glui.h"
  #else
    #include "GL/glui.h"
  #endif
#endif

#endif


#endif // __EDUNETDRAWEXTERNAL_H__