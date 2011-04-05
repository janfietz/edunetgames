#ifndef __EDUNETDRAWEXTERNAL_H__
#define __EDUNETDRAWEXTERNAL_H__

#if EDUNET_HAVE_GLUT
// Include headers for OpenGL (gl.h), OpenGL Utility Library (glu.h) and
// OpenGL Utility Toolkit (glut.h).
//


//-----------------------------------------------------------------------------
// GLUI
#if EDUNET_HAVE_GLUI
  #ifdef WIN32
    #include "glui/GL/glui.h"
  #else
#if __APPLE__ && __MACH__
	#include "glui/GL/glui.h"
#else
    #include "GL/glui.h"
#endif
  #endif
#endif

#endif


#endif // __EDUNETDRAWEXTERNAL_H__
