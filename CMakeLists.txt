cmake_minimum_required(VERSION 2.8)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)


PROJECT(EDUNET_GAMES)
SET(EDUNET_GAMES 0.05)
SET(CMake_VERSION_MAJOR 0)
SET(CMake_VERSION_MINOR 5)
SET(CMake_VERSION_PATCH 0)

ADD_DEFINITIONS( -DBOOST_ALL_DYN_LINK )
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMakeModules/")
include(ETMacros)

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
ELSE()
	SET(CMAKE_INCLUDE_PATH ${CMAKE_SOURCE_DIR}../include)
	SET(CMAKE_LIBRARY_PATH ${CMAKE_SOURCE_DIR}../lib)
ENDIF (ET_USE_THIRDPARTYDIR)


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

#### RakNet
FIND_PACKAGE(RakNet)
set(ET_RakNet_SOURCES ${ET_THIRD_PARTY_DIR}/raknet CACHE PATH "Path to raknet sources" )

#### BOOST
set(Boost_ADDITIONAL_VERSIONS "1.47" "1.47.0")

set(Boost_USE_STATIC_LIBS        OFF)
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME    OFF)

IF ( ET_USE_THIRDPARTYDIR )
	SET( BOOST_ROOT ${ET_THIRD_PARTY_DIR} )
ENDIF( ET_USE_THIRDPARTYDIR )

FIND_PACKAGE( Boost COMPONENTS system filesystem program_options )


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
 )

# -----------------------------------------------------------------------------
IF (WIN32)
  SET(EDUNET_NEEDED_WIN_LIBRARY ws2_32.lib)
ELSE (WIN32)
  SET(EDUNET_NEEDED_WIN_LIBRARY)
  INCLUDE_DIRECTORIES(/usr/include /usr/local/include )
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
)

# -----------------------------------------------------------------------------
add_subdirectory(Source)
add_subdirectory(External)

# -----------------------------------------------------------------------------
IF (WIN32)
	# -----------------------------------------------------------------------------
	# collect needed shared libs and copy them to output directories

	FILE (GLOB ET_SHARED_THIRDPARTY_LIBS "${ET_THIRD_PARTY_DIR}lib/*.dll")
	#OPENGL STUFF
	FILE (GLOB ET_SHARED_OPENGL_DLLS "${EDUNET_GAMES_SOURCE_DIR}/bin/win32/*32.dll")

	#copy needed dlls to debug and release kind of hack now
	COPYFILES_TO("Copy thirdparty DLL"  ${EDUNET_COMMON_OUTPUT_DIRECTORY} ${ET_SHARED_THIRDPARTY_LIBS})
	COPYFILES_TO("Copy OpenGl DLL" ${EDUNET_COMMON_OUTPUT_DIRECTORY} ${ET_SHARED_OPENGL_DLLS} )	
	
ENDIF (WIN32)

INSTALL(FILES ${ET_SHARED_THIRDPARTY_LIBS} DESTINATION bin)
INSTALL(FILES ${ET_SHARED_OPENGL_DLLS} DESTINATION bin)

# -----------------------------------------------------------------------------
# create a packege

SET(CPACK_GENERATOR "ZIP")
SET(CPACK_PACKAGE_NAME "${PROJECT_NAME}")

SET(CPACK_PACKAGE_VERSION_MAJOR ${CMake_VERSION_MAJOR})
SET(CPACK_PACKAGE_VERSION_MINOR ${CMake_VERSION_MINOR})
SET(CPACK_PACKAGE_VERSION_PATCH ${CMake_VERSION_PATCH})
SET(CPACK_SOURCE_GENERATOR "ZIP")
SET(CPACK_SOURCE_PACKAGE_FILE_NAME "${PROJECT_NAME}_SRC")
#SET(CPACK_SOURCE_STRIP_FILES "")

INCLUDE(CPack)
