project(Tutorial02)
INCLUDE_DIRECTORIES( 
${EDUNET_GAMES_SOURCE_DIR}/Source/opensteer/include

 ${RakNet_SOURCES}/DependentExtensions
)

aux_source_directory(${Tutorial02_SOURCE_DIR} Tutorial_Src)

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

add_executable(${PROJECT_NAME} ${Tutorial_Src})


