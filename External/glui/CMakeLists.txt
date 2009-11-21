# This is basically the overall name of the project in Visual Studio this is the name of the Solution File

ADD_DEFINITIONS( -D_CRT_SECURE_NO_WARNINGS)
ADD_LIBRARY(GLUI
	arcball.h			glui_checkbox.cpp		glui_internal.h			glui_panel.cpp			glui_spinner.cpp		glui_treepanel.cpp
GL				glui.cpp			glui_column.cpp			glui_internal_control.h		glui_radio.cpp			glui_statictext.cpp		glui_window.cpp
Jamfile				glui_add_controls.cpp		glui_commandline.cpp		glui_list.cpp			glui_rollout.cpp		glui_string.cpp			quaternion.cpp
algebra3.cpp			glui_bitmap_img_data.cpp	glui_control.cpp		glui_listbox.cpp		glui_rotation.cpp		glui_textbox.cpp		quaternion.h
algebra3.h			glui_bitmaps.cpp		glui_edittext.cpp		glui_mouse_iaction.cpp		glui_scrollbar.cpp		glui_translation.cpp		
arcball.cpp			glui_button.cpp			glui_filebrowser.cpp		glui_node.cpp			glui_separator.cpp		glui_tree.cpp
	
)

IF (BUILD_SHARED_LIBS)
  TARGET_LINK_LIBRARIES(GLUI ${GLUT_glut_LIBRARY} ${OPENGL_gl_LIBRARY} ${OPENGL_glu_LIBRARY})
ENDIF (BUILD_SHARED_LIBS)
