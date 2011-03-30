cmake_minimum_required(VERSION 2.8)

# -----------------------------------------------------------------------------
PROJECT(mgfModule)

file (GLOB ${PROJECT_NAME}_SoureFiles "${${PROJECT_NAME}_SOURCE_DIR}/*.cpp")
file (GLOB ${PROJECT_NAME}_HeaderFiles "${${PROJECT_NAME}_SOURCE_DIR}/*.h")

add_library(${PROJECT_NAME} ${${PROJECT_NAME}_SoureFiles} ${${PROJECT_NAME}_HeaderFiles}
)

source_group( source FILES ${${PROJECT_NAME}_SoureFiles} ${${PROJECT_NAME}_HeaderFiles} )

set_property( TARGET ${PROJECT_NAME} PROPERTY FOLDER "mgf/libraries" )

set_target_properties( ${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX "_Debug" )
set_target_properties( ${PROJECT_NAME} PROPERTIES RELEASE_POSTFIX "_Release" )
set_target_properties( ${PROJECT_NAME} PROPERTIES MINSIZEREL_POSTFIX "_MinsizeRel" )
set_target_properties( ${PROJECT_NAME} PROPERTIES RELWITHDEBINFO_POSTFIX "_RelWithDebugInfo" )


if (MSVC)
   set_target_properties( ${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "/Yustdafx.h" )
   set_source_files_properties( "${${PROJECT_NAME}_SOURCE_DIR}/stdafx.cpp" PROPERTIES COMPILE_FLAGS "/Ycstdafx.h" )
endif(MSVC)
