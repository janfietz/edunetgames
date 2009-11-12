#ifndef __EDUNETDRAWCONFIG_H__
#define __EDUNETDRAWCONFIG_H__

#ifndef EDUNET_NEEDS_OGL
#define EDUNET_NEEDS_OGL 0
#endif // EDUNET_NEEDS_OGL

// glut
#ifndef EDUNET_HAVE_GLUT
#define EDUNET_HAVE_GLUT EDUNET_NEEDS_OGL
#endif

// glui
#ifndef EDUNET_HAVE_GLUI
#define EDUNET_HAVE_GLUI EDUNET_NEEDS_OGL
#endif


#endif // __EDUNETDRAWCONFIG_H__