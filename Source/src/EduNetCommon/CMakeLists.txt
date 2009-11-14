project(EduNetCommon)
INCLUDE_DIRECTORIES( ${EDUNET_GAMES_SOURCE_DIR}/Source/src } )
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
)

add_library(${PROJECT_NAME} STATIC ${EduNetCommon_LIB_SRC})