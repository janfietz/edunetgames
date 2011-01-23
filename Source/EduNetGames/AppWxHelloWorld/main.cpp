//-----------------------------------------------------------------------------
// Copyright (c) Jan Fietz, Cyrus Preuss
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
// * Redistributions of source code must retain the above copyright notice,
//   this list of conditions and the following disclaimer.
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
// * Neither the name of EduNetGames nor the names of its contributors
//   may be used to endorse or promote products derived from this software
//   without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
// IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
// ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
// EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//-----------------------------------------------------------------------------

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all "standard" wxWidgets headers)
#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "wx/glcanvas.h" 

//#include "EduNetApplication/EduNetMain.h"
//
//#include "EduNetApplication/EduNetModulePluginLoader.h"
//#include "EduNetApplication/EduNetModuleManager.h"

namespace EduNet
{
	//ModulePluginLoader* gLoadPlugin = NULL;
	void initializeDynamicPlugins( )
	{
		/*ModuleManager kModuleManager;
		gLoadPlugin = ET_NEW ModulePluginLoader();
		gLoadPlugin->loadModules( kModuleManager.getCurrentModulePath() );
		gLoadPlugin->createPluginsFromModules();*/
	}

	void shutdownDynamicPlugins( )
	{
		//gLoadPlugin->unloadModules();
		//ET_SAFE_DELETE( gLoadPlugin ); 
	}
}



// ----------------------------------------------------------------------------
// resources
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// private classes
// ----------------------------------------------------------------------------

// Define a new application type, each program should derive a class from wxApp
class MyApp : public wxApp
{
public:
	// override base class virtuals
	// ----------------------------

	// this one is called on application startup and is a good place for the app
	// initialization (doing it here and not in the ctor allows to have an error
	// return: if OnInit() returns false, the application terminates)
	virtual bool OnInit();
};

// Define a new frame type: this is going to be our main frame
class MyFrame : public wxFrame
{
public:
	// ctor(s)
	MyFrame(const wxString& title);

	// event handlers (these functions should _not_ be virtual)
	void OnQuit(wxCommandEvent& event);
	void OnAbout(wxCommandEvent& event);

private:
	// any class wishing to process wxWidgets events must use this macro
	DECLARE_EVENT_TABLE()
};

// ----------------------------------------------------------------------------
// constants
// ----------------------------------------------------------------------------

// IDs for the controls and the menu commands
enum
{
	// menu items
	Minimal_Quit = wxID_EXIT,

	// it is important for the id corresponding to the "About" command to have
	// this standard value as otherwise it won't be handled properly under Mac
	// (where it is special and put into the "Apple" menu)
	Minimal_About = wxID_ABOUT
};

// ----------------------------------------------------------------------------
// event tables and other macros for wxWidgets
// ----------------------------------------------------------------------------

// the event tables connect the wxWidgets events with the functions (event
// handlers) which process them. It can be also done at run-time, but for the
// simple menu events like this the static method is much simpler.
BEGIN_EVENT_TABLE(MyFrame, wxFrame)
EVT_MENU(Minimal_Quit,  MyFrame::OnQuit)
EVT_MENU(Minimal_About, MyFrame::OnAbout)
END_EVENT_TABLE()

// Create a new application object: this macro will allow wxWidgets to create
// the application object during program execution (it's better than using a
// static object for many reasons) and also implements the accessor function
// wxGetApp() which will return the reference of the right type (i.e. MyApp and
// not wxApp)
//IMPLEMENT_APP(MyApp)

// ============================================================================
// implementation
// ============================================================================

// ----------------------------------------------------------------------------
// the application class
// ----------------------------------------------------------------------------

// 'Main program' equivalent: the program execution "starts" here
bool MyApp::OnInit()
{
	// call the base class initialization method, currently it only parses a
	// few common command-line options but it could be do more in the future
	if ( !wxApp::OnInit() )
		return false;

	// create the main application window
	MyFrame *frame = new MyFrame("Minimal wxWidgets App");
	wxGLCanvas * MyGLCanvas = new wxGLCanvas(frame, -1, wxPoint(0,0), wxSize(400,400), wxSUNKEN_BORDER, _("some text")); 


	// and show it (the frames, unlike simple controls, are not shown when
	// created initially)
	frame->Show(true);

	MyGLCanvas->SetCurrent(); 
	glClearColor(0.0, 0.0, 0.0, 0.0); 
	glClear(GL_COLOR_BUFFER_BIT); 
	glViewport(50, 50, (GLint)100, (GLint)100); 
	glLoadIdentity(); 
	glTranslatef(0.0f,1.0f,0.0f); 

	glColor3f(1.0, 1.0, 1.0); 

	glBegin(GL_TRIANGLES); 
	glColor3f(1.0f,0.0f,0.0f);                        // Set The Color To Red 
	glVertex3f( 0.0f, 1.0f, 0.0f);                        // Move Up One Unit From Center (Top Point) 
	glColor3f(0.0f,1.0f,0.0f);                        // Set The Color To Green 
	glVertex3f(-1.0f,-1.0f, 0.0f);                        // Left And Down One Unit (Bottom Left) 
	glColor3f(0.0f,0.0f,1.0f);                        // Set The Color To Blue 
	glVertex3f( 1.0f,-1.0f, 0.0f);                        // Right And Down One Unit (Bottom Right) 
	glEnd();   // Done Drawing A Triangle 
	glFlush(); 

	MyGLCanvas->SwapBuffers(); 

	// success: wxApp::OnRun() will be called which will enter the main message
	// loop and the application will run. If we returned false here, the
	// application would exit immediately.
	return true;
}

// ----------------------------------------------------------------------------
// main frame
// ----------------------------------------------------------------------------

// frame constructor
MyFrame::MyFrame(const wxString& title)
: wxFrame(NULL, wxID_ANY, title, wxPoint(50,50), wxSize(450,450))
{
	// set the frame icon
	SetIcon(wxICON(sample));

#if wxUSE_MENUS
	// create a menu bar
	wxMenu *fileMenu = new wxMenu;

	// the "About" item should be in the help menu
	wxMenu *helpMenu = new wxMenu;
	helpMenu->Append(Minimal_About, "&About...\tF1", "Show about dialog");

	fileMenu->Append(Minimal_Quit, "E&xit\tAlt-X", "Quit this program");

	// now append the freshly created menu to the menu bar...
	wxMenuBar *menuBar = new wxMenuBar();
	menuBar->Append(fileMenu, "&File");
	menuBar->Append(helpMenu, "&Help");

	// ... and attach this menu bar to the frame
	SetMenuBar(menuBar);
#endif // wxUSE_MENUS

#if wxUSE_STATUSBAR
	// create a status bar just for fun (by default with 1 pane only)
	CreateStatusBar(2);
	SetStatusText("Welcome to wxWidgets!");
#endif // wxUSE_STATUSBAR
}


// event handlers

void MyFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
	// true is to force the frame to close
	Close(true);
}

void MyFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
	wxMessageBox(wxString::Format
		(
		"Welcome to %s!\n"
		"\n"
		"This is the minimal wxWidgets sample\n"
		"running under %s.",
		wxVERSION_STRING,
		wxGetOsDescription()
		),
		"About wxWidgets minimal sample",
		wxOK | wxICON_INFORMATION,
		this);
}

IMPLEMENT_APP_NO_MAIN(MyApp)

////-----------------------------------------------------------------------------
int main (int argc, char **argv)
{
	if(true == wxEntryStart(argc, argv) )
	{
		wxApp* pApp = static_cast<wxApp*>(wxApp::GetInstance());
		pApp->CallOnInit();
		pApp->OnRun();
		pApp->OnExit();
	}
	wxEntryCleanup();
}



