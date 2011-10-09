project(ModNetSoccer)
INCLUDE_DIRECTORIES(
${EDUNET_GAMES_SOURCE_DIR}/Source/opensteer/include
${ET_RakNet_SOURCES}/DependentExtensions
)

file (GLOB Tutorial_Src "${${PROJECT_NAME}_SOURCE_DIR}/*.cpp")
file (GLOB Tutorial_Hdrs "${${PROJECT_NAME}_SOURCE_DIR}/*.h")

LINK_DIRECTORIES( ${LINK_DIRECTORIES} ${Boost_LIBRARY_DIRS} )

add_library(${PROJECT_NAME} SHARED  ${Tutorial_Hdrs} ${Tutorial_Src})
target_link_libraries(${PROJECT_NAME} 
${EduNet_MODULE_LIBRARIES}
ExtOpensteer
EduNetCommon
EduNetCore
${RAKNET_LIBRARY_LIBS}
)

set_property( TARGET ${PROJECT_NAME} PROPERTY FOLDER "edunet/modules" )

set_target_properties( ${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX "_Debug" )
set_target_properties( ${PROJECT_NAME} PROPERTIES RELEASE_POSTFIX "_Release" )
set_target_properties( ${PROJECT_NAME} PROPERTIES MINSIZEREL_POSTFIX "_MinsizeRel" )
set_target_properties( ${PROJECT_NAME} PROPERTIES RELWITHDEBINFO_POSTFIX "_RelWithDebugInfo" )

INSTALL(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin
				LIBRARY DESTINATION bin
				ARCHIVE DESTINATION lib)

