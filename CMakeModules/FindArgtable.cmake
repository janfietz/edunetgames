# - Try to find Argtable
# Once done this will define
#
#  Argtable_FOUND - system has Argtable
#  Argtable_INCLUDES - the Argtable include directory
#  Argtable_LIBRARY - Link these to use Argtable

FIND_LIBRARY (Argtable_LIBRARY_RELEASE NAMES argtable2
    PATHS
    ENV LD_LIBRARY_PATH
    ENV LIBRARY_PATH
    /usr/lib64
    /usr/lib
    /usr/local/lib64
    /usr/local/lib
    /opt/local/lib
	 ${ARGTABLE_ROOT}/lib
    )
	
FIND_LIBRARY (Argtable_LIBRARY_DEBUG NAMES argtable2d
    PATHS
    ENV LD_LIBRARY_PATH
    ENV LIBRARY_PATH
    /usr/lib64
    /usr/lib
    /usr/local/lib64
    /usr/local/lib
    /opt/local/lib
	 ${ARGTABLE_ROOT}/lib
    )
	
FIND_PATH (Argtable_INCLUDES argtable2.h
    ENV CPATH
	${ARGTABLE_ROOT}/include
    /usr/include
    /usr/local/include
    /opt/local/include
    )

IF(Argtable_INCLUDES AND Argtable_LIBRARY_RELEASE)
    SET(Argtable_FOUND TRUE)
ENDIF(Argtable_INCLUDES AND Argtable_LIBRARY_RELEASE)

IF(Argtable_FOUND)
  IF(NOT Argtable_FIND_QUIETLY)
    MESSAGE(STATUS "Found Argtable: ${Argtable_LIBRARIES}")
  ENDIF(NOT Argtable_FIND_QUIETLY)
	IF (CMAKE_CONFIGURATION_TYPES OR CMAKE_BUILD_TYPE)
        SET(Argtable_LIBRARY optimized ${Argtable_LIBRARY_RELEASE} debug ${Argtable_LIBRARY_DEBUG})
	ELSE()
		# if there are no configuration types and CMAKE_BUILD_TYPE has no value
		# then just use the release libraries
		SET(Argtable_LIBRARY ${Argtable_LIBRARY_RELEASE} )
	ENDIF()
ELSE(Argtable_FOUND)
  IF(Argtable_FIND_REQUIRED)
    MESSAGE(FATAL_ERROR "Could not find Argtable")
  ENDIF(Argtable_FIND_REQUIRED)
ENDIF(Argtable_FOUND)

