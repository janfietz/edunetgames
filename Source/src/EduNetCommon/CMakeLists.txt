project(EduNetCommon)
INCLUDE_DIRECTORIES( ${EDUNET_GAMES_SOURCE_DIR}/Source/src
 ${EDUNET_GAMES_SOURCE_DIR}/Source/opensteer/include
 ${RakNet_SOURCES}/DependentExtensions}
 )

file (GLOB EduNetCommon_LIB_SRC "${EduNetCommon_SOURCE_DIR}/*.cpp")
file (GLOB EduNetCommon_LIB_Hdrs "${EduNetCommon_SOURCE_DIR}/*.h")
file (GLOB OpenSteerUT_LIB_SRC "${EduNetCommon_SOURCE_DIR}/../OpenSteerUT/*.cpp")
file (GLOB OpenSteerUT_LIB_Hdrs "${EduNetCommon_SOURCE_DIR}/../OpenSteerUT/*.h")

#set compiler options to link the lib into a shared lib
IF (NOT WIN32)
   SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
ENDIF (NOT WIN32)

add_library(${PROJECT_NAME} STATIC
${EduNetCommon_LIB_SRC}
${EduNetCommon_LIB_Hdrs}
${OpenSteerUT_LIB_SRC}
${OpenSteerUT_LIB_Hdrs}
${RakNet_SOURCES}/DependentExtensions/RPC3/RPC3.cpp
)

set_target_properties( ${PROJECT_NAME} PROPERTIES
	ARCHIVE_OUTPUT_DIRECTORY ${EDUNET_COMMON_LIB_OUTPUT_DIRECTORY} )
