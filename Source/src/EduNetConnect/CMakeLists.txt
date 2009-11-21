project(EduNetConnect)

INCLUDE_DIRECTORIES( ${EDUNET_GAMES_SOURCE_DIR}/Source/src
 ${EDUNET_GAMES_SOURCE_DIR}/Source/opensteer/include
 ${RakNet_SOURCES}/DependentExtensions
 )
SET(EduNetConnect_LIB_SRC
NetworkPlugin.cpp
PeerPlugin.cpp
ClientPlugin.cpp
AbstractNetworkVehicle.cpp
AbstractReplica.cpp
AbstractVehicleReplica.cpp
AbstractVehicleReplicaConnection.cpp
ClientServerPlugin.cpp
NetworkPlot.cpp
NetworkVehicle.cpp
SimpleNetworkVehicle.cpp
)

add_library(${PROJECT_NAME} STATIC ${EduNetConnect_LIB_SRC})
