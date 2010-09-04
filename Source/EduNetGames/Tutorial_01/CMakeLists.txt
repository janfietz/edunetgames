project(Tutorial01)
INCLUDE_DIRECTORIES(
${EDUNET_GAMES_SOURCE_DIR}/Source/opensteer/include
${ET_RakNet_SOURCES}/DependentExtensions
)

file (GLOB Tutorial_Src "${Tutorial01_SOURCE_DIR}/*.cpp")
file (GLOB Tutorial_Hdrs "${Tutorial01_SOURCE_DIR}/*.h")
 
LINK_DIRECTORIES( ${LINK_DIRECTORIES} ${Boost_LIBRARY_DIRS} )
LINK_LIBRARIES(
${EduNet_DEFAULT_LIBRARIES}
${RakNet_LIBRARY}
)

add_executable(${PROJECT_NAME} ${Tutorial_Hdrs} ${Tutorial_Src})
set_target_properties( ${PROJECT_NAME} PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY ${EDUNET_COMMON_OUTPUT_DIRECTORY} )


