project(EduNetApplication)

INCLUDE_DIRECTORIES( ${EDUNET_GAMES_SOURCE_DIR}/Source/src
 ${EDUNET_GAMES_SOURCE_DIR}/Source/opensteer/include
 ${EDUNET_GAMES_SOURCE_DIR}/../ThirdParty/RakNet-3.705/DependentExtensions
 )
SET(EduNetApplication_LIB_SRC
EduNetApplication.cpp
EduNetGames.cpp
EduNetMain.cpp
)

add_library(${PROJECT_NAME} STATIC ${EduNetApplication_LIB_SRC})
