project(EduNetCommon)
INCLUDE_DIRECTORIES( ${EDUNET_GAMES_SOURCE_DIR}/Source/src
 ${EDUNET_GAMES_SOURCE_DIR}/Source/opensteer/include
 ${RakNet_SOURCES}/DependentExtensions }
 )
SET(EduNetCommon_LIB_SRC
EduNetGui.cpp
EduNetOptions.cpp
../OpenSteerUT/AbstractVehicleGroup.cpp
../OpenSteerUT/AbstractVehicleUpdate.cpp
../OpenSteerUT/AbstractVehicleMath.cpp
../OpenSteerUT/AbstractVehicleMotionStatePlot.cpp
../OpenSteerUT/EmptyPlugin.cpp
../OpenSteerUT/OpenSteerStreams.cpp
../OpenSteerUT/PluginArray.cpp
../OpenSteerUT/CameraPlugin.cpp
../OpenSteerUT/GridPlugin.cpp
../OpenSteerUT/OpenSteerStreams.cpp
../OpenSteerUT/SimplePhysicsVehicle.cpp
${RakNet_SOURCES}/DependentExtensions/RPC3/RPC3.cpp
)

LINK_LIBRARIES(
 opensteer  

)

add_library(${PROJECT_NAME} STATIC ${EduNetCommon_LIB_SRC})
