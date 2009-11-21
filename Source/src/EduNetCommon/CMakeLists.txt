project(EduNetCommon)
INCLUDE_DIRECTORIES( ${EDUNET_GAMES_SOURCE_DIR}/Source/src
 ${EDUNET_GAMES_SOURCE_DIR}/Source/opensteer/include
 ${RakNet_SOURCES}/DependentExtensions }
 )

aux_source_directory(${EduNetCommon_SOURCE_DIR} EduNetCommon_LIB_SRC)
aux_source_directory(${EduNetCommon_SOURCE_DIR}/../OpenSteerUT OpenSteerUT_LIB_SRC)

MESSAGE( ${OpenSteerUT_LIB_SRC} )

add_library(${PROJECT_NAME} STATIC 
${EduNetCommon_LIB_SRC}
${OpenSteerUT_LIB_SRC}
${RakNet_SOURCES}/DependentExtensions/RPC3/RPC3.cpp
)