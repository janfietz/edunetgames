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
#include "NetworkPlot.h"

//-----------------------------------------------------------------------------
NetworkPlot::NetworkPlot()
{
	this->m_kBandwith.accessValues(0).setName( "BytesPerSecondSend" );
	this->m_kBandwith.accessValues(0).setName( "BytesPerSecondReceveid" );
}
//-----------------------------------------------------------------------------
NetworkPlot::~NetworkPlot()
{

}
//-----------------------------------------------------------------------------
void NetworkPlot::recordUpdate( RakNetStatistics& kStats,
	const float currentTime, const float elapsedTime )
{

	float fBytesPerSecondSend = (float)(kStats.bitsPerSecondSent / 8.0);
	float fBytesPerSecontReceived = (float)(kStats.bitsPerSecondReceived / 8.0);
	Profile::GraphValues& kBandWidthValuesSend = 
		this->m_kBandwith.accessValues(0);		
	kBandWidthValuesSend.addValue( currentTime, fBytesPerSecondSend);

	Profile::GraphValues& kBandWidthValuesReceive = 
		this->m_kBandwith.accessValues(1);
	kBandWidthValuesReceive.addValue( currentTime+0.01f, fBytesPerSecontReceived);
}
							   
//-----------------------------------------------------------------------------
void NetworkPlot::draw( void ) const
{
	// draw bandwidth state plot
	const float fGraphStart = 220;
	const float fGraphHeight = 175;
	const float fGraphWidth = 400;
	Profile::GraphPlot kPlot;
	Profile::TGraphPointerArray kGraphArray;
	kGraphArray.push_back( &this->m_kBandwith );
	kPlot.draw( kGraphArray, 50, fGraphStart, fGraphWidth, fGraphHeight * kGraphArray.size() );
}