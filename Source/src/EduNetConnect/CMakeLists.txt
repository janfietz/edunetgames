project(EduNetConnect)

INCLUDE_DIRECTORIES( ${EDUNET_GAMES_SOURCE_DIR}/Source/src
 ${EDUNET_GAMES_SOURCE_DIR}/Source/opensteer/include
 ${EDUNET_GAMES_SOURCE_DIR}/../ThirdParty/RakNet-3.705/DependentExtensions
 )
SET(EduNetConnect_LIB_SRC
NetworkPlugin.cpp
PeerPlugin.cpp
ClientPlugin.cpp
OSReplicaConnection.cpp
)

add_library(${PROJECT_NAME} STATIC ${EduNetConnect_LIB_SRC})
