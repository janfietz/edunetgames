cmake_minimum_required(VERSION 2.8)

# -----------------------------------------------------------------------------
# this enables project hirarchies
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

PROJECT(EDUNET_GAMES)
SET(EDUNET_GAMES 0.01)

# -----------------------------------------------------------------------------
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

# -----------------------------------------------------------------------------
IF (NOT CMAKE_BUILD_TYPE)
# SET(CMAKE_BUILD_TYPE "Debug")
 SET(CMAKE_BUILD_TYPE "Release")
ENDIF (NOT CMAKE_BUILD_TYPE)

# -----------------------------------------------------------------------------
IF (WIN32)
	OPTION(ET_CREATE_DISTRIBUTABLE_MSVC_PROJECTFILES "Create MSVC projectfiles that can be distributed" OFF)
ENDIF (WIN32)

# -----------------------------------------------------------------------------
IF (ET_CREATE_DISTRIBUTABLE_MSVC_PROJECTFILES)
	SET(EDUNET_COMMON_OUTPUT_DIRECTORY ${EDUNET_GAMES_SOURCE_DIR}/bin)
	SET(EDUNET_COMMON_LIB_OUTPUT_DIRECTORY ${EDUNET_GAMES_SOURCE_DIR}/lib)
	SET(CMAKE_SUPPRESS_REGENERATION  1)
	SET(CMAKE_USE_RELATIVE_PATHS 1)
ELSE()
	SET(EDUNET_COMMON_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
	SET(EDUNET_COMMON_LIB_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
ENDIF()

SET (LIBRARY_OUTPUT_PATH ${EDUNET_COMMON_LIB_OUTPUT_DIRECTORY} CACHE PATH "Single output directory for building all libraries.")

# -----------------------------------------------------------------------------
SET( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${EDUNET_COMMON_LIB_OUTPUT_DIRECTORY} )
SET( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${EDUNET_COMMON_LIB_OUTPUT_DIRECTORY} )
SET( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${EDUNET_COMMON_LIB_OUTPUT_DIRECTORY} )
SET( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_MINSIZEREL ${EDUNET_COMMON_LIB_OUTPUT_DIRECTORY} )
SET( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELWITHDEBINFO ${EDUNET_COMMON_LIB_OUTPUT_DIRECTORY} )

# -----------------------------------------------------------------------------
SET( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${EDUNET_COMMON_LIB_OUTPUT_DIRECTORY} )
SET( CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${EDUNET_COMMON_LIB_OUTPUT_DIRECTORY} )
SET( CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${EDUNET_COMMON_LIB_OUTPUT_DIRECTORY} )
SET( CMAKE_LIBRARY_OUTPUT_DIRECTORY_MINSIZEREL ${EDUNET_COMMON_LIB_OUTPUT_DIRECTORY} )
SET( CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELWITHDEBINFO ${EDUNET_COMMON_LIB_OUTPUT_DIRECTORY} )

# -----------------------------------------------------------------------------
SET( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${EDUNET_COMMON_OUTPUT_DIRECTORY} )
SET( CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${EDUNET_COMMON_OUTPUT_DIRECTORY} )
SET( CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${EDUNET_COMMON_OUTPUT_DIRECTORY} )
SET( CMAKE_RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL ${EDUNET_COMMON_OUTPUT_DIRECTORY} )
SET( CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${EDUNET_COMMON_OUTPUT_DIRECTORY})

# -----------------------------------------------------------------------------
# string (REPLACE "/D_WINDOWS" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
remove_definitions(-D_WINDOWS )

IF(COMMAND cmake_policy)
   cmake_policy(SET CMP0003 NEW)
ENDIF(COMMAND cmake_policy)

set(ET_USE_MMGR_FLUID "FALSE" CACHE BOOL "Use the fluid memory manager?")
set(ET_USE_THIRDPARTYDIR "TRUE" CACHE BOOL "Search libs and include in thirdparty folder?")
set(ET_THIRD_PARTY_DIR ${EDUNET_GAMES_SOURCE_DIR}/../ThirdParty  CACHE PATH "Path to thirdparty includes and libs" )
IF (ET_USE_THIRDPARTYDIR)
	SET(CMAKE_INCLUDE_PATH ${ET_THIRD_PARTY_DIR}/include)
	SET(CMAKE_LIBRARY_PATH ${ET_THIRD_PARTY_DIR}/lib)
ENDIF (ET_USE_THIRDPARTYDIR)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMakeModules/")

IF (WIN32)
	SET(ET_PLATFORM_BUILD_DIR buildwin32)	
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

# -----------------------------------------------------------------------------
# GLUT
FIND_PACKAGE(GLUT)
IF (NOT GLUT_FOUND)
	MESSAGE("GLUT NOT FOUND")
	Set( GLUT_INCLUDE_DIR ${ET_THIRD_PARTY_DIR}/glut)
	IF (MSVC)
		MESSAGE ("GLUT NOT FOUND, trying to use Bullet/Glut/glut32.lib for MSVC")
		SET(GLUT_glut_LIBRARY ${EDUNET_GAMES_SOURCE_DIR}/bin/win32/lib/glut32.lib)
	ENDIF (MSVC)
ENDIF(NOT GLUT_FOUND)

# -----------------------------------------------------------------------------
# BOOST
IF ( ET_USE_THIRDPARTYDIR )
	SET( BOOST_ROOT ${ET_THIRD_PARTY_DIR} )
ENDIF( ET_USE_THIRDPARTYDIR )
FIND_PACKAGE( Boost2 COMPONENTS system filesystem )
IF( NOT Boost_FOUND )
	MESSAGE( "Boost NOT FOUND use ${BOOST_INCLUDEDIR} instead" )
ENDIF( NOT Boost_FOUND )

# -----------------------------------------------------------------------------
# RakNet
FIND_PACKAGE(RakNet)
set(ET_RakNet_SOURCES ${ET_THIRD_PARTY_DIR}/raknet CACHE PATH "Path to raknet sources" )

# -----------------------------------------------------------------------------
# Here you can define what libraries of wxWidgets you need for your
# application. You can figure out what libraries you need here;
# http://www.wxwidgets.org/manuals/2.8/wx_librarieslist.html

# -----------------------------------------------------------------------------
# why should we use this macro?
# SET(wxWidgets_USE_DEBUG, TRUE)
FIND_PACKAGE(wxWidgets REQUIRED aui net gl core base)
IF(wxWidgets_FOUND)
	# wxWidgets include (this will do all the magic to configure everything)
	INCLUDE( ${wxWidgets_USE_FILE} )
ELSE(wxWidgets_FOUND)
	# For convenience. When we cannot continue, inform the user
	MESSAGE("wxWidgets not found!")
ENDIF(wxWidgets_FOUND)

# -----------------------------------------------------------------------------
IF (MINGW)
MESSAGE ("GLUT NOT FOUND not found, trying to use MINGW glut32")
SET(GLUT_glut_LIBRARY glut32)
ENDIF (MINGW)

# -----------------------------------------------------------------------------
INCLUDE_DIRECTORIES(${EDUNET_GAMES_SOURCE_DIR}/Source
${EDUNET_GAMES_SOURCE_DIR}/Source/src
${EDUNET_GAMES_SOURCE_DIR}/External
${GLUT_INCLUDE_DIR}
${Boost_INCLUDE_DIR}
${RakNet_INCLUDES}
${wxWidgets_INCLUDES}
 )

# -----------------------------------------------------------------------------
IF (WIN32)
  SET(EDUNET_NEEDED_WIN_LIBRARY ws2_32.lib)
ELSE (WIN32)
  SET(EDUNET_NEEDED_WIN_LIBRARY)
  INCLUDE_DIRECTORIES(/usr/include /usr/local/include )
ENDIF (WIN32)

# -----------------------------------------------------------------------------
# collect needed shared libs and copy them to output directories
IF (WIN32)
  FILE (GLOB ET_SHARED_THIRDPARTY_LIBS "${ET_THIRD_PARTY_DIR}/lib/*.dll")
  #OPENGL STUFF
  FILE (GLOB ET_SHARED_OPENGL_DLLS "${EDUNET_GAMES_SOURCE_DIR}/bin/win32/*32.dll")
ELSE (WIN32)

ENDIF (WIN32)

# -----------------------------------------------------------------------------
# collect all needed default libraries
Set( EduNet_DEFAULT_LIBRARIES
ExtOpensteer
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
ExtGLUI
ExtLinearMath
${Boost_LIBRARIES}
)

# -----------------------------------------------------------------------------
# collect all needed executable libraries
Set( EduNet_PLAYER_LIBRARIES
ExtOpensteer
${GLUT_glut_LIBRARY} 
${OPENGL_gl_LIBRARY} 
${OPENGL_glu_LIBRARY}
EduNetMemory
EduNetCore
EduNetCommon
EduNetModule
EduNetApplication
EduNetProfile
${EDUNET_NEEDED_WIN_LIBRARY}
ExtGLUI
ExtLinearMath
${Boost_LIBRARIES}
${wxWidgets_LIBRARIES}
)


# -----------------------------------------------------------------------------
# collect all needed module libraries
Set( EduNet_MODULE_LIBRARIES
ExtOpensteer
${GLUT_glut_LIBRARY} 
${OPENGL_gl_LIBRARY} 
${OPENGL_glu_LIBRARY}
EduNetMemory
EduNetCore
EduNetCommon
EduNetConnect
EduNetModule
EduNetProfile
${EDUNET_NEEDED_WIN_LIBRARY}
ExtGLUI
ExtLinearMath
${Boost_LIBRARIES}
${wxWidgets_LIBRARIES}
)

# -----------------------------------------------------------------------------
add_subdirectory(Source)
add_subdirectory(External)

# -----------------------------------------------------------------------------
IF (WIN32)
	#copy needed dlls to debug and release kind of hack now
	INSTALL(FILES ${ET_SHARED_THIRDPARTY_LIBS} DESTINATION ${EDUNET_COMMON_OUTPUT_DIRECTORY})
	INSTALL(FILES ${ET_SHARED_OPENGL_DLLS} DESTINATION ${EDUNET_COMMON_OUTPUT_DIRECTORY})	
ENDIF (WIN32)