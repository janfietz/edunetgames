project(ModNetBoid)
INCLUDE_DIRECTORIES(
${EDUNET_GAMES_SOURCE_DIR}/Source/opensteer/include
${ET_RakNet_SOURCES}/DependentExtensions
)

file (GLOB Tutorial_Src "${${PROJECT_NAME}_SOURCE_DIR}/*.cpp")
file (GLOB Tutorial_Hdrs "${${PROJECT_NAME}_SOURCE_DIR}/*.h")

# wxWidgets include (this will do all the magic to configure everything)
include( ${wxWidgets_USE_FILE} )

LINK_DIRECTORIES( 
#${LINK_DIRECTORIES} 
${Boost_LIBRARY_DIRS} )

LINK_LIBRARIES(
${EduNet_MODULE_LIBRARIES}
${RakNet_LIBRARY}
)

add_library(${PROJECT_NAME} SHARED  ${Tutorial_Hdrs} ${Tutorial_Src})
set_property( TARGET ${PROJECT_NAME} PROPERTY FOLDER "edunet/modules" )

set_target_properties( ${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX "_Debug" )
set_target_properties( ${PROJECT_NAME} PROPERTIES RELEASE_POSTFIX "_Release" )
set_target_properties( ${PROJECT_NAME} PROPERTIES MINSIZEREL_POSTFIX "_MinsizeRel" )
set_target_properties( ${PROJECT_NAME} PROPERTIES RELWITHDEBINFO_POSTFIX "_RelWithDebugInfo" )



