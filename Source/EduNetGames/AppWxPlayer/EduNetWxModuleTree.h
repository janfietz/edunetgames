#ifndef __EDUNET_MODULE_TREE_WX_H__
#define __EDUNET_MODULE_TREE_WX_H__
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
#include <wx/treectrl.h>

namespace EduNet
{
	// menu and control ids
	enum
	{
		ModuleTreeCtrl_Quit = wxID_EXIT,
		ModuleTreeCtrl_About = wxID_ABOUT,
		ModuleTreeCtrl_ClearLog = wxID_CLEAR,
		ModuleTreeCtrl_TogButtons = wxID_HIGHEST,
		ModuleTreeCtrl_Ctrl
	};

	class ModuleTreeCtrl : public wxTreeCtrl
	{
	public:
		enum
		{
			TreeCtrlIcon_Root,
			TreeCtrlIcon_Module,
			TreeCtrlIcon_Plugin,
			TreeCtrlIcon_Count,
		};

		ModuleTreeCtrl() {}
		ModuleTreeCtrl(class ModulePluginLoader* moduleManager, wxWindow *parent,
			const wxPoint& pos = wxDefaultPosition,
			const wxSize& size = wxDefaultSize,
			long style = wxTR_DEFAULT_STYLE | wxTR_HIDE_ROOT );
		virtual ~ModuleTreeCtrl(){};

		void OnItemMenu(wxTreeEvent& event);
		void OnContextMenu(wxContextMenuEvent& event);
		void OnItemRClick(wxTreeEvent& event);
		void OnItemActivated(wxTreeEvent& event);
	
	private:
		enum ItemDateType
		{
			ItemData_Root,
			ItemData_Module,
			ItemData_Plugin,
		};
		//-----------------------------------------------------------------------------
		class RootItemData : public wxTreeItemData
		{
		public:
			RootItemData(const wxString& desc ) : m_desc(desc), m_type(ItemData_Root) { }

			void ShowInfo(wxTreeCtrl *tree);
			const wxChar *GetDesc() const { return m_desc.c_str(); }
			ItemDateType GetType() const { return m_type; }
		protected:
			wxString m_desc;
			ItemDateType m_type;
		};
		//-----------------------------------------------------------------------------
		class ModuleItemData : public RootItemData
		{
		public:
			ModuleItemData( const wxString& moduleName, const wxString& desc ): 
			  RootItemData(desc),m_moduleName(moduleName){ m_type = ItemData_Module; }; 
		
			  const wxString& GetModuleName() const{ return m_moduleName; }
		private:
			wxString m_moduleName;
		};
		//-----------------------------------------------------------------------------
		class PluginItemData : public ModuleItemData
		{
		public:
			PluginItemData(const wxString& pluginName,
				const wxString& moduleName, const wxString& desc) :
				ModuleItemData(moduleName, desc ), m_pluginName(pluginName){ m_type = ItemData_Plugin; }

			const wxString& GetPluginName() const{ return m_pluginName; }
		private:
			wxString m_pluginName;
		};

		void addItems();
		void addModuleItem(const class RawModule* const module, const wxTreeItemId& idParent);
		void createImageList(int size = 16);

		ModulePluginLoader* m_moduleManager;

		int m_imageSize;
		
		DECLARE_DYNAMIC_CLASS(ModuleTreeCtrl)
		DECLARE_EVENT_TABLE()
		
		void ShowMenu(wxTreeItemId id, const wxPoint& pt);
		
	};
}

#endif //__EDUNET_MODULE_TREE_WX_H__
