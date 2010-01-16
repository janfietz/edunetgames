project(EduNetMemory)

INCLUDE_DIRECTORIES( ${EDUNET_GAMES_SOURCE_DIR}/Source/src )

SET( EduNetMemoryFuildSrc
Fluid/mmgr.cpp
)
SET( EduNetMemoryFuildHdr
Fluid/mmgr.h
Fluid/nommgr.h
)

SET( EduNetMemorySrc
EduNetMemory.cpp
)
SET( EduNetMemoryHdr
EduNetMemory.h
)

#set compiler options to link the lib into a shared lib
IF (NOT WIN32)
   SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
ENDIF (NOT WIN32)

add_library(${PROJECT_NAME} STATIC
	${EduNetMemoryHdr} ${EduNetMemorySrc} ${EduNetMemoryFuildSrc} ${EduNetMemoryFuildHdr})

set_target_properties( ${PROJECT_NAME} PROPERTIES
	ARCHIVE_OUTPUT_DIRECTORY ${EDUNET_COMMON_LIB_OUTPUT_DIRECTORY} )
