project(EduNetProfile)

INCLUDE_DIRECTORIES( ${EDUNET_GAMES_SOURCE_DIR}/Source/src
 ${EDUNET_GAMES_SOURCE_DIR}/Source/opensteer/include
 ${RakNet_SOURCES}/DependentExtensions
 )

SET(EDUNETPROFILE_SOURCES 
GraphPlot.cpp
)


SET(EDUNETPROFILE_HEADERS
GraphPlot.h
)

ADD_LIBRARY(EduNetProfile STATIC ${EDUNETPROFILE_SOURCES})

