///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Sep  8 2010)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "EduNetConnectGui.h"

///////////////////////////////////////////////////////////////////////////

NetworkPluginGui::NetworkPluginGui( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxVERTICAL );
	
	wxStaticBoxSizer* sbSizer1;
	sbSizer1 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Profiles Plots") ), wxVERTICAL );
	
	m_checkBox1 = new wxCheckBox( this, wxID_ANY, wxT("Motion"), wxDefaultPosition, wxDefaultSize, 0 );
	sbSizer1->Add( m_checkBox1, 0, wxALL, 5 );
	
	m_checkBox2 = new wxCheckBox( this, wxID_ANY, wxT("Network"), wxDefaultPosition, wxDefaultSize, 0 );
	sbSizer1->Add( m_checkBox2, 0, wxALL, 5 );
	
	bSizer2->Add( sbSizer1, 1, wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbSizer2;
	sbSizer2 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Connection") ), wxVERTICAL );
	
	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText1 = new wxStaticText( this, wxID_ANY, wxT("Address:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1->Wrap( -1 );
	bSizer3->Add( m_staticText1, 0, wxBOTTOM|wxLEFT|wxTOP, 10 );
	
	m_AddressTxtCtrl = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_AddressTxtCtrl->SetMinSize( wxSize( 200,-1 ) );
	
	bSizer3->Add( m_AddressTxtCtrl, 0, wxALL, 5 );
	
	m_button1 = new wxButton( this, wxID_ANY, wxT("Connect"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer3->Add( m_button1, 0, wxALL, 5 );
	
	sbSizer2->Add( bSizer3, 1, wxEXPAND, 5 );
	
	m_AutoConnectCheckBox = new wxCheckBox( this, wxID_ANY, wxT("Autoconnect"), wxDefaultPosition, wxDefaultSize, 0 );
	sbSizer2->Add( m_AutoConnectCheckBox, 0, wxALL, 5 );
	
	bSizer2->Add( sbSizer2, 1, wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbSizer4;
	sbSizer4 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Network Simulator") ), wxVERTICAL );
	
	m_checkBox4 = new wxCheckBox( this, wxID_ANY, wxT("Enabled"), wxDefaultPosition, wxDefaultSize, 0 );
	sbSizer4->Add( m_checkBox4, 0, wxALL, 5 );
	
	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText2 = new wxStaticText( this, wxID_ANY, wxT("Packet Loss"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText2->Wrap( -1 );
	bSizer4->Add( m_staticText2, 0, wxBOTTOM|wxLEFT|wxTOP, 10 );
	
	m_staticText3 = new wxStaticText( this, wxID_ANY, wxT("0%"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText3->Wrap( -1 );
	bSizer4->Add( m_staticText3, 0, wxBOTTOM|wxLEFT|wxTOP, 10 );
	
	m_PacketLossSlider = new wxSlider( this, wxID_ANY, 0, 0, 100, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL );
	bSizer4->Add( m_PacketLossSlider, 0, wxBOTTOM|wxTOP, 5 );
	
	m_staticText4 = new wxStaticText( this, wxID_ANY, wxT("100%"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText4->Wrap( -1 );
	bSizer4->Add( m_staticText4, 0, wxBOTTOM|wxTOP, 10 );
	
	sbSizer4->Add( bSizer4, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText5 = new wxStaticText( this, wxID_ANY, wxT("Delay"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText5->Wrap( -1 );
	bSizer5->Add( m_staticText5, 0, wxALL, 10 );
	
	m_DelayControl = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 20000, 0 );
	bSizer5->Add( m_DelayControl, 0, wxALL, 5 );
	
	sbSizer4->Add( bSizer5, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer6;
	bSizer6 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText6 = new wxStaticText( this, wxID_ANY, wxT("Delay Variance"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText6->Wrap( -1 );
	bSizer6->Add( m_staticText6, 0, wxALL, 10 );
	
	m_DelayVariance = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 1000, 0 );
	bSizer6->Add( m_DelayVariance, 0, wxALL, 5 );
	
	sbSizer4->Add( bSizer6, 1, wxEXPAND, 5 );
	
	bSizer2->Add( sbSizer4, 1, wxEXPAND, 5 );
	
	this->SetSizer( bSizer2 );
	this->Layout();
	
	// Connect Events
	m_checkBox1->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( NetworkPluginGui::OnCheckedMotionPlot ), NULL, this );
	m_checkBox2->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( NetworkPluginGui::OnCheckedNetworkPlot ), NULL, this );
	m_AddressTxtCtrl->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( NetworkPluginGui::OnAddressChanged ), NULL, this );
	m_AutoConnectCheckBox->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( NetworkPluginGui::OnAutoConnectChecked ), NULL, this );
	m_checkBox4->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( NetworkPluginGui::OnSimEnabledChecked ), NULL, this );
	m_PacketLossSlider->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( NetworkPluginGui::OnPacketLossChanged ), NULL, this );
	m_DelayControl->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( NetworkPluginGui::OnSimDelayChanged ), NULL, this );
	m_DelayVariance->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( NetworkPluginGui::OnSimVarianceChanged ), NULL, this );
}

NetworkPluginGui::~NetworkPluginGui()
{
	// Disconnect Events
	m_checkBox1->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( NetworkPluginGui::OnCheckedMotionPlot ), NULL, this );
	m_checkBox2->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( NetworkPluginGui::OnCheckedNetworkPlot ), NULL, this );
	m_AddressTxtCtrl->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( NetworkPluginGui::OnAddressChanged ), NULL, this );
	m_AutoConnectCheckBox->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( NetworkPluginGui::OnAutoConnectChecked ), NULL, this );
	m_checkBox4->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( NetworkPluginGui::OnSimEnabledChecked ), NULL, this );
	m_PacketLossSlider->Disconnect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( NetworkPluginGui::OnPacketLossChanged ), NULL, this );
	m_DelayControl->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( NetworkPluginGui::OnSimDelayChanged ), NULL, this );
	m_DelayVariance->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( NetworkPluginGui::OnSimVarianceChanged ), NULL, this );
	
}
