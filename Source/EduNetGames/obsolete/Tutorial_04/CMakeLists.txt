project(Tutorial04)
INCLUDE_DIRECTORIES(
${EDUNET_GAMES_SOURCE_DIR}/Source/opensteer/include
${ET_RakNet_SOURCES}/DependentExtensions
)

file (GLOB Tutorial_Src "${${PROJECT_NAME}_SOURCE_DIR}/*.cpp")
file (GLOB Tutorial_Hdrs "${${PROJECT_NAME}_SOURCE_DIR}/*.h")
file (GLOB Renderer_Src "${${PROJECT_NAME}_SOURCE_DIR}/../../opensteer/src/Draw.cpp")

LINK_DIRECTORIES( ${LINK_DIRECTORIES} ${Boost_LIBRARY_DIRS} )

LINK_LIBRARIES(
${EduNet_DEFAULT_LIBRARIES}
${RakNet_LIBRARY}
)

add_executable(${PROJECT_NAME} ${Tutorial_Hdrs} ${Tutorial_Src} ${Renderer_Src})

set_target_properties( ${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX "_Debug" )
set_target_properties( ${PROJECT_NAME} PROPERTIES RELEASE_POSTFIX "_Release" )
set_target_properties( ${PROJECT_NAME} PROPERTIES MINSIZEREL_POSTFIX "_MinsizeRel" )
set_target_properties( ${PROJECT_NAME} PROPERTIES RELWITHDEBINFO_POSTFIX "_RelWithDebugInfo" )


