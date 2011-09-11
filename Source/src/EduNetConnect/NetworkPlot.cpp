//-----------------------------------------------------------------------------
// Copyright (c) 2011, Jan Fietz, Cyrus Preuss
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
#include "NetworkPlot.h"
#include "OpenSteer/Plugin.h"

//-----------------------------------------------------------------------------
NetworkPlot::NetworkPlot()
{
	this->m_kBandwith.accessValues(0).
		setName( "Bytes/sec Send    " ).
		setColor( 0.75f, 0.25, 0.25 );
	this->m_kBandwith.accessValues(1).
		setName( "Bytes/sec Received" ).
		setColor( 0.25f, 0.75, 0.25 );
}

//-----------------------------------------------------------------------------
NetworkPlot::~NetworkPlot()
{

}

//-----------------------------------------------------------------------------
void NetworkPlot::recordUpdate( RakNetStatistics& kStats,
	const float currentTime, const float elapsedTime )
{
	float fBytesPerSecondSend = (float)(kStats.valueOverLastSecond[ACTUAL_BYTES_SENT]);
	float fBytesPerSecondReceived = (float)(kStats.valueOverLastSecond[ACTUAL_BYTES_RECEIVED]);
	Profile::GraphValues& kBandWidthValuesSend = 
		this->m_kBandwith.accessValues(0);
	kBandWidthValuesSend.addValue( currentTime, fBytesPerSecondSend);

	Profile::GraphValues& kBandWidthValuesReceive = 
		this->m_kBandwith.accessValues(1);
	kBandWidthValuesReceive.addValue( currentTime, fBytesPerSecondReceived);
}
							   
//-----------------------------------------------------------------------------
void NetworkPlot::draw( osAbstractPlugin* pkNetworkPlugin ) const
{
	osAbstractEntity* pkPluginEntity = dynamic_cast<osAbstractEntity*>(pkNetworkPlugin);
	// draw bandwidth state plot
	float fGraphStart = 50;
	const float fGraphHeight = 200;
	const float fGraphWidth = 400;
	if( NULL != pkPluginEntity && pkPluginEntity->isRemoteObject() )
	{
		fGraphStart += fGraphHeight;
	}
	Profile::GraphPlot kPlot;
	Profile::TGraphPointerArray kGraphArray;
	kGraphArray.push_back( &this->m_kBandwith );
	kGraphArray.m_bWireFrame = true;
	kPlot.draw( kGraphArray, 50, fGraphStart, fGraphWidth, fGraphHeight * kGraphArray.size() );
}