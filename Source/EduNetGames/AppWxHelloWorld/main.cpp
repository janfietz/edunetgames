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

#include "wx/wx.h"
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

	

	// Declare our main frame class 
	class MyFrame : public wxFrame
	{ 
	public:
		// Constructor 
		MyFrame(const wxString& title);
		// Event handlers
		void OnQuit(wxCommandEvent& event);
		void OnAbout(wxCommandEvent& event);
	private: 
			// This class handles events
		DECLARE_EVENT_TABLE()
	};
	
// Event table
	// Event table for MyFrame 

	void MyFrame::OnAbout(wxCommandEvent& event)
	{
		wxString msg; msg.Printf(wxT("Hello and welcome to %s"),
			wxVERSION_STRING);
		wxMessageBox(msg, wxT("About Minimal"),
			wxOK | wxICON_INFORMATION, this);
	}
	void MyFrame::OnQuit(wxCommandEvent& event)
	{
		// Destroy the frame
		Close();
	}
	
//#include "mondrian.xpm"
	MyFrame::MyFrame(const wxString& title) :
		wxFrame(NULL, wxID_ANY, title)
	{
		// Set the frame icon
		//SetIcon(wxIcon(mondrian_xpm));
		// Create a menu bar
		wxMenu *fileMenu = new wxMenu;
		// The “About” item should be in the help menu 
		wxMenu *helpMenu = new wxMenu;
		helpMenu->Append(wxID_ABOUT, wxT("&About...\tF1"),
			wxT("Show about dialog"));
		fileMenu->Append(wxID_EXIT, wxT("E&xit\tAlt-X"),
			wxT("Quit this program"));
		// Now append the freshly created menu to the menu bar... 
		wxMenuBar *menuBar = new wxMenuBar();
		menuBar->Append(fileMenu, wxT("&File"));
		menuBar->Append(helpMenu, wxT("&Help"));
		// ... and attach this menu bar to the frame
		SetMenuBar(menuBar);
		// Create a status bar just for fun 
		CreateStatusBar(2);
		SetStatusText(wxT("Welcome to wxWidgets!"));
	}
}


// Declare the application class 
class MyApp : public wxApp
{ 
public:
	// Called on application startup 
	virtual bool OnInit();
};

// Give wxWidgets the means to create a MyApp object 
//IMPLEMENT_APP(MyApp)

// Initialize the application bool 
bool MyApp::OnInit()
{
	// Create the main application window 
	EduNet::MyFrame *frame = new EduNet::MyFrame(wxT("Minimal wxWidgets App"));
	// Show it 
	frame->Show(true);
	// Start the event loop
	return true;
}

BEGIN_EVENT_TABLE(EduNet::MyFrame, wxFrame)
	EVT_MENU(wxID_ABOUT, EduNet::MyFrame::OnAbout)
	EVT_MENU(wxID_EXIT, EduNet::MyFrame::OnQuit)
END_EVENT_TABLE()

// Implements MyApp& GetApp() 
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



