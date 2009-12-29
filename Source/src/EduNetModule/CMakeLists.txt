project(EduNetModule)

INCLUDE_DIRECTORIES( ${EDUNET_GAMES_SOURCE_DIR}/Source/src
 ${EDUNET_GAMES_SOURCE_DIR}/Source/opensteer/include
 ${EDUNET_GAMES_SOURCE_DIR}/../ThirdParty/RakNet-3.705/DependentExtensions
 )

file (GLOB EduNetModule_LIB_SRC "${EduNetModule_SOURCE_DIR}/*.cpp")
file (GLOB EduNetModule_LIB_Hdrs "${EduNetModule_SOURCE_DIR}/*.h")

#set compiler options to link the lib into a shared lib
IF (NOT WIN32)
   SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
ENDIF (NOT WIN32)

add_library(${PROJECT_NAME} STATIC
	${EduNetModule_LIB_Hdrs} ${EduNetModule_LIB_SRC})
set_target_properties( ${PROJECT_NAME} PROPERTIES
	ARCHIVE_OUTPUT_DIRECTORY ${EDUNET_COMMON_LIB_OUTPUT_DIRECTORY} )