cmake_minimum_required(VERSION 2.6)

PROJECT(EDUNET_GAMES)
SET(EDUNET_GAMES 0.01)

IF (NOT CMAKE_BUILD_TYPE)
# SET(CMAKE_BUILD_TYPE "Debug")
 SET(CMAKE_BUILD_TYPE "Release")
ENDIF (NOT CMAKE_BUILD_TYPE) 

# string (REPLACE "/D_WINDOWS" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
remove_definitions(-D_WINDOWS )
   
IF(COMMAND cmake_policy)
   cmake_policy(SET CMP0003 NEW)
ENDIF(COMMAND cmake_policy)
   
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMakeModules/")

# This is the shortcut to finding GLU, GLUT and OpenGL if they are properly installed on your system
# This should be the case.

FIND_PACKAGE(OpenGL)
IF (OPENGL_FOUND)
MESSAGE("OPENGL FOUND")
MESSAGE(${OPENGL_LIBRARIES})
ELSE (OPENGL_FOUND)
MESSAGE("OPENGL NOT FOUND")
SET(OPENGL_gl_LIBRARY opengl32)
SET(OPENGL_glu_LIBRARY glu32)
ENDIF (OPENGL_FOUND)

# ADD_DEFINITIONS(-DBT_USE_FREEGLUT)

FIND_PACKAGE(GLU)

FIND_PACKAGE(GLUT REQUIRED)
IF (GLUT_FOUND)
MESSAGE("GLUT FOUND")
MESSAGE(${GLUT_glut_LIBRARY})
ELSE (GLUT_FOUND)
MESSAGE("GLUT NOT FOUND")
ENDIF (GLUT_FOUND)

FIND_PACKAGE(RakNet REQUIRED)
IF (RakNet_FOUND)
MESSAGE("RakNet FOUND")
MESSAGE(${RakNet_LIBRARY})
ELSE (RakNet_FOUND)
MESSAGE("RakNet NOT FOUND")
ENDIF (RakNet_FOUND)

FIND_PACKAGE(GLUI REQUIRED)
IF (GLUI_FOUND)
MESSAGE(${GLUI_LIBRARY})
ENDIF (GLUI_FOUND)

FIND_PACKAGE(Argtable REQUIRED)
IF (Argtable_FOUND)
MESSAGE(${Argtable_LIBRARY})
ENDIF (Argtable_FOUND)


IF (MINGW)
MESSAGE ("GLUT NOT FOUND not found, trying to use MINGW glut32")
SET(GLUT_glut_LIBRARY glut32)
ENDIF (MINGW)

IF (MSVC)
MESSAGE ("GLUT NOT FOUND, trying to use Bullet/Glut/glut32.lib for MSVC")
SET(GLUT_glut_LIBRARY ${EDUNET_GAMES_SOURCE_DIR}/Glut/glut32.lib)
ENDIF (MSVC)


IF (WIN32)
  INCLUDE_DIRECTORIES(${EDUNET_GAMES_SOURCE_DIR}/Glut)
ELSE (WIN32)
  # This is the lines for linux.  This should always work if everything is installed and working fine.
  INCLUDE_DIRECTORIES(/usr/include /usr/local/include ${GLUT_INCLUDE_DIR} ${RakNet_INCLUDES}/raknet) 
ENDIF (WIN32)

SUBDIRS(Source)