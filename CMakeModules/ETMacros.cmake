# - Try to find RakNet
######################################################################
# return a list of directories that we should add_subdirectory()
macro(EDUNET_COLLECT_SUBPROJECT_DIRECTORY_NAMES varname filename)
  file(GLOB EDUNET_PLUGIN_CMAKE_FILES
    RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "*/${filename}")
  foreach(EDUNET_PLUGIN_CMAKE_FILE ${EDUNET_PLUGIN_CMAKE_FILES})
	get_filename_component(EDUNET_PLUGIN_DIR ${EDUNET_PLUGIN_CMAKE_FILE} PATH)
    set(${varname} ${${varname}} ${EDUNET_PLUGIN_DIR})
  endforeach(EDUNET_PLUGIN_CMAKE_FILE ${EDUNET_PLUGIN_CMAKE_FILES})
endmacro(EDUNET_COLLECT_SUBPROJECT_DIRECTORY_NAMES varname filename)

######################################################################
# Add a list of subdirs
macro(ADD_SUBDIRECTORIES prefix header )
  message(STATUS "- ${header} -")
  foreach(subdir ${ARGN})
    message(STATUS "${prefix}${subdir}")
    add_subdirectory(${subdir})
  endforeach(subdir ${ARGN})
endmacro(ADD_SUBDIRECTORIES prefix header)

######################################################################
# copy all files to the target folder
macro(COPYFILES_TO msg targetdir)
  message(STATUS "- ${msg} to ${targetdir}")
  foreach(et_libPath ${ARGN})		
		#get_filename_component( filename ${et_libPath} NAME )
		message(STATUS "   ${et_libPath}")
		configure_file( ${et_libPath} ${targetdir} COPYONLY )
	endforeach(et_libPath)
endmacro(COPYFILES_TO msg targetdir)