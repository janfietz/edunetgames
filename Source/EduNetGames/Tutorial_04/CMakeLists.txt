project(Tutorial04)
INCLUDE_DIRECTORIES( 
${EDUNET_GAMES_SOURCE_DIR}/Source/opensteer/include
${RakNet_SOURCES}/DependentExtensions
)

file (GLOB Tutorial_Src "${Tutorial04_SOURCE_DIR}/*.cpp")
file (GLOB Tutorial_Hdrs "${Tutorial04_SOURCE_DIR}/*.h")

LINK_LIBRARIES(
${EduNet_DEFAULT_LIBRARIES} 
${RakNet_LIBRARY}
)

add_executable(${PROJECT_NAME} ${Tutorial_Hdrs} ${Tutorial_Src})
set_target_properties( ${PROJECT_NAME} PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY ${EDUNET_COMMON_OUTPUT_DIRECTORY} )


