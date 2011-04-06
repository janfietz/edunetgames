//-----------------------------------------------------------------------------
// Copyright (c) 2009, Jan Fietz, Cyrus Preuss
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

#include "EduNetPlayerWxFrame.h"
#include "EduNetPlayerWxEvents.h"
#include "EtWxPluginPanel.h"
#include "EduNetWxModuleTree.h"
#include "EduNetCommon/EduNetOptions.h"

#include "EduNetApplication/EduNetModulePluginLoader.h"

#include "OpenSteerUT/EmptyPlugin.h"
namespace EduNet
{
	enum
	{
		PANE_MODULETREE = 1,
		VIEW_MODULETREE,
	};

	BEGIN_EVENT_TABLE(PlayerWxFrame, wxFrame)
		EVT_MENU(wxID_EXIT,  PlayerWxFrame::OnQuit)
		EVT_MENU(wxID_ABOUT, PlayerWxFrame::OnAbout)
		EVT_ACTIVATE(PlayerWxFrame::OnActivate)
		EVT_PLUGIN_CREATED(ModuleTreeCtrl_Ctrl, PlayerWxFrame::OnCreatePlugin)
		EVT_MENU(VIEW_MODULETREE,PlayerWxFrame::OnCreateModuleTree)
	END_EVENT_TABLE()


	// frame constructor
	PlayerWxFrame::PlayerWxFrame(const wxString& title, ModulePluginLoader* pModuleManager)
	: wxFrame(NULL, wxID_ANY, title, wxPoint(50,50), wxSize(800,700)),
		m_pModuleManager(pModuleManager),
		m_pluginPanel(NULL)
	{

		m_mgr.SetManagedWindow(this);

		// set the frame icon
		SetIcon(wxICON(sample));

#if wxUSE_MENUS
		// create a menu bar
		wxMenu *fileMenu = new wxMenu;

		// the "About" item should be in the help menu
		wxMenu *helpMenu = new wxMenu;
		helpMenu->Append(wxID_ABOUT, "&About...\tF1", "Show about dialog");

		fileMenu->Append(wxID_EXIT, "E&xit\tAlt-X", "Quit this program");

		wxMenu *viewMenu = new wxMenu;
		viewMenu->Append(VIEW_MODULETREE, wxT("&Show Modules"), wxT("Show the module tree"));
		

		// now append the freshly created menu to the menu bar...
		wxMenuBar *menuBar = new wxMenuBar();
		menuBar->Append(fileMenu, wxT("&File") );
		menuBar->Append(viewMenu, wxT("&View") );
		menuBar->Append(helpMenu, wxT("&Help") );

		// ... and attach this menu bar to the frame
		SetMenuBar(menuBar);
#endif // wxUSE_MENUS

		m_pluginPanel = new PluginWxPanel(this);
		m_mgr.AddPane(m_pluginPanel, wxAuiPaneInfo().Name(wxT("plugin_content")).
			CenterPane().PaneBorder(false));
		
#if wxUSE_STATUSBAR
		// create a status bar just for fun (by default with 1 pane only)
		CreateStatusBar(2);
		SetStatusText("Welcome to wxWidgets!");
#endif // wxUSE_STATUSBAR

		m_mgr.Update();

		Bind( wxEVT_COMMAND_BUTTON_CLICKED, &PlayerWxFrame::OnButtonClicked, this);

		wxString kPreselectedPlugin(EduNet::Options::accessOptions().getSelectedPlugin() );
		bool bPreselectedPluginCreated(false);
		if(kPreselectedPlugin.IsEmpty() == false )
		{
			wxString module;
			bPreselectedPluginCreated = CreatePlugin(kPreselectedPlugin, module);
		}

		
		if (bPreselectedPluginCreated == false)
		{
			CreateModuleTree();
		}

		
	}

	//-----------------------------------------------------------------------------
	PlayerWxFrame::~PlayerWxFrame()
	{
		m_pluginPanel->OnActivate(false);

		m_mgr.UnInit();
	}

	// event handlers

	void PlayerWxFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
	{
		// true is to force the frame to close
		Close(true);
	}

	void PlayerWxFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
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

	void PlayerWxFrame::OnActivate( wxActivateEvent& event )
	{		
		//m_pluginPanel->OnActivate(event.GetActive() );				
	}

	//-----------------------------------------------------------------------------
	void PlayerWxFrame::OnCreatePlugin( PluginCreateEvent& event)
	{
		wxASSERT(m_pModuleManager != NULL);
		
		const wxString& moduleName = event.GetModuleName();
		const wxString& pluginName = event.GetPluginName();
		CreatePlugin(pluginName, moduleName);

	}

	bool PlayerWxFrame::CreatePlugin( const wxString &pluginName, const wxString &moduleName )
	{
		OpenSteer::AbstractPlugin* plugin;
		if( moduleName.empty() )
		{
			plugin = m_pModuleManager->createPluginByName( pluginName.c_str() );
		}
		else
		{
			plugin = m_pModuleManager->createPluginByName( pluginName.c_str(), moduleName.c_str() );
		}		
		m_pluginPanel->ChangePlugin(plugin);
#if wxUSE_STATUSBAR
		if (NULL != plugin)
		{
			wxString txt;
			txt.Printf("Created plugin:%s", plugin->pluginName() );
			SetStatusText( txt );
		}
		
#endif // wxUSE_STATUSBAR
		return plugin != NULL;
	}

	//-----------------------------------------------------------------------------
	void PlayerWxFrame::CreateModuleTree( )
	{
		wxAuiPaneInfo& auiInfo = m_mgr.GetPane(wxT("ModuleTree"));
		if (auiInfo.IsOk() == true)
		{
			auiInfo.Show(true);
		}
		else
		{
			ModuleTreeCtrl* moduleTree = new ModuleTreeCtrl( m_pModuleManager, this, wxPoint(0,0), wxSize(250,300) );
			m_mgr.AddPane(moduleTree, wxAuiPaneInfo().
				Name(wxT("ModuleTree")).Caption(wxT("Module Tree")).
				Left().Layer(1).Position(1).
				CloseButton(true).MaximizeButton(true));
		}
		
		m_mgr.Update();
	}

	//-----------------------------------------------------------------------------
	void PlayerWxFrame::OnCreateModuleTree( wxCommandEvent& WXUNUSED(event ) )
	{			
		CreateModuleTree();		
	}

	void PlayerWxFrame::OnButtonClicked( wxCommandEvent& event )
	{

	}
}
