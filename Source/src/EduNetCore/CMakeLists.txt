cmake_minimum_required(VERSION 2.8)

# -----------------------------------------------------------------------------
project(EduNetCore)

INCLUDE_DIRECTORIES( 
${EDUNET_GAMES_SOURCE_DIR}/Source/src
${EDUNET_GAMES_SOURCE_DIR}/Source
 )

file (GLOB EduNetCore_LIB_SRC "${${PROJECT_NAME}_SOURCE_DIR}/*.cpp")
file (GLOB EduNetCore_LIB_Hdrs "${${PROJECT_NAME}_SOURCE_DIR}/*.h")

file (GLOB MgfCore_LIB_SRC "${${PROJECT_NAME}_SOURCE_DIR}/../../mgf/core/*.cpp")
file (GLOB MgfCore_LIB_Hdrs "${${PROJECT_NAME}_SOURCE_DIR}/../../mgf/core/*.h")
file (GLOB MgfCore_OS_Hdrs "${${PROJECT_NAME}_SOURCE_DIR}/../../mgf/core/os/*.h")
file (GLOB MgfCore_WX_Hdrs "${${PROJECT_NAME}_SOURCE_DIR}/../../mgf/wx/*.h")

file (GLOB MgfMemory_LIB_SRC "${${PROJECT_NAME}_SOURCE_DIR}/../../mgf/memory/M*.cpp")
file (GLOB MgfMemory_LIB_Hdrs "${${PROJECT_NAME}_SOURCE_DIR}/../../mgf/memory/M*.h")


#set compiler options to link the lib into a shared lib
IF (NOT WIN32)
   SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
ENDIF (NOT WIN32)

add_library(${PROJECT_NAME} STATIC
${EduNetCore_LIB_SRC}
${EduNetCore_LIB_Hdrs}
${MgfCore_LIB_SRC}
${MgfCore_LIB_Hdrs}
${MgfCore_OS_Hdrs}
${MgfCore_WX_Hdrs}
${MgfMemory_LIB_SRC}
${MgfMemory_LIB_Hdrs}
)

source_group( source FILES ${EduNetCore_LIB_SRC} ${EduNetCore_LIB_Hdrs} )
source_group( mgf\\core FILES ${MgfCore_LIB_SRC} ${MgfCore_LIB_Hdrs} )
source_group( mgf\\core\\os FILES ${MgfCore_OS_Hdrs} )
source_group( mgf\\wx FILES ${MgfCore_WX_Hdrs} )
source_group( mgf\\memory FILES ${MgfMemory_LIB_SRC} ${MgfMemory_LIB_Hdrs} )

set_property( TARGET ${PROJECT_NAME} PROPERTY FOLDER "edunet/libraries" )

set_target_properties( ${PROJECT_NAME} PROPERTIES
	ARCHIVE_OUTPUT_DIRECTORY ${EDUNET_COMMON_LIB_OUTPUT_DIRECTORY} )

set_target_properties( ${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX "_Debug" )
set_target_properties( ${PROJECT_NAME} PROPERTIES RELEASE_POSTFIX "_Release" )
set_target_properties( ${PROJECT_NAME} PROPERTIES MINSIZEREL_POSTFIX "_MinsizeRel" )
set_target_properties( ${PROJECT_NAME} PROPERTIES RELWITHDEBINFO_POSTFIX "_RelWithDebugInfo" )


#if (MSVC)
   set_target_properties( ${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "/Yustdafx.h" )
   set_source_files_properties( "${${PROJECT_NAME}_SOURCE_DIR}/../../mgf/core/stdafx.cpp" PROPERTIES COMPILE_FLAGS "/Ycstdafx.h" )
#endif(MSVC)
