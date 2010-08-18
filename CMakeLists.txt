cmake_minimum_required(VERSION 2.6)

PROJECT(EDUNET_GAMES)
SET(EDUNET_GAMES 0.01)
######################################################################
# return a list of directories that we should add_subdirectory()
macro(EDUNET_COLLECT_SUBPROJECT_DIRECTORY_NAMES varname filename)
  file(GLOB EDUNET_PLUGIN_CMAKE_FILES
    RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "*/${filename}")
  foreach(EDUNET_PLUGIN_CMAKE_FILE ${EDUNET_PLUGIN_CMAKE_FILES})
	get_filename_component(EDUNET_PLUGIN_DIR ${EDUNET_PLUGIN_CMAKE_FILE} PATH)
    set(${varname} ${${varname}} ${EDUNET_PLUGIN_DIR})
  endforeach(EDUNET_PLUGIN_CMAKE_FILE ${EDUNET_PLUGIN_CMAKE_FILES})
endmacro(EDUNET_COLLECT_SUBPROJECT_DIRECTORY_NAMES varname filename)

macro(ADD_SUBDIRECTORIES prefix header )
  message(STATUS "- ${header} -")
  foreach(subdir ${ARGN})
    message(STATUS "${prefix}${subdir}")
    add_subdirectory(${subdir})
  endforeach(subdir ${ARGN})
endmacro(ADD_SUBDIRECTORIES prefix header)
######################################################################

IF (NOT CMAKE_BUILD_TYPE)
# SET(CMAKE_BUILD_TYPE "Debug")
 SET(CMAKE_BUILD_TYPE "Release")
ENDIF (NOT CMAKE_BUILD_TYPE)

# string (REPLACE "/D_WINDOWS" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
remove_definitions(-D_WINDOWS )

IF(COMMAND cmake_policy)
   cmake_policy(SET CMP0003 NEW)
ENDIF(COMMAND cmake_policy)

set(ET_USE_MMGR_FLUID "FALSE" CACHE BOOL "Use the fluid memory manager?")
set(ET_USE_THIRDPARTYDIR "TRUE" CACHE BOOL "Search libs and include in thirdparty folder?")
IF (ET_USE_THIRDPARTYDIR)
	IF (NOT THIRD_PARTY_DIR)
		SET( THIRD_PARTY_DIR ${EDUNET_GAMES_SOURCE_DIR}/../ThirdParty )
	ENDIF (NOT THIRD_PARTY_DIR)
	SET(CMAKE_INCLUDE_PATH ${THIRD_PARTY_DIR}/include)
	SET(CMAKE_LIBRARY_PATH ${THIRD_PARTY_DIR}/lib)
ENDIF (ET_USE_THIRDPARTYDIR)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMakeModules/")

IF (WIN32)
	SET(ET_PLATFORM_BUILD_DIR build_win32)	
ELSE (WIN32)
	SET(ET_PLATFORM_BUILD_DIR build_x)		
ENDIF (WIN32)

# This is the shortcut to finding GLU, GLUT and OpenGL if they are properly installed on your system
# This should be the case.

FIND_PACKAGE(OpenGL)
IF (NOT OPENGL_FOUND)
MESSAGE("OPENGL NOT FOUND")
SET(OPENGL_gl_LIBRARY opengl32)
SET(OPENGL_glu_LIBRARY glu32)
ENDIF (NOT OPENGL_FOUND)

# ADD_DEFINITIONS(-DBT_USE_FREEGLUT)

FIND_PACKAGE(GLU)

FIND_PACKAGE(GLUT)
IF (NOT GLUT_FOUND)
	MESSAGE("GLUT NOT FOUND")
	Set( GLUT_INCLUDE_DIR ${THIRD_PARTY_DIR}/glut)
	IF (MSVC)
		MESSAGE ("GLUT NOT FOUND, trying to use Bullet/Glut/glut32.lib for MSVC")
		SET(GLUT_glut_LIBRARY ${EDUNET_GAMES_SOURCE_DIR}/bin/win32/lib/glut32.lib)
	ENDIF (MSVC)
ENDIF(NOT GLUT_FOUND)

#### BOOST
SET( Boost_DEBUG TRUE)
IF ( ET_USE_THIRDPARTYDIR )
	SET( BOOST_ROOT ${THIRD_PARTY_DIR} )
ENDIF( ET_USE_THIRDPARTYDIR )
FIND_PACKAGE( Boost2 COMPONENTS system filesystem )
IF( NOT Boost_FOUND )
	MESSAGE( "Boost NOT FOUND use ${BOOST_INCLUDEDIR} instead" )
ENDIF( NOT Boost_FOUND )

#### RakNet
IF ( ET_USE_THIRDPARTYDIR )
	SET( RAKNET_ROOT ${THIRD_PARTY_DIR} )
ENDIF( ET_USE_THIRDPARTYDIR )
FIND_PACKAGE(RakNet)
SET( RakNet_SOURCES ${THIRD_PARTY_DIR}/raknet )

IF ( ET_USE_THIRDPARTYDIR )
	SET( ARGTABLE_ROOT ${THIRD_PARTY_DIR} )
ENDIF( ET_USE_THIRDPARTYDIR )
FIND_PACKAGE(Argtable)
IF (NOT Argtable_FOUND)
	MESSAGE("Argtable NOT FOUND use ${Argtable_INCLUDES} instead")
ENDIF (NOT Argtable_FOUND)


IF (MINGW)
MESSAGE ("GLUT NOT FOUND not found, trying to use MINGW glut32")
SET(GLUT_glut_LIBRARY glut32)
ENDIF (MINGW)



INCLUDE_DIRECTORIES(${EDUNET_GAMES_SOURCE_DIR}/Source
${EDUNET_GAMES_SOURCE_DIR}/Source/src
${EDUNET_GAMES_SOURCE_DIR}/External
${GLUT_INCLUDE_DIR}
${Boost_INCLUDE_DIR}
${RakNet_INCLUDES}
${Argtable_INCLUDES}
 )

IF (WIN32)
  SET(EDUNET_NEEDED_WIN_LIBRARY ws2_32.lib)
ELSE (WIN32)
  SET(EDUNET_NEEDED_WIN_LIBRARY)
  INCLUDE_DIRECTORIES(/usr/include /usr/local/include )
ENDIF (WIN32)

SET(EDUNET_COMMON_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
SET(EDUNET_COMMON_LIB_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/libs)


Set( EduNet_DEFAULT_LIBRARIES
opensteer
${GLUT_glut_LIBRARY} 
${OPENGL_gl_LIBRARY} 
${OPENGL_glu_LIBRARY}
EduNetMemory
EduNetCore
EduNetCommon
EduNetConnect
EduNetModule
EduNetApplication
EduNetProfile
${EDUNET_NEEDED_WIN_LIBRARY}
GLUI
${Argtable_LIBRARY}
${Boost_LIBRARIES}
)

add_subdirectory(Source)
add_subdirectory(External)
