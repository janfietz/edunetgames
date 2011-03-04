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
	BEGIN_EVENT_TABLE(PluginWxPanel, wxPanel)
		
	END_EVENT_TABLE()

	PluginWxPanel::PluginWxPanel()
	{
		this->initGlCanvas();
	}


	// frame constructor
	PluginWxPanel::PluginWxPanel( wxWindow *parent,
	wxWindowID id/*=wxID_ANY*/,
	const wxPoint &pos/*=wxDefaultPosition*/,
	const wxSize &size/*=wxDefaultSize*/,
	long style/*=wxTAB_TRAVERSAL*/, 
	const wxString &name/*=wxPanelNameStr*/ ):wxPanel( parent, id, pos,size,style,name)
	{
		this->initGlCanvas();

		ChangePlugin(NULL);
	}

	void PluginWxPanel::initGlCanvas()
	{

		wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
		

		m_pGlCanvas = new PlayerGlCanvas(this, wxID_ANY, wxDefaultPosition, wxDefaultSize,wxFULL_REPAINT_ON_RESIZE);

		m_pGlCanvas->initGL();

		sizer->Add(m_pGlCanvas, 1, wxEXPAND | wxALL, 5);
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
// 			if (pPlugin == OpenSteer::Plugin::getSelectedPlugin())
// 			{
// 				OpenSteer::Plugin::selectPlugin( NULL );
// 			}
			pPlugin->close();			
		}

		m_spPlugin = plugin;

		if (m_spPlugin == NULL)		
		{
			// create an empty plugin by default
			m_spPlugin = wxSharedPtr<OpenSteer::AbstractPlugin>(new EmptyPlugin());
		}

		OpenSteer::AbstractPlugin* pPlugin = m_spPlugin.get();
		//OpenSteer::Plugin::selectPlugin( pPlugin );
		pPlugin->prepareOpen();
		pPlugin->open();

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
