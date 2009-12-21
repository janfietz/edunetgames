project(EduNetApplication)

INCLUDE_DIRECTORIES( ${EDUNET_GAMES_SOURCE_DIR}/Source/src
 ${EDUNET_GAMES_SOURCE_DIR}/Source/opensteer/include
 ${EDUNET_GAMES_SOURCE_DIR}/../ThirdParty/RakNet-3.705/DependentExtensions
 )

file (GLOB EduNetApplication_LIB_SRC "${EduNetApplication_SOURCE_DIR}/*.cpp")
file (GLOB EduNetApplication_LIB_Hdrs "${EduNetApplication_SOURCE_DIR}/*.h")

LINK_LIBRARIES(
 opensteer
 ${GLUT_glut_LIBRARY}
 EduNetCommon
EduNetConnect
IProf
EduNetProfile
${RakNet_LIBRARY}
 GLUI
${Argtable_LIBRARY}
)

add_library(${PROJECT_NAME} STATIC 
	${EduNetApplication_LIB_Hdrs} ${EduNetApplication_LIB_SRC})
set_target_properties( ${PROJECT_NAME} PROPERTIES
	ARCHIVE_OUTPUT_DIRECTORY ${EDUNET_COMMON_LIB_OUTPUT_DIRECTORY} )