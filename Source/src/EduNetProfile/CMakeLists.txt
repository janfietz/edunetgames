project(EduNetProfile)

INCLUDE_DIRECTORIES( ${EDUNET_GAMES_SOURCE_DIR}/Source/src
 ${EDUNET_GAMES_SOURCE_DIR}/Source/opensteer/include
 ${RakNet_SOURCES}/DependentExtensions
 )

file (GLOB EDUNETPROFILE_SOURCES "${EduNetProfile_SOURCE_DIR}/*.cpp")
file (GLOB EDUNETPROFILE_HEADERS "${EduNetProfile_SOURCE_DIR}/*.h")

#set compiler options to link the lib into a shared lib
IF (NOT WIN32)
   SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
ENDIF (NOT WIN32)

ADD_LIBRARY(EduNetProfile STATIC  ${EDUNETPROFILE_SOURCES})
set_target_properties( ${PROJECT_NAME} PROPERTIES
	ARCHIVE_OUTPUT_DIRECTORY ${EDUNET_COMMON_LIB_OUTPUT_DIRECTORY} )

