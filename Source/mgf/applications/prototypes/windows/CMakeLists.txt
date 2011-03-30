cmake_minimum_required(VERSION 2.8)

# -----------------------------------------------------------------------------
PROJECT(mgfViewer)

file (GLOB ${PROJECT_NAME}_SoureFiles "${${PROJECT_NAME}_SOURCE_DIR}/*.cpp")
file (GLOB ${PROJECT_NAME}_HeaderFiles "${${PROJECT_NAME}_SOURCE_DIR}/*.h")


#LINK_LIBRARIES(
#${EduNet_PLAYER_LIBRARIES}
#)

add_executable(${PROJECT_NAME} WIN32 ${${PROJECT_NAME}_SoureFiles} ${${PROJECT_NAME}_HeaderFiles}
)

source_group( source FILES ${${PROJECT_NAME}_SoureFiles} ${${PROJECT_NAME}_HeaderFiles} )

target_link_libraries( ${PROJECT_NAME} mgfMemory mgfProfile mgfCore mgfCommon mgfConnect mgfApplication mgfModule ${MGF_EXTERNAL_LIBRARIES} )


set_property( TARGET ${PROJECT_NAME} PROPERTY FOLDER "mgf/applications/prototypes" )

set_target_properties( ${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX "_Debug" )
set_target_properties( ${PROJECT_NAME} PROPERTIES RELEASE_POSTFIX "_Release" )
set_target_properties( ${PROJECT_NAME} PROPERTIES MINSIZEREL_POSTFIX "_MinsizeRel" )
set_target_properties( ${PROJECT_NAME} PROPERTIES RELWITHDEBINFO_POSTFIX "_RelWithDebugInfo" )


if (MSVC)
   set_target_properties( ${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "/Yustdafx.h" )
   set_source_files_properties( "${${PROJECT_NAME}_SOURCE_DIR}/stdafx.cpp" PROPERTIES COMPILE_FLAGS "/Ycstdafx.h" )
endif(MSVC)
