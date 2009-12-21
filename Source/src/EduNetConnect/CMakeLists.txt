project(EduNetConnect)

INCLUDE_DIRECTORIES( ${EDUNET_GAMES_SOURCE_DIR}/Source/src
 ${EDUNET_GAMES_SOURCE_DIR}/Source/opensteer/include
 ${RakNet_SOURCES}/DependentExtensions
 )

file (GLOB EduNetConnect_LIB_SRC "${EduNetConnect_SOURCE_DIR}/*.cpp")
file (GLOB EduNetConnect_LIB_Hdrs "${EduNetConnect_SOURCE_DIR}/*.h")

add_library(${PROJECT_NAME} STATIC
	${EduNetConnect_LIB_Hdrs} ${EduNetConnect_LIB_SRC})
set_target_properties( ${PROJECT_NAME} PROPERTIES
	ARCHIVE_OUTPUT_DIRECTORY ${EDUNET_COMMON_LIB_OUTPUT_DIRECTORY} )
