project(Start)
INCLUDE_DIRECTORIES(
${EDUNET_GAMES_SOURCE_DIR}/Source/opensteer/include )

file (GLOB Tutorial_Src "${Start_SOURCE_DIR}/*.cpp")
file (GLOB Tutorial_Hdrs "${Start_SOURCE_DIR}/*.h")

LINK_DIRECTORIES( ${LINK_DIRECTORIES} ${Boost_LIBRARY_DIRS} )

LINK_LIBRARIES( ${EduNet_DEFAULT_LIBRARIES} )

add_executable(${PROJECT_NAME} ${Tutorial_Hdrs} ${Tutorial_Src})
set_target_properties( ${PROJECT_NAME} PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY ${EDUNET_COMMON_OUTPUT_DIRECTORY} )


