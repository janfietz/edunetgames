project(EduNetPlayer)
INCLUDE_DIRECTORIES(
 ${EDUNET_GAMES_SOURCE_DIR}/Source/opensteer/include
${EDUNET_GAMES_SOURCE_DIR}/Source/Src
)
file (GLOB Tutorial_Src "${EduNetPlayer_SOURCE_DIR}/*.cpp")
file (GLOB Tutorial_Hdrs "${EduNetPlayer_SOURCE_DIR}/*.h")

LINK_DIRECTORIES( ${LINK_DIRECTORIES} ${Boost_LIBRARY_DIRS} )

LINK_LIBRARIES(
${EduNet_DEFAULT_LIBRARIES}
)

add_executable(${PROJECT_NAME}  ${Tutorial_Hdrs} ${Tutorial_Src})

set_target_properties( ${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX "_Debug" )
set_target_properties( ${PROJECT_NAME} PROPERTIES RELEASE_POSTFIX "_Release" )
set_target_properties( ${PROJECT_NAME} PROPERTIES MINSIZEREL_POSTFIX "_MinsizeRel" )
set_target_properties( ${PROJECT_NAME} PROPERTIES RELWITHDEBINFO_POSTFIX "_RelWithDebugInfo" )

