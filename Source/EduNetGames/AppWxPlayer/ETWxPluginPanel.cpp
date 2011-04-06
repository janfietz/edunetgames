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

#include "ETWxPluginPanel.h"
#include "EduNetPlayerGlCanvas.h"
#include "OpenSteerUT/EmptyPlugin.h"


namespace EduNet
{	

	PluginWxPanel::PluginWxPanel():m_pPluginGuiRoot(NULL)
	{
		this->initGlCanvas();
	}


	// frame constructor
	PluginWxPanel::PluginWxPanel( wxWindow *parent,
	wxWindowID id/*=wxID_ANY*/,
	const wxPoint &pos/*=wxDefaultPosition*/,
	const wxSize &size/*=wxDefaultSize*/,
	long style/*=wxTAB_TRAVERSAL*/, 
	const wxString &name/*=wxPanelNameStr*/ ):wxPanel( parent, id, pos,size,style,name),
	m_pPluginGuiRoot(NULL)
	{
		this->initGlCanvas();



		//ChangePlugin(NULL);
	}

	void PluginWxPanel::initGlCanvas()
	{
		wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

		m_splitter = new wxSplitterWindow(this, wxID_ANY);
		m_splitter->SetSashGravity(0.8);
		m_splitter->SetMinimumPaneSize(100);

		
		m_pGlCanvas = new PlayerGlCanvas(m_splitter, wxID_ANY, wxDefaultPosition, wxDefaultSize,wxFULL_REPAINT_ON_RESIZE);
		
		sizer->Add(m_splitter, 1, wxEXPAND | wxALL, 5);
		SetSizer(sizer);
		GetSizer()->SetSizeHints(this);
	}

	

	// event handlers
	//-----------------------------------------------------------------------------
	void PluginWxPanel::ChangePlugin( OpenSteer::AbstractPlugin* plugin )
	{
		
		if (m_spPlugin != NULL)
		{			
			OpenSteer::AbstractPlugin* pPlugin = m_spPlugin.get();
			m_pGlCanvas->setPlugin( NULL );
			pPlugin->close();
		}

		m_spPlugin = plugin;

		if (m_spPlugin == NULL)		
		{
			// create an empty plugin by default
			m_spPlugin = wxSharedPtr<OpenSteer::AbstractPlugin>(new EmptyPlugin());
		}

		if (m_pPluginGuiRoot != NULL)
		{
			m_splitter->Unsplit(m_pPluginGuiRoot);
			m_pPluginGuiRoot->Destroy();
		}

		m_pPluginGuiRoot = new wxPanel(m_splitter,wxID_ANY,wxDefaultPosition,wxDefaultSize, 0, "PluginGuiPanel");
		
		wxBoxSizer* panelSizer = new wxBoxSizer(wxVERTICAL);
		m_pPluginGuiRoot->SetSizer(panelSizer);

		OpenSteer::AbstractPlugin* pPlugin = m_spPlugin.get();

		pPlugin->prepareOpen();

		wxWindow* subWindow = pPlugin->prepareGui(m_pPluginGuiRoot );
		if( subWindow != NULL)
		{
			subWindow->SetExtraStyle(wxWS_EX_BLOCK_EVENTS);
			panelSizer->Add(subWindow, 1, wxEXPAND);
			subWindow->Layout();
		}

		pPlugin->open();

		m_splitter->SplitVertically(m_pGlCanvas, m_pPluginGuiRoot);

		m_pGlCanvas->setPlugin( pPlugin );

	}
//-----------------------------------------------------------------------------
	void PluginWxPanel::OnActivate( bool bActive )
	{
		if (bActive)
		{
			ChangePlugin(NULL);
		}
		else
		{
			if (m_spPlugin != NULL)
			{			
				OpenSteer::AbstractPlugin* pPlugin = m_spPlugin.get();
				m_pGlCanvas->setPlugin( NULL );
// 				if (pPlugin == OpenSteer::Plugin::getSelectedPlugin())
// 				{
// 					OpenSteer::Plugin::selectPlugin( NULL );
// 				}
				pPlugin->close();			
			}
		}
	}
}
