project(MemoryTest)
INCLUDE_DIRECTORIES(
${EDUNET_GAMES_SOURCE_DIR}/Source/Src
)
file (GLOB Tutorial_Src "${MemoryTest_SOURCE_DIR}/*.cpp")
file (GLOB Tutorial_Hdrs "${MemoryTest_SOURCE_DIR}/*.h")

LINK_DIRECTORIES( ${LINK_DIRECTORIES} ${Boost_LIBRARY_DIRS} )

LINK_LIBRARIES(
EduNetMemory
)

add_executable(${PROJECT_NAME}  ${Tutorial_Hdrs} ${Tutorial_Src})
set_target_properties( ${PROJECT_NAME} PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY ${EDUNET_COMMON_OUTPUT_DIRECTORY} )



