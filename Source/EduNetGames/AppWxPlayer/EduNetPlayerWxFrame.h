#ifndef __EDUNET_PLAYER_WXFRAME_H__
#define __EDUNET_PLAYER_WXFRAME_H__
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

#include "wx/sharedptr.h" 
#include "wx/aui/aui.h"
#include "OpenSteer/AbstractPlugin.h"

#include "EduNetPlayerWxEvents.h"

namespace EduNet
{

	// Define a new frame type: this is going to be our main frame
	class PlayerWxFrame : public wxFrame
	{
	public:
		// ctor(s)
		PlayerWxFrame(const wxString& title, class ModulePluginLoader* pModuleManager);
		virtual ~PlayerWxFrame();

		// event handlers (these functions should _not_ be virtual)
		void OnQuit(wxCommandEvent& event);
		void OnAbout(wxCommandEvent& event);
		void OnActivate(wxActivateEvent& event);
		void OnCreatePlugin( PluginCreateEvent& event);

		void CreateModuleTree();

	private:
		
		wxAuiManager m_mgr;
		class PluginWxPanel* m_pluginPanel;

		ModulePluginLoader* m_pModuleManager;
		
		// any class wishing to process wxWidgets events must use this macro
		DECLARE_EVENT_TABLE()
		void OnCreateModuleTree( wxCommandEvent& event );
	};

}

#endif //__EDUNET_PLAYER_WXFRAME_H__
