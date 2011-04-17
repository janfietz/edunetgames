#ifndef __EDUNET_PLAYER_WX_EVENTS_H__
#define __EDUNET_PLAYER_WX_EVENTS_H__
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

#ifdef WXDLLIMPEXP_CORE
#define __PREV_WXDLLIMPEXP_CORE
#undef WXDLLIMPEXP_CORE
#define WXDLLIMPEXP_CORE
#endif

namespace EduNet
{
	class PluginCreateEvent : public wxNotifyEvent
	{ 
	public:
		PluginCreateEvent(wxString moduleName = "", wxString pluginName="", wxEventType commandType = wxEVT_NULL, int id = 0):
		  wxNotifyEvent(commandType, id), 
			m_moduleName(moduleName),
			m_pluginName(pluginName)
		{}
		PluginCreateEvent(const PluginCreateEvent& event): wxNotifyEvent(event),
			m_moduleName(event.m_moduleName),
			m_pluginName(event.m_pluginName)
		{}
		virtual wxEvent *Clone() const { return new PluginCreateEvent(*this); }

		const wxString& GetPluginName() const{ return m_pluginName; }
		const wxString& GetModuleName() const{ return m_moduleName; }

	private:
		wxString m_moduleName;
		wxString m_pluginName;

		DECLARE_DYNAMIC_CLASS(PluginCreateEvent); 
	};
	typedef void (wxEvtHandler::*PluginCreateEventFunction) (PluginCreateEvent&);

	// TODO: @JF do not use deprecated code see event.h
	BEGIN_DECLARE_EVENT_TYPES() 
		DECLARE_EVENT_TYPE(etEVT_COMMAND_PLUGIN_CREATE, PluginCreateEvent)
	END_DECLARE_EVENT_TYPES()
	
	#define EVT_PLUGIN_CREATED(id, fn) \
		DECLARE_EVENT_TABLE_ENTRY( etEVT_COMMAND_PLUGIN_CREATE, id, -1, (wxObjectEventFunction) (wxEventFunction) (PluginCreateEventFunction) & fn, (wxObject *) NULL ),
}


#ifdef __PREV_WXDLLIMPEXP_CORE
#undef WXDLLIMPEXP_CORE
#define WXDLLIMPEXP_CORE __PREV_WXDLLIMPEXP_CORE
#endif


#endif //__EDUNET_PLAYER_WX_EVENTS_H__
