project(NetBoidDll)
INCLUDE_DIRECTORIES( 
${EDUNET_GAMES_SOURCE_DIR}/Source/opensteer/include
${RakNet_SOURCES}/DependentExtensions
)

file (GLOB Tutorial_Src "${NetBoidDll_SOURCE_DIR}/*.cpp")
file (GLOB Tutorial_Hdrs "${NetBoidDll_SOURCE_DIR}/*.h")
message(STATUS ${Tutorial_Hdrs} ${Tutorial_Src} )

LINK_DIRECTORIES( ${LINK_DIRECTORIES} ${Boost_LIBRARYDIR} )

LINK_LIBRARIES(
${EduNet_DEFAULT_LIBRARIES} 
${RakNet_LIBRARY}
)

add_library(${PROJECT_NAME} SHARED  ${Tutorial_Hdrs} ${Tutorial_Src})
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${EDUNET_COMMON_OUTPUT_DIRECTORY})
set_target_properties(${PROJECT_NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${EDUNET_COMMON_OUTPUT_DIRECTORY})



