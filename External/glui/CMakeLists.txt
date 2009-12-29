# This is basically the overall name of the project in Visual Studio this is the name of the Solution File

ADD_DEFINITIONS( -D_CRT_SECURE_NO_WARNINGS)

# For every executable you have with a main method you should have an add_executable line below.
# For every add executable line you should list every .cpp and .h file you have associated with that executable.


# You shouldn't have to modify anything below this line
########################################################

IF (WIN32)
  # This is the Windows code for which Opengl, and Glut are not properly installed
  # since I can't install them I must cheat and copy libraries around
  INCLUDE_DIRECTORIES(${GLUT_ROOT})
 # LINK_DIRECTORIES(${GLUT_ROOT}\\lib)
 # IF (${GLUT_glut_LIBRARY} MATCHES "GLUT_glut_LIBRARY-NOTFOUND")
 #   LINK_LIBRARIES(${GLUT_ROOT}\\lib\\glut32 ${OPENGL_gl_LIBRARY} ${OPENGL_glU_LIBRARY})
 #   TARGET_LINK_LIBRARIES(table ${GLUT_ROOT}\\lib\\glut32)
#
#    ADD_CUSTOM_COMMAND(TARGET table POST_BUILD COMMAND copy ${GLUT_ROOT}\\lib\\glut32.dll ${GLUT_ROOT}\\bin\\vs2005\\Debug
#                        COMMAND copy ${GLUT_ROOT}\\lib\\glut32.dll ${GLUT_ROOT}\\bin\\vs2003\\Debug
#            COMMAND copy ${GLUT_ROOT}\\lib\\glut32.dll ${GLUT_ROOT}\\bin\\vs6\\Debug)
#  ELSE (${GLUT_glut_LIBRARY} MATCHES "GLUT_glut_LIBRARY-NOTFOUND")
#    LINK_LIBRARIES(${GLUT_glut_LIBRARY} ${OPENGL_gl_LIBRARY} ${OPENGL_glu_LIBRARY})
#    TARGET_LINK_LIBRARIES(table ${GLUT_glut_LIBRARY})
#  ENDIF(${GLUT_glut_LIBRARY} MATCHES "GLUT_glut_LIBRARY-NOTFOUND")
#  TARGET_LINK_LIBRARIES(table ${OPENGL_gl_LIBRARY})
#  TARGET_LINK_LIBRARIES(table ${OPENGL_glu_LIBRARY})
ELSE (WIN32)
  # This is the lines for linux.  This should always work if everything is installed and working fine.
#  SET(CMAKE_BUILD_TYPE Debug)
#  SET(CMAKE_CXX_FLAGS_DEBUG "-g")
  #INCLUDE_DIRECTORIES( ${GLUT_INCLUDE_DIR} )
#  TARGET_LINK_LIBRARIES(table ${GLUT_glut_LIBRARY} ${OPENGL_gl_LIBRARY} ${OPENGL_glu_LIBRARY})
#  TARGET_LINK_LIBRARIES(checker ${GLUT_glut_LIBRARY} ${OPENGL_gl_LIBRARY} ${OPENGL_glu_LIBRARY})
ENDIF (WIN32)

#set compiler options to link the lib into a shared lib
IF (NOT WIN32)
   SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
ENDIF (NOT WIN32)

ADD_LIBRARY(GLUI
	arcball.h			glui_checkbox.cpp		glui_internal.h			glui_panel.cpp			glui_spinner.cpp		glui_treepanel.cpp
GL				glui.cpp			glui_column.cpp			glui_internal_control.h		glui_radio.cpp			glui_statictext.cpp		glui_window.cpp
Jamfile				glui_add_controls.cpp		glui_commandline.cpp		glui_list.cpp			glui_rollout.cpp		glui_string.cpp			quaternion.cpp
algebra3.cpp			glui_bitmap_img_data.cpp	glui_control.cpp		glui_listbox.cpp		glui_rotation.cpp		glui_textbox.cpp		quaternion.h
algebra3.h			glui_bitmaps.cpp		glui_edittext.cpp		glui_mouse_iaction.cpp		glui_scrollbar.cpp		glui_translation.cpp
arcball.cpp			glui_button.cpp			glui_filebrowser.cpp		glui_node.cpp			glui_separator.cpp		glui_tree.cpp

)

set_target_properties( GLUI PROPERTIES
	ARCHIVE_OUTPUT_DIRECTORY ${EDUNET_COMMON_LIB_OUTPUT_DIRECTORY} )

IF (BUILD_SHARED_LIBS)
  TARGET_LINK_LIBRARIES(GLUI ${GLUT_glut_LIBRARY} ${OPENGL_gl_LIBRARY} ${OPENGL_glu_LIBRARY})
ENDIF (BUILD_SHARED_LIBS)
