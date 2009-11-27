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

FIND_PACKAGE(GLUT)
IF (GLUT_FOUND)
MESSAGE("GLUT FOUND")
MESSAGE(${GLUT_glut_LIBRARY})
ELSE (GLUT_FOUND)
	MESSAGE("GLUT NOT FOUND")
	Set( GLUT_INCLUDE_DIR ${EDUNET_GAMES_SOURCE_DIR}/../ThirdParty/glut)
ENDIF (GLUT_FOUND)

FIND_PACKAGE(RakNet)
IF (RakNet_FOUND)
MESSAGE("RakNet FOUND")
MESSAGE(${RakNet_LIBRARY})
ELSE (RakNet_FOUND)
	MESSAGE("Raknet NOT FOUND")
	Set( RakNet_INCLUDES ${EDUNET_GAMES_SOURCE_DIR}/../ThirdParty/RakNet/Source )
	MESSAGE("Raknet include " ${RakNet_INCLUDES} )
	IF(DEBUG)
		Set( RakNet_LIBRARY ${EDUNET_GAMES_SOURCE_DIR}/../ThirdParty/RakNet/lib/RakNetLibStatic.lib )
	ELSE(DEBUG)
		Set( RakNet_LIBRARY ${EDUNET_GAMES_SOURCE_DIR}/../ThirdParty/RakNet/lib/RakNetLibStaticDebug.lib )
	ENDIF(DEBUG)
ENDIF (RakNet_FOUND)
Set( RakNet_SOURCES ${EDUNET_GAMES_SOURCE_DIR}/../ThirdParty/RakNet )

#FIND_PACKAGE(GLUI)
#IF (GLUI_FOUND)
#MESSAGE(${GLUI_LIBRARY})
#ELSE (GLUI_FOUND)
#MESSAGE("GLUI NOT FOUND")
#ENDIF (GLUI_FOUND)

FIND_PACKAGE(Argtable)
IF (Argtable_FOUND)
MESSAGE(${Argtable_LIBRARY})
ELSE (Argtable_FOUND)
MESSAGE("ArtableNOT FOUND")
	Set( Argtable_INCLUDES ${EDUNET_GAMES_SOURCE_DIR}/../ThirdParty/argtable2-11/src )
	IF(DEBUG)
		Set( Argtable_LIBRARY ${EDUNET_GAMES_SOURCE_DIR}/../ThirdParty/argtable2-11/src/argtable.lib )
	ELSE(DEBUG)
		Set( Argtable_LIBRARY ${EDUNET_GAMES_SOURCE_DIR}/../ThirdParty/argtable2-11/src/argtabled.lib )
	ENDIF(DEBUG)
ENDIF (Argtable_FOUND)


IF (MINGW)
MESSAGE ("GLUT NOT FOUND not found, trying to use MINGW glut32")
SET(GLUT_glut_LIBRARY glut32)
ENDIF (MINGW)

IF (MSVC)
MESSAGE ("GLUT NOT FOUND, trying to use Bullet/Glut/glut32.lib for MSVC")
SET(GLUT_glut_LIBRARY ${EDUNET_GAMES_SOURCE_DIR}/Glut/glut32.lib)
ENDIF (MSVC)

INCLUDE_DIRECTORIES(${EDUNET_GAMES_SOURCE_DIR}/Source
${EDUNET_GAMES_SOURCE_DIR}/Source/src
${EDUNET_GAMES_SOURCE_DIR}/External
${GLUT_INCLUDE_DIR}
${RakNet_INCLUDES}/raknet
${Argtable_INCLUDES}
 )

IF (WIN32)
  #INCLUDE_DIRECTORIES(${GLUT_INCLUDE_DIR} ${RakNet_INCLUDES}/raknet ${Argtable_INCLUDES} )
ELSE (WIN32)
  # This is the lines for linux.  This should always work if everything is installed and working fine.
  INCLUDE_DIRECTORIES(/usr/include /usr/local/include )
ENDIF (WIN32)

SUBDIRS(Source External)
