project(EduNetCore)

INCLUDE_DIRECTORIES( ${EDUNET_GAMES_SOURCE_DIR}/Source/src
 )

file (GLOB EduNetCore_LIB_SRC "${EduNetCore_SOURCE_DIR}/*.cpp")
file (GLOB EduNetCore_LIB_Hdrs "${EduNetCore_SOURCE_DIR}/*.h")

#set compiler options to link the lib into a shared lib
IF (NOT WIN32)
   SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
ENDIF (NOT WIN32)

add_library(${PROJECT_NAME} STATIC
${EduNetCore_LIB_SRC}
${EduNetCore_LIB_Hdrs}
)

set_target_properties( ${PROJECT_NAME} PROPERTIES
	ARCHIVE_OUTPUT_DIRECTORY ${EDUNET_COMMON_LIB_OUTPUT_DIRECTORY} )
