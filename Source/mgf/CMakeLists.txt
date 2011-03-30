cmake_minimum_required(VERSION 2.8)

Set( MGF_EXTERNAL_LIBRARIES
${wxWidgets_LIBRARIES}
)

add_subdirectory( application )
add_subdirectory( common )
add_subdirectory( connect )
add_subdirectory( core )
add_subdirectory( memory )
add_subdirectory( module )
add_subdirectory( profile )


add_dependencies( mgfCore mgfProfile )
add_dependencies( mgfCore mgfMemory )
add_dependencies( mgfCommon mgfCore )
add_dependencies( mgfConnect mgfCommon )
add_dependencies( mgfApplication mgfCommon )
add_dependencies( mgfModule mgfCommon )

add_subdirectory( applications )
