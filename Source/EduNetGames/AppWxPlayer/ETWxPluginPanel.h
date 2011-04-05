#ifndef __EDUNET_PLAYER_WXPLUGIN_PANEL_H__
#define __EDUNET_PLAYER_WXPLUGIN_PANEL_H__
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
#include <mgf/wx/WxWidgets.h>

#include <wx/sharedptr.h> 
#include <wx/panel.h>
#include <wx/splitter.h>
#include "OpenSteer/AbstractPlugin.h"

namespace EduNet
{
	// Define a new frame type: this is going to be our main frame
	class PluginWxPanel : public wxPanel
	{
	public:
		
		PluginWxPanel();
		PluginWxPanel( wxWindow *parent,
			wxWindowID id=wxID_ANY, 
			const wxPoint &pos=wxDefaultPosition,
			const wxSize &size=wxDefaultSize,
			long style=wxTAB_TRAVERSAL,
			const wxString &name=wxPanelNameStr);
		virtual ~PluginWxPanel(){};

		// event handlers (these functions should _not_ be virtual)		
		
		void OnActivate( bool bActive );

		void ChangePlugin( OpenSteer::AbstractPlugin* plugin );
	private:

		void initGlCanvas();

		wxSharedPtr<OpenSteer::AbstractPlugin> m_spPlugin;
		class PlayerGlCanvas* m_pGlCanvas;
		wxPanel* m_pPluginGuiRoot;
		wxSplitterWindow*  m_splitter;
	};

}

#endif //__EDUNET_PLAYER_WXPLUGIN_PANEL_H__
