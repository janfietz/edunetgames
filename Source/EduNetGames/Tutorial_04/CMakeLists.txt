project(Tutorial04)
INCLUDE_DIRECTORIES( 
${EDUNET_GAMES_SOURCE_DIR}/Source/opensteer/include

 ${RakNet_SOURCES}/DependentExtensions
)

SET(Tutorial04_Srcs
main.cpp
NetPedestrian.cpp
NetPedestrianFactory.cpp
NetPedestrianPlugin.cpp
NetPedestrianPlugins.cpp
NetPedestrianReplica.cpp
NetPedestrianReplicaConnection.cpp
)


LINK_LIBRARIES(
 opensteer  
 ${GLUT_glut_LIBRARY}
 EduNetCommon
EduNetConnect
EduNetApplication
EduNetProfile
${RakNet_LIBRARY}
 GLUI
${Argtable_LIBRARY}
)

SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)

add_executable(${PROJECT_NAME} ${Tutorial04_Srcs})


