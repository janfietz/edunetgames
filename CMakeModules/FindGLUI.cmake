# - Try to find GLUI
# Once done this will define
#
#  GLUI_FOUND - system has GLUI
#  GLUI_INCLUDES - the GLUI include directory
#  GLUI_LIBRARY - Link these to use GLUI

FIND_LIBRARY (GLUI_LIBRARY NAMES glui
    PATHS
    ENV LD_LIBRARY_PATH
    ENV LIBRARY_PATH
    /usr/lib64
    /usr/lib
    /usr/local/lib64
    /usr/local/lib
    /opt/local/lib
    )

FIND_PATH (GLUI_INCLUDES GL/glui.h
    ENV CPATH
    /usr/include
    /usr/local/include
    /opt/local/include
    )

IF(GLUI_INCLUDES AND GLUI_LIBRARY)
    SET(GLUI_FOUND TRUE)
ENDIF(GLUI_INCLUDES AND GLUI_LIBRARY)

IF(GLUI_FOUND)
  IF(NOT GLUI_FIND_QUIETLY)
    MESSAGE(STATUS "Found GLUI: ${GLUI_LIBRARIES}")
  ENDIF(NOT GLUI_FIND_QUIETLY)
ELSE(GLUI_FOUND)
  IF(GLUI_FIND_REQUIRED)
    MESSAGE(FATAL_ERROR "Could not find GLUI")
  ENDIF(GLUI_FIND_REQUIRED)
ENDIF(GLUI_FOUND)

