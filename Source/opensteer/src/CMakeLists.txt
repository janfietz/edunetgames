project(ExtOpensteer)

INCLUDE_DIRECTORIES(${EDUNET_GAMES_SOURCE_DIR}/Source/opensteer/include
)

SET(OPENTSTEER_LIB_SRC
 AABBox.cpp
 Annotation.cpp
 GlobalData.cpp
 GlobalSelection.cpp
 Obstacle.cpp
 OldPathway.cpp
 Path.cpp
 Plugin.cpp
 PluginRegistry.cpp
 PolylineSegmentedPath.cpp
 PolylineSegmentedPathwaySegmentRadii.cpp
 PolylineSegmentedPathwaySingleRadius.cpp
 SegmentedPath.cpp
 SegmentedPathway.cpp
 SimpleVehicle.cpp
 TerrainRayTest.cpp
 Vec3.cpp
 Vec3Utilities.cpp
 Color.cpp
 lq.c
 Clock.cpp
 Camera.cpp
 Pathway.cpp
 Entity.cpp
 SimplePlayer.cpp
 Renderer.cpp
)

SET(OPENTSTEER_LIB_HDRS
 ../include/OpenSteer/AABBox.h
 ../include/OpenSteer/AbstractEntity.h
 ../include/OpenSteer/AbstractLocalSpace.h
 ../include/OpenSteer/AbstractPlayer.h
 ../include/OpenSteer/AbstractPlugin.h
 ../include/OpenSteer/AbstractRenderer.h
 ../include/OpenSteer/AbstractState.h
 ../include/OpenSteer/AbstractUpdated.h
 ../include/OpenSteer/AbstractVehicle.h
 ../include/OpenSteer/Annotation.h
 ../include/OpenSteer/Camera.h
 ../include/OpenSteer/Clock.h
 ../include/OpenSteer/Color.h
 ../include/OpenSteer/Draw.h
 ../include/OpenSteer/Entity.h
 ../include/OpenSteer/GlobalData.h
 ../include/OpenSteer/GlobalSelection.h
 ../include/OpenSteer/InstanceTracker.h
 ../include/OpenSteer/LocalSpace.h
 ../include/OpenSteer/lq.h
 ../include/OpenSteer/Obstacle.h
 ../include/OpenSteer/OldPathway.h
 ../include/OpenSteer/OpenSteerMacros.h
 ../include/OpenSteer/OpenSteerTypes.h
 ../include/OpenSteer/Path.h
 ../include/OpenSteer/Pathway.h
 ../include/OpenSteer/Plugin.h
 ../include/OpenSteer/PluginRegistry.h
 ../include/OpenSteer/PolylineSegmentedPath.h
 ../include/OpenSteer/PolylineSegmentedPathwaySegmentRadii.h
 ../include/OpenSteer/PolylineSegmentedPathwaySingleRadius.h
 ../include/OpenSteer/Proximity.h
 ../include/OpenSteer/QueryPathAlike.h
 ../include/OpenSteer/QueryPathAlikeBaseDataExtractionPolicies.h
 ../include/OpenSteer/QueryPathAlikeMappings.h
 ../include/OpenSteer/QueryPathAlikeUtilities.h
 ../include/OpenSteer/Renderer.h
 ../include/OpenSteer/SegmentedPath.h
 ../include/OpenSteer/SegmentedPathAlikeUtilities.h
 ../include/OpenSteer/SegmentedPathway.h
 ../include/OpenSteer/SharedPointer.h
 ../include/OpenSteer/SimplePlayer.h
 ../include/OpenSteer/SimpleVehicle.h
 ../include/OpenSteer/StandardTypes.h
 ../include/OpenSteer/SteerLibrary.h
 ../include/OpenSteer/UnusedParameter.h
 ../include/OpenSteer/Utilities.h
 ../include/OpenSteer/Vec3.h
 ../include/OpenSteer/Vec3Utilities.h
 )

#set compiler options to link the lib into a shared lib
IF (NOT WIN32)
   SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
   SET(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
ENDIF (NOT WIN32)

add_library(${PROJECT_NAME} STATIC ${OPENTSTEER_LIB_HDRS} ${OPENTSTEER_LIB_SRC})
set_target_properties( ${PROJECT_NAME} PROPERTIES
	ARCHIVE_OUTPUT_DIRECTORY ${EDUNET_COMMON_LIB_OUTPUT_DIRECTORY} )

set_target_properties( ${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX "_Debug" )
set_target_properties( ${PROJECT_NAME} PROPERTIES RELEASE_POSTFIX "_Release" )
set_target_properties( ${PROJECT_NAME} PROPERTIES MINSIZEREL_POSTFIX "_MinsizeRel" )
set_target_properties( ${PROJECT_NAME} PROPERTIES RELWITHDEBINFO_POSTFIX "_RelWithDebugInfo" )
	
	
# APPLE
if ( WIN32 )
   message(STATUS "${CMAKE_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_BINARY_DIR} ${CMAKE_CURRENT_BINARY_DIR}")
   message(STATUS "${PROJECT_NAME} builds to ${LIBRARY_OUTPUT_PATH} ${CMAKE_CFG_INTDIR}")
else(UNIX)

endif()

