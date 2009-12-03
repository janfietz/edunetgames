project(EduNetConnect)

INCLUDE_DIRECTORIES( ${EDUNET_GAMES_SOURCE_DIR}/Source/src
 ${EDUNET_GAMES_SOURCE_DIR}/Source/opensteer/include
 ${RakNet_SOURCES}/DependentExtensions
 )

aux_source_directory(${EduNetConnect_SOURCE_DIR} EduNetConnect_LIB_SRC)

add_library(${PROJECT_NAME} STATIC ${EduNetConnect_LIB_SRC})
