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
	
	wxGridBagSizer* gbSizer1;
	gbSizer1 = new wxGridBagSizer( 0, 0 );
	gbSizer1->SetFlexibleDirection( wxBOTH );
	gbSizer1->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText1 = new wxStaticText( this, wxID_ANY, wxT("Address:"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT );
	m_staticText1->Wrap( -1 );
	gbSizer1->Add( m_staticText1, wxGBPosition( 0, 1 ), wxGBSpan( 1, 1 ), wxALIGN_RIGHT|wxALL, 10 );
	
	m_AddressTxtCtrl = new wxTextCtrl( this, etNet_Address, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	m_AddressTxtCtrl->SetMinSize( wxSize( 200,-1 ) );
	
	gbSizer1->Add( m_AddressTxtCtrl, wxGBPosition( 0, 2 ), wxGBSpan( 1, 1 ), wxALL, 5 );
	
	m_staticText11 = new wxStaticText( this, wxID_ANY, wxT("Port:"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT );
	m_staticText11->Wrap( -1 );
	gbSizer1->Add( m_staticText11, wxGBPosition( 1, 1 ), wxGBSpan( 1, 1 ), wxALIGN_RIGHT|wxALL, 10 );
	
	m_PortTxtCtrl = new wxTextCtrl( this, etNet_Port, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	m_PortTxtCtrl->SetMinSize( wxSize( 100,-1 ) );
	
	gbSizer1->Add( m_PortTxtCtrl, wxGBPosition( 1, 2 ), wxGBSpan( 1, 1 ), wxALL, 5 );
	
	sbSizer2->Add( gbSizer1, 1, 0, 5 );
	
	wxBoxSizer* bSizer11;
	bSizer11 = new wxBoxSizer( wxHORIZONTAL );
	
	m_AutoConnectCheckBox = new wxCheckBox( this, wxID_ANY, wxT("Autoconnect"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer11->Add( m_AutoConnectCheckBox, 0, wxALL, 10 );
	
	m_button1 = new wxButton( this, wxID_ANY, wxT("Connect"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer11->Add( m_button1, 0, wxALL, 5 );
	
	sbSizer2->Add( bSizer11, 1, 0, 5 );
	
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
	m_PortTxtCtrl->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( NetworkPluginGui::OnAddressChanged ), NULL, this );
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
	m_PortTxtCtrl->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( NetworkPluginGui::OnAddressChanged ), NULL, this );
	m_AutoConnectCheckBox->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( NetworkPluginGui::OnAutoConnectChecked ), NULL, this );
	m_checkBox4->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( NetworkPluginGui::OnSimEnabledChecked ), NULL, this );
	m_PacketLossSlider->Disconnect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( NetworkPluginGui::OnPacketLossChanged ), NULL, this );
	m_DelayControl->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( NetworkPluginGui::OnSimDelayChanged ), NULL, this );
	m_DelayVariance->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( NetworkPluginGui::OnSimVarianceChanged ), NULL, this );
	
}

ReplicationSettings::ReplicationSettings( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
	wxStaticBoxSizer* sbSizer4;
	sbSizer4 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Replication Settings") ), wxVERTICAL );
	
	wxBoxSizer* bSizer6;
	bSizer6 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText7 = new wxStaticText( this, wxID_ANY, wxT("Rate  0.1Hz"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText7->Wrap( -1 );
	bSizer6->Add( m_staticText7, 0, wxALL, 10 );
	
	m_replicationRate = new wxSlider( this, wxID_ANY, 50, 1, 1000, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL );
	bSizer6->Add( m_replicationRate, 0, wxALL, 5 );
	
	m_staticText8 = new wxStaticText( this, wxID_ANY, wxT("100Hz"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText8->Wrap( -1 );
	bSizer6->Add( m_staticText8, 0, wxALL, 10 );
	
	sbSizer4->Add( bSizer6, 1, wxALIGN_LEFT|wxALIGN_TOP|wxSHAPED, 5 );
	
	wxBoxSizer* bSizer8;
	bSizer8 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText9 = new wxStaticText( this, wxID_ANY, wxT("Default Reliability"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText9->Wrap( -1 );
	bSizer8->Add( m_staticText9, 0, wxALL, 10 );
	
	wxString m_ReplicationReliabilityChoices[] = { wxT("Unreliable"), wxT("Unreliable Sequenced"), wxT("Reliable"), wxT("Reliable Ordered"), wxT("Reliable Sequenced") };
	int m_ReplicationReliabilityNChoices = sizeof( m_ReplicationReliabilityChoices ) / sizeof( wxString );
	m_ReplicationReliability = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_ReplicationReliabilityNChoices, m_ReplicationReliabilityChoices, 0 );
	m_ReplicationReliability->SetSelection( 0 );
	bSizer8->Add( m_ReplicationReliability, 0, wxALL, 5 );
	
	sbSizer4->Add( bSizer8, 1, wxALIGN_LEFT|wxALIGN_TOP|wxSHAPED, 5 );
	
	wxBoxSizer* bSizer81;
	bSizer81 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText91 = new wxStaticText( this, wxID_ANY, wxT("Default Priority"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText91->Wrap( -1 );
	bSizer81->Add( m_staticText91, 0, wxALL, 10 );
	
	wxString m_ReplicationPriorityChoices[] = { wxT("Low"), wxT("Medium"), wxT("High") };
	int m_ReplicationPriorityNChoices = sizeof( m_ReplicationPriorityChoices ) / sizeof( wxString );
	m_ReplicationPriority = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_ReplicationPriorityNChoices, m_ReplicationPriorityChoices, 0 );
	m_ReplicationPriority->SetSelection( 0 );
	bSizer81->Add( m_ReplicationPriority, 0, wxALL, 5 );
	
	sbSizer4->Add( bSizer81, 1, wxEXPAND, 5 );
	
	this->SetSizer( sbSizer4 );
	this->Layout();
	
	// Connect Events
	m_replicationRate->Connect( wxEVT_SCROLL_TOP, wxScrollEventHandler( ReplicationSettings::OnReplicationRateChanged ), NULL, this );
	m_replicationRate->Connect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( ReplicationSettings::OnReplicationRateChanged ), NULL, this );
	m_replicationRate->Connect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( ReplicationSettings::OnReplicationRateChanged ), NULL, this );
	m_replicationRate->Connect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( ReplicationSettings::OnReplicationRateChanged ), NULL, this );
	m_replicationRate->Connect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( ReplicationSettings::OnReplicationRateChanged ), NULL, this );
	m_replicationRate->Connect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( ReplicationSettings::OnReplicationRateChanged ), NULL, this );
	m_replicationRate->Connect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( ReplicationSettings::OnReplicationRateChanged ), NULL, this );
	m_replicationRate->Connect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( ReplicationSettings::OnReplicationRateChanged ), NULL, this );
	m_replicationRate->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( ReplicationSettings::OnReplicationRateChanged ), NULL, this );
	m_ReplicationReliability->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( ReplicationSettings::OnReliabilityChanged ), NULL, this );
	m_ReplicationPriority->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( ReplicationSettings::OnPriorityChanged ), NULL, this );
}

ReplicationSettings::~ReplicationSettings()
{
	// Disconnect Events
	m_replicationRate->Disconnect( wxEVT_SCROLL_TOP, wxScrollEventHandler( ReplicationSettings::OnReplicationRateChanged ), NULL, this );
	m_replicationRate->Disconnect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( ReplicationSettings::OnReplicationRateChanged ), NULL, this );
	m_replicationRate->Disconnect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( ReplicationSettings::OnReplicationRateChanged ), NULL, this );
	m_replicationRate->Disconnect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( ReplicationSettings::OnReplicationRateChanged ), NULL, this );
	m_replicationRate->Disconnect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( ReplicationSettings::OnReplicationRateChanged ), NULL, this );
	m_replicationRate->Disconnect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( ReplicationSettings::OnReplicationRateChanged ), NULL, this );
	m_replicationRate->Disconnect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( ReplicationSettings::OnReplicationRateChanged ), NULL, this );
	m_replicationRate->Disconnect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( ReplicationSettings::OnReplicationRateChanged ), NULL, this );
	m_replicationRate->Disconnect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( ReplicationSettings::OnReplicationRateChanged ), NULL, this );
	m_ReplicationReliability->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( ReplicationSettings::OnReliabilityChanged ), NULL, this );
	m_ReplicationPriority->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( ReplicationSettings::OnPriorityChanged ), NULL, this );
	
}
