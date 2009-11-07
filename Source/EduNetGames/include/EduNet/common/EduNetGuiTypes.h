#ifndef __EDUNETGUITYPES_H__
#define __EDUNETGUITYPES_H__


#if EDUNET_HAVE_GLUI

// forward all glui classes
class GLUI;
class GLUI_Control;
class GLUI_Listbox;
class GLUI_StaticText;
class GLUI_EditText;
class GLUI_Panel;
class GLUI_Spinner;
class GLUI_RadioButton;
class GLUI_RadioGroup;
class GLUI_Glut_Window;
class GLUI_TreePanel;
class GLUI_Scrollbar;
class GLUI_List;

class Arcball;

GLUI* getRootGLUI();

#endif //  EDUNET_HAVE_GLUI



#endif // __EDUNETGUITYPES_H__