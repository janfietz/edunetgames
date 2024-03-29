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

#include "OpenSteer/Annotation.h"
#include "OpenSteer/GlobalData.h"


namespace OpenSteer	{

	bool annotationIsOn (void) 
	{
		return GlobalData::getEnableAnnotation();
	}

	void setAnnotationOn (void) 
	{
		GlobalData::setEnableAnnotation( true );
	}
	
	void setAnnotationOff (void) 
	{
		GlobalData::setEnableAnnotation( false );
	}
	
	bool toggleAnnotationState (void) 
	{
		GlobalData::setEnableAnnotation( !annotationIsOn() );
		return annotationIsOn();
	}

	bool drawPhaseActive (void)
	{
		return GlobalData::getDrawPhaseActive();
	}


//////////////////////////////////////////////////////////////////////////
	void DiscAnnotation::draw( AbstractRenderer* pRenderer, bool bDrawPhaseActive )const 
	{
		if (bDrawPhaseActive)
		{
			pRenderer->drawCircleOrDisk (m_radius, m_axis, m_center, m_color,
				m_segments, m_filled, m_b3d);
		}
		else
		{
			pRenderer->deferredDrawCircleOrDisk (m_radius, m_axis, m_center, m_color,
				m_segments, m_filled, m_b3d);
		}
	}
//////////////////////////////////////////////////////////////////////////
	void LineAnnotation::draw( AbstractRenderer* pRenderer, bool bDrawPhaseActive )const 
	{
		if (bDrawPhaseActive)
		{
			pRenderer->drawLine (m_startPoint, m_endPoint, m_color);
		}
		else
		{
			pRenderer->deferredDrawLine (m_startPoint, m_endPoint, m_color);
		}
	}
}