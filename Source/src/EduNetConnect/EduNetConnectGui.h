///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Sep  8 2010)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __EduNetConnectGui__
#define __EduNetConnectGui__

#include <wx/string.h>
#include <wx/checkbox.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/sizer.h>
#include <wx/statbox.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/button.h>
#include <wx/slider.h>
#include <wx/spinctrl.h>
#include <wx/panel.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class NetworkPluginGui
///////////////////////////////////////////////////////////////////////////////
class NetworkPluginGui : public wxPanel 
{
	private:
	
	protected:
		wxCheckBox* m_checkBox1;
		wxCheckBox* m_checkBox2;
		wxStaticText* m_staticText1;
		wxTextCtrl* m_AddressTxtCtrl;
		wxButton* m_button1;
		wxCheckBox* m_AutoConnectCheckBox;
		wxCheckBox* m_checkBox4;
		wxStaticText* m_staticText2;
		wxStaticText* m_staticText3;
		wxSlider* m_PacketLossSlider;
		wxStaticText* m_staticText4;
		wxStaticText* m_staticText5;
		wxSpinCtrl* m_DelayControl;
		wxStaticText* m_staticText6;
		wxSpinCtrl* m_DelayVariance;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnCheckedMotionPlot( wxCommandEvent& event ) = 0;
		virtual void OnCheckedNetworkPlot( wxCommandEvent& event ) = 0;
		virtual void OnAddressChanged( wxCommandEvent& event ) = 0;
		virtual void OnAutoConnectChecked( wxCommandEvent& event ) = 0;
		virtual void OnSimEnabledChecked( wxCommandEvent& event ) = 0;
		virtual void OnPacketLossChanged( wxScrollEvent& event ) = 0;
		virtual void OnSimDelayChanged( wxSpinEvent& event ) = 0;
		virtual void OnSimVarianceChanged( wxSpinEvent& event ) = 0;
		
	
	public:
		
		NetworkPluginGui( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,500 ), long style = wxTAB_TRAVERSAL );
		~NetworkPluginGui();
	
};

#endif //__EduNetConnectGui__
