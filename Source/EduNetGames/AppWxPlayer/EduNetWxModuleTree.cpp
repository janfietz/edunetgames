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

#include "EduNetWxModuleTree.h"

#include "icon1.xpm"
#include "icon2.xpm"
#include "icon3.xpm"
#include "icon4.xpm"
#include "icon5.xpm"

#include <wx/debug.h> 

#include "EduNetApplication/EduNetModulePluginLoader.h"
#include "EduNetApplication/EduNetModuleManager.h"
#include "EduNetModule/EduNetPluginFactory.h"

#include "EduNetPlayerWxEvents.h"

namespace EduNet
{
IMPLEMENT_DYNAMIC_CLASS(ModuleTreeCtrl, wxTreeCtrl)

BEGIN_EVENT_TABLE(ModuleTreeCtrl, wxTreeCtrl)
	EVT_TREE_ITEM_MENU(ModuleTreeCtrl_Ctrl, ModuleTreeCtrl::OnItemMenu)
	EVT_TREE_ITEM_RIGHT_CLICK(ModuleTreeCtrl_Ctrl, ModuleTreeCtrl::OnItemRClick)
	EVT_TREE_ITEM_ACTIVATED(ModuleTreeCtrl_Ctrl, ModuleTreeCtrl::OnItemActivated)
END_EVENT_TABLE()

//-----------------------------------------------------------------------------
	ModuleTreeCtrl::ModuleTreeCtrl( ModulePluginLoader* moduleManager,
		wxWindow *parent, const wxPoint& pos, const wxSize& size, long style ):
		wxTreeCtrl(parent, ModuleTreeCtrl_Ctrl, pos, size, style),
		m_moduleManager(moduleManager),
		m_imageSize(16)
	{
		wxASSERT(m_moduleManager != NULL);

		createImageList();
		addItems();
	}

		void ModuleTreeCtrl::createImageList(int size)
		{
			if ( size == -1 )
			{
				SetImageList(NULL);
				return;
			}
			if ( size == 0 )
				size = m_imageSize;
			else
				m_imageSize = size;

			// Make an image list containing small icons
			wxImageList *images = new wxImageList(size, size, true);

			// should correspond to TreeCtrlIcon_xxx enum
			wxBusyCursor wait;
			wxIcon icons[TreeCtrlIcon_Count];

			
			icons[TreeCtrlIcon_Root] = wxIcon(icon1_xpm);
			icons[TreeCtrlIcon_Module] = wxIcon(icon2_xpm);
			icons[TreeCtrlIcon_Plugin] = wxIcon(icon3_xpm);
		
			for ( size_t i = 0; i < WXSIZEOF(icons); i++ )
			{
				int sizeOrig = icons[0].GetWidth();
				if ( size == sizeOrig )
				{
					images->Add(icons[i]);
				}
				else
				{
					images->Add(wxBitmap(wxBitmap(icons[i]).ConvertToImage().Rescale(size, size)));
				}
			}

			AssignImageList(images);
		}
//-----------------------------------------------------------------------------
	void ModuleTreeCtrl::addItems()
	{

		int image = ModuleTreeCtrl::TreeCtrlIcon_Root;
		wxTreeItemId rootId = AddRoot(wxT("Root"),
			image, image,
			new RootItemData(wxT("Root item")));

		const ModuleManager& moduleManager = m_moduleManager->GetModuleManager(); 
		const RawModules& kModule = moduleManager.getModules();

		RawModules::const_iterator modIter = kModule.begin();
		const RawModules::const_iterator modIterEnd = kModule.end();
		while (modIter != modIterEnd)
		{
			const RawModule* module = modIter->get();
			addModuleItem( module, rootId );
			++modIter;
		}
	}
	//-----------------------------------------------------------------------------
	void ModuleTreeCtrl::addModuleItem(const RawModule* const module,
		const wxTreeItemId& idParent)
	{
		ModuleEntry* pkEntry = module->accessEntry();
		if ( NULL != pkEntry )
		{
			PluginFactory* pkFactory = pkEntry->createPluginFactory();			

			enStringArray_t kList;
			pkFactory->getPluginNames ( kList );

			wxString moduleName(pkEntry->getName());
			
			ModuleTreeCtrl::ModuleItemData* moduleItem = new ModuleTreeCtrl::ModuleItemData( moduleName ,moduleName );

			int image = ModuleTreeCtrl::TreeCtrlIcon_Module;
			wxTreeItemId idModule = AppendItem(idParent, moduleName, image, image,
				moduleItem);


			image = ModuleTreeCtrl::TreeCtrlIcon_Plugin;
			enStringArray_t::iterator kNameIter = kList.begin();
			enStringArray_t::iterator kNameIterEnd = kList.end();
			while ( kNameIterEnd != kNameIter )
			{
				wxString pluginName(( *kNameIter ).c_str());

				ModuleTreeCtrl::PluginItemData* moduleItem = new ModuleTreeCtrl::PluginItemData( pluginName, moduleName, pluginName );
				wxTreeItemId id = AppendItem(idModule, pluginName, image, image,
					moduleItem);
				++kNameIter;
			}

			pkEntry->destroyPluginFactory(pkFactory);
		}		
	}
	//-----------------------------------------------------------------------------
	void ModuleTreeCtrl::OnItemMenu(wxTreeEvent& event)
	{
		wxTreeItemId itemId = event.GetItem();
		ModuleTreeCtrl::RootItemData *item = itemId.IsOk() ? (ModuleTreeCtrl::RootItemData *)GetItemData(itemId)
			: NULL;
		wxPoint clientpt = event.GetPoint();
		wxPoint screenpt = ClientToScreen(clientpt);

		wxLogMessage(wxT("OnItemMenu for item \"%s\" at screen coords (%i, %i)"),
			item ? item->GetDesc() : wxT(""), screenpt.x, screenpt.y);

		ShowMenu(itemId, clientpt);
		event.Skip();
	}

	void ModuleTreeCtrl::OnContextMenu(wxContextMenuEvent& event)
	{
		wxPoint pt = event.GetPosition();

		wxLogMessage(wxT("OnContextMenu at screen coords (%i, %i)"), pt.x, pt.y);

		event.Skip();
	}

	//-----------------------------------------------------------------------------
	void ModuleTreeCtrl::OnItemRClick(wxTreeEvent& event)
	{
		wxTreeItemId itemId = event.GetItem();
		ModuleTreeCtrl::RootItemData *item = itemId.IsOk() ? (ModuleTreeCtrl::RootItemData *)GetItemData(itemId)
			: NULL;

		wxLogMessage(wxT("Item \"%s\" right clicked"), item ? item->GetDesc()
			: wxT(""));

		event.Skip();
	}
	
	//-----------------------------------------------------------------------------
	void ModuleTreeCtrl::OnItemActivated(wxTreeEvent& event)
	{
		wxTreeItemId itemId = event.GetItem();
		ModuleTreeCtrl::RootItemData *item = itemId.IsOk() ? (ModuleTreeCtrl::RootItemData *)GetItemData(itemId)
			: NULL;

		if (item->GetType() == ItemData_Plugin)
		{
			ModuleTreeCtrl::PluginItemData *pluginItemData = dynamic_cast<ModuleTreeCtrl::PluginItemData*>( item );
			if (pluginItemData)
			{
				// create a new plugin
				PluginCreateEvent event( pluginItemData->GetModuleName(),
					pluginItemData->GetPluginName(),
					etEVT_COMMAND_PLUGIN_CREATE, GetId());
				event.SetEventObject(this);
				GetEventHandler()->ProcessEvent(event);
			}
		}
				
		event.Skip();
	}


	void ModuleTreeCtrl::ShowMenu(wxTreeItemId id, const wxPoint& pt)
	{
		wxString title;
		if ( id.IsOk() )
		{
			title << wxT("Menu for ") << GetItemText(id);
		}
		else
		{
			title = wxT("Menu for no particular item");
		}

#if wxUSE_MENUS
		wxMenu menu(title);
		menu.Append(ModuleTreeCtrl_About, wxT("&About..."));
		PopupMenu(&menu, pt);
#endif // wxUSE_MENUS
	}

	
}
