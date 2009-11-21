project(EduNetApplication)

INCLUDE_DIRECTORIES( ${EDUNET_GAMES_SOURCE_DIR}/Source/src
 ${EDUNET_GAMES_SOURCE_DIR}/Source/opensteer/include
 ${RakNet_SOURCES}/DependentExtensions
 )
SET(EduNetApplication_LIB_SRC
EduNetApplication.cpp
EduNetGames.cpp
EduNetMain.cpp
)

LINK_LIBRARIES(
 opensteer  
 ${GLUT_glut_LIBRARY}
 EduNetCommon
 glui
)

add_library(${PROJECT_NAME} STATIC ${EduNetApplication_LIB_SRC})
