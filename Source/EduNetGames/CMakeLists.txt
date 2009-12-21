# return a list of directories that we should add_subdirectory()
macro(EDUNET_COLLECT_SUBPROJECT_DIRECTORY_NAMES varname filename)
  file(GLOB EDUNET_PLUGIN_CMAKE_FILES 
    RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "*/${filename}")
  foreach(EDUNET_PLUGIN_CMAKE_FILE ${EDUNET_PLUGIN_CMAKE_FILES})
	get_filename_component(EDUNET_PLUGIN_DIR ${EDUNET_PLUGIN_CMAKE_FILE} PATH)
    set(${varname} ${${varname}} ${EDUNET_PLUGIN_DIR})
  endforeach(EDUNET_PLUGIN_CMAKE_FILE ${EDUNET_PLUGIN_CMAKE_FILES})
endmacro(EDUNET_COLLECT_SUBPROJECT_DIRECTORY_NAMES varname)

macro(ADD_SUBDIRECTORIES prefix)
  foreach(subdir ${ARGN})
    message(STATUS "${prefix}${subdir}")
    add_subdirectory(${subdir})
  endforeach(subdir ${ARGN})
endmacro(ADD_SUBDIRECTORIES prefix)

# Find all of the subdirectories with CMakeLists.txt files
edunet_collect_subproject_directory_names(EDUNET_PLUGIN_DIRS "CMakeLists.txt")
add_subdirectories(" + " ${EDUNET_PLUGIN_DIRS})
