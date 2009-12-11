project(EduNetApplication)

INCLUDE_DIRECTORIES( ${EDUNET_GAMES_SOURCE_DIR}/Source/src
 ${EDUNET_GAMES_SOURCE_DIR}/Source/opensteer/include
 ${EDUNET_GAMES_SOURCE_DIR}/../ThirdParty/RakNet-3.705/DependentExtensions
 )

aux_source_directory(${EduNetApplication_SOURCE_DIR} EduNetApplication_LIB_SRC)


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

add_library(${PROJECT_NAME} STATIC ${EduNetApplication_LIB_SRC})
