# collect all plugins
# Find all of the subdirectories with CMakeLists.txt files
#edunet_collect_subproject_directory_names(EDUNET_PLUGIN_DIRS "CMakeLists.txt")
#add_subdirectories(" + " "Plugins" ${EDUNET_PLUGIN_DIRS} )
#test
#add_subdirectory(Tutorial_01)
#add_subdirectory(MemoryTest)
#add_subdirectory(Tutorial_03)
add_subdirectory(EduNetPlayer)
add_subdirectory(ModZoning)
add_subdirectory(ModNetSoccer)