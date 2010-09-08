project(ModNetSoccer)
INCLUDE_DIRECTORIES(
${EDUNET_GAMES_SOURCE_DIR}/Source/opensteer/include
${ET_RakNet_SOURCES}/DependentExtensions
)

file (GLOB Tutorial_Src "${ModNetSoccer_SOURCE_DIR}/*.cpp")
file (GLOB Tutorial_Hdrs "${ModNetSoccer_SOURCE_DIR}/*.h")

LINK_DIRECTORIES( ${LINK_DIRECTORIES} ${Boost_LIBRARY_DIRS} )

LINK_LIBRARIES(
${EduNet_DEFAULT_LIBRARIES}
${RakNet_LIBRARY}
)

add_library(${PROJECT_NAME} SHARED  ${Tutorial_Hdrs} ${Tutorial_Src})

SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES PREFIX "Module")

set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${EDUNET_COMMON_OUTPUT_DIRECTORY})
set_target_properties(${PROJECT_NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${EDUNET_COMMON_OUTPUT_DIRECTORY})



