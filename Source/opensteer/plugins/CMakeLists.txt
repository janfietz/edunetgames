project(OpenSteerDemo)
INCLUDE_DIRECTORIES( ${EDUNET_GAMES_SOURCE_DIR}/Source/opensteer/include } )
SET(opensteerDemoSrcs
Soccer.cpp
PedestriansWalkingAnEight.cpp
Pedestrian.cpp
OneTurning.cpp
MultiplePursuit.cpp
MapDrive.cpp
LowSpeedTurn.cpp
CaptureTheFlag.cpp
Boids.cpp
../src/main.cpp
../src/OpenSteerDemo.cpp
)

IF(WIN32)
	SET(opensteerDemoSrcs ${opensteerDemoSrcs}
	../resources/resource.h
	../resources/SteerTest.rc
	../resources/OpenSteerDemo.ico)
ENDIF(WIN32)

if ( WIN32 )
	
	#SET(GLUT_glut_LIBRARY ${CMAKE_BINARY_DIR}/bin)
	#SET(GLUT_LIBRARIES glut32.lib)
else(UNIX)
     
endif()

SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)


LINK_LIBRARIES(
 opensteer  ${GLUT_glut_LIBRARY}
)

add_executable(${PROJECT_NAME} ${opensteerDemoSrcs})


