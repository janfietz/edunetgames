#project(opensteer)
INCLUDE_DIRECTORIES( ${EDUNET_GAMES_SOURCE_DIR}/Source/opensteer/include )

SET(OPENTSTEER_LIB_SRC
 Obstacle.cpp
 OldPathway.cpp
 Path.cpp
 Plugin.cpp
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
 Draw.cpp
 lq.c
 Clock.cpp
 Camera.cpp
 Pathway.cpp
 Entity.cpp
SimplePlayer.cpp
)

SET(OPENTSTEER_LIB_HDRS
../include/OpenSteer/AbstractVehicle.h
 ../include/OpenSteer/Annotation.h
 ../include/OpenSteer/Camera.h
 ../include/OpenSteer/Clock.h
 ../include/OpenSteer/Color.h
 ../include/OpenSteer/Draw.h
 ../include/OpenSteer/LocalSpace.h
 ../include/OpenSteer/lq.h
 ../include/OpenSteer/Obstacle.h
 ../include/OpenSteer/OldPathway.h
 ../include/OpenSteer/Path.h
 ../include/OpenSteer/Pathway.h
 ../include/OpenSteer/PlugIn.h
 ../include/OpenSteer/PolylineSegmentedPath.h
 ../include/OpenSteer/PolylineSegmentedPathwaySegmentRadii.h
 ../include/OpenSteer/PolylineSegmentedPathwaySingleRadius.h
 ../include/OpenSteer/Proximity.h
 ../include/OpenSteer/QueryPathAlike.h
 ../include/OpenSteer/QueryPathAlikeBaseDataExtractionPolicies.h
 ../include/OpenSteer/QueryPathAlikeMappings.h
 ../include/OpenSteer/QueryPathAlikeUtilities.h
 ../include/OpenSteer/SegmentedPath.h
 ../include/OpenSteer/SegmentedPathAlikeUtilities.h
 ../include/OpenSteer/SegmentedPathway.h
 ../include/OpenSteer/SharedPointer.h
 ../include/OpenSteer/SimpleVehicle.h
 ../include/OpenSteer/StandardTypes.h
 ../include/OpenSteer/SteerLibrary.h
 ../include/OpenSteer/UnusedParameter.h
 ../include/OpenSteer/Utilities.h
 ../include/OpenSteer/Vec3.h
 ../include/OpenSteer/Vec3Utilities.h
../include/OpenSteer/SimplePlayer.h
)

#SET(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)

add_library(opensteer STATIC ${OPENTSTEER_LIB_HDRS} ${OPENTSTEER_LIB_SRC})
set_target_properties( opensteer PROPERTIES
	ARCHIVE_OUTPUT_DIRECTORY ${EDUNET_COMMON_LIB_OUTPUT_DIRECTORY} )

# APPLE
if ( WIN32 )
   message(STATUS "${CMAKE_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_BINARY_DIR} ${CMAKE_CURRENT_BINARY_DIR}")
   message(STATUS "${PROJECT_NAME} builds to ${LIBRARY_OUTPUT_PATH} ${CMAKE_CFG_INTDIR}")
else(UNIX)
    MESSAGE(STATUS "Not created")
endif()

