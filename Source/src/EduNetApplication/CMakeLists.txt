project(EduNetApplication)

INCLUDE_DIRECTORIES( ${EDUNET_GAMES_SOURCE_DIR}/Source/src
 ${EDUNET_GAMES_SOURCE_DIR}/Source/opensteer/include
 ${EDUNET_GAMES_SOURCE_DIR}/../ThirdParty/RakNet-3.705/DependentExtensions
 )

file (GLOB EduNetApplication_LIB_SRC "${${PROJECT_NAME}_SOURCE_DIR}/*.cpp")
file (GLOB EduNetApplication_LIB_Hdrs "${${PROJECT_NAME}_SOURCE_DIR}/*.h")

LINK_LIBRARIES(
ExtOpensteer
ExtLinearMath
${GLUT_glut_LIBRARY}
EduNetMemory
EduNetCommon
EduNetConnect
EduNetModule
ExtIProf
EduNetProfile
ExtGLUI
${Argtable_LIBRARY}
)

#set compiler options to link the lib into a shared lib
IF (NOT WIN32)
   SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
ENDIF (NOT WIN32)

add_library(${PROJECT_NAME} STATIC
	${EduNetApplication_LIB_Hdrs} ${EduNetApplication_LIB_SRC})
set_target_properties( ${PROJECT_NAME} PROPERTIES
	ARCHIVE_OUTPUT_DIRECTORY ${EDUNET_COMMON_LIB_OUTPUT_DIRECTORY} )
	
set_target_properties( ${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX "_Debug" )
set_target_properties( ${PROJECT_NAME} PROPERTIES RELEASE_POSTFIX "_Release" )
set_target_properties( ${PROJECT_NAME} PROPERTIES MINSIZEREL_POSTFIX "_MinsizeRel" )
set_target_properties( ${PROJECT_NAME} PROPERTIES RELWITHDEBINFO_POSTFIX "_RelWithDebugInfo" )
	