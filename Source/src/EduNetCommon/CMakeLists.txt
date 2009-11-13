project(EduNetCommon)
INCLUDE_DIRECTORIES( ${EDUNET_GAMES_SOURCE_DIR}/Source/src
 ${EDUNET_GAMES_SOURCE_DIR}/Source/opensteer/include
 ${EDUNET_GAMES_SOURCE_DIR}/../ThirdParty/RakNet-3.705/DependentExtensions }
 )
SET(EduNetCommon_LIB_SRC
EduNetGui.cpp
EduNetOptions.cpp
../OpenSteerUT/AbstractNetworkVehicle.cpp
../OpenSteerUT/AbstractVehicleGroup.cpp
../OpenSteerUT/AbstractVehicleUpdate.cpp
../OpenSteerUT/EmptyPlugin.cpp
../OpenSteerUT/OpenSteerStreams.cpp
../OpenSteerUT/PluginArray.cpp
../OpenSteerUT/SimpleNetworkVehicle.cpp
${EDUNET_GAMES_SOURCE_DIR}/../ThirdParty/RakNet-3.705/DependentExtensions/RPC3/RPC3.cpp
)

add_library(${PROJECT_NAME} STATIC ${EduNetCommon_LIB_SRC})