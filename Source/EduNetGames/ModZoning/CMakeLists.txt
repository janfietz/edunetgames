project(ModZoning)
INCLUDE_DIRECTORIES(
${EDUNET_GAMES_SOURCE_DIR}/Source/opensteer/include
${ET_RakNet_SOURCES}/DependentExtensions
)

file (GLOB Module_Src "${${PROJECT_NAME}_SOURCE_DIR}/*.cpp")
file (GLOB Module_Hdrs "${${PROJECT_NAME}_SOURCE_DIR}/*.h")
file (GLOB Pedestrian_Src "${${PROJECT_NAME}_SOURCE_DIR}/../Pedestrians/*.cpp")
file (GLOB Pedestrian_Hdrs "${${PROJECT_NAME}_SOURCE_DIR}/../Pedestrians/*.h")

LINK_DIRECTORIES( ${LINK_DIRECTORIES} ${Boost_LIBRARY_DIRS} )

add_library(${PROJECT_NAME} SHARED  ${Module_Hdrs} ${Module_Src}  ${Pedestrian_Hdrs} ${Pedestrian_Src})
target_link_libraries(${PROJECT_NAME} 
${EduNet_MODULE_LIBRARIES}
ExtOpensteer
EduNetCommon
EduNetCore
${RAKNET_LIBRARY_LIBS}
)

set_property( TARGET ${PROJECT_NAME} PROPERTY FOLDER "edunet/modules" )

source_group( source FILES ${Module_Src} ${Module_Hdrs} )
source_group( pedestrians FILES ${Pedestrian_Src} ${Pedestrian_Hdrs} )


set_target_properties( ${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX "_Debug" )
set_target_properties( ${PROJECT_NAME} PROPERTIES RELEASE_POSTFIX "_Release" )
set_target_properties( ${PROJECT_NAME} PROPERTIES MINSIZEREL_POSTFIX "_MinsizeRel" )
set_target_properties( ${PROJECT_NAME} PROPERTIES RELWITHDEBINFO_POSTFIX "_RelWithDebugInfo" )

INSTALL(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin
				LIBRARY DESTINATION bin
				ARCHIVE DESTINATION lib)

