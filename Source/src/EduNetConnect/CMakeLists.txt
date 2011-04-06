project(EduNetConnect)

INCLUDE_DIRECTORIES( ${EDUNET_GAMES_SOURCE_DIR}/Source/src
 ${EDUNET_GAMES_SOURCE_DIR}/Source/opensteer/include
 ${ET_RakNet_SOURCES}/DependentExtensions
 )

file (GLOB EduNetConnect_LIB_SRC "${${PROJECT_NAME}_SOURCE_DIR}/*.cpp")
file (GLOB EduNetConnect_LIB_Hdrs "${${PROJECT_NAME}_SOURCE_DIR}/*.h")

include( ${wxWidgets_USE_FILE} )

#set compiler options to link the lib into a shared lib
IF (NOT WIN32)
   SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
ENDIF (NOT WIN32)

add_library(${PROJECT_NAME} STATIC
	${EduNetConnect_LIB_Hdrs} ${EduNetConnect_LIB_SRC})

source_group( source FILES ${EduNetConnect_LIB_Hdrs} ${EduNetConnect_LIB_SRC} )

set_property( TARGET ${PROJECT_NAME} PROPERTY FOLDER "edunet/libraries" )

set_target_properties( ${PROJECT_NAME} PROPERTIES
	ARCHIVE_OUTPUT_DIRECTORY ${EDUNET_COMMON_LIB_OUTPUT_DIRECTORY} )
	
set_target_properties( ${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX "_Debug" )
set_target_properties( ${PROJECT_NAME} PROPERTIES RELEASE_POSTFIX "_Release" )
set_target_properties( ${PROJECT_NAME} PROPERTIES MINSIZEREL_POSTFIX "_MinsizeRel" )
set_target_properties( ${PROJECT_NAME} PROPERTIES RELWITHDEBINFO_POSTFIX "_RelWithDebugInfo" )
