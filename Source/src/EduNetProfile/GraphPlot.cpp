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

#include "GraphPlot.h"
#include "EduNetCommon/EduNetDraw.h"
#include "EduNetCommon/EduNetMath.h"
#include "OpenSteer/Draw.h"

using namespace Profile;

const Profile::TGraphValue Profile::TGraphValue::ms_Min( -FLT_MAX, -FLT_MAX, -FLT_MAX );
const Profile::TGraphValue Profile::TGraphValue::ms_Max( FLT_MAX, FLT_MAX, FLT_MAX );


//-----------------------------------------------------------------------------
GraphPlot::GraphPlot( void )
{

}

//-----------------------------------------------------------------------------
GraphPlot::~GraphPlot()
{

}

//-----------------------------------------------------------------------------
void GraphPlot::setGraphColor( size_t uiId )
{
	if (uiId == 0)
	{
		glColor4f(1,1,1,0.5);
	}
	else 
	{
		uiId = ( uiId >> 8) + uiId;
		GLubyte r = static_cast<GLubyte>( uiId * 37 );
		GLubyte g = static_cast<GLubyte>( uiId * 59 );
		GLubyte b = static_cast<GLubyte>( uiId * 45 );
		glColor3ub((r & 127) + 80, (g & 127) + 80, (b & 127) + 80);
	}
}

//-----------------------------------------------------------------------------
void GraphPlot::drawRectangle(float x0, float y0, float x1, float y1)
{
	glBegin(GL_LINE_STRIP);
	glVertex2f(x0, y0);
	glVertex2f(x1, y0);
	glVertex2f(x1, y1);
	glVertex2f(x0, y1);
	glVertex2f(x0, y0);
	glEnd();
}

//-----------------------------------------------------------------------------
void GraphPlot::drawQuad(float x0, float y0, float x1, float y1)
{
	glBegin(GL_QUADS);
	glVertex2f(x0, y0);
	glVertex2f(x1, y0);
	glVertex2f(x1, y1);
	glVertex2f(x0, y1);
	glEnd();
}

//-----------------------------------------------------------------------------
void GraphPlot::draw( const GraphValues& kValues, float sx, float sy, float width, float height ) const
{
	if( kValues.size() < 2 )
	{
		return;
	}

	GraphLocation kGraphLocation;
	kGraphLocation.sx = sx;
	kGraphLocation.sy = sy;
	kGraphLocation.width = width;
	kGraphLocation.height = height;
	GraphValue& kMin = kGraphLocation.kMin;
	GraphValue& kMax = kGraphLocation.kMax;
	GraphValue& kInterval = kGraphLocation.kInterval;
	GraphValue& kScale = kGraphLocation.kScale;
	kMin = GraphValue::ms_Max;
	kMax = GraphValue::ms_Min;
	kInterval = GraphValue::ms_Min;
	kScale = GraphValue::ms_Min;

	this->computeGraphLocation( kValues, kGraphLocation );

	if( kInterval.x > 0 )
	{
		float sw( OpenSteer::drawGetWindowHeight() ), 
			sh( OpenSteer::drawGetWindowWidth() );
		const GLint originalMatrixMode = OpenSteer::begin2dDrawing (sw, sh);
		this->drawGraphFrame( kGraphLocation );
		OpenSteer::end2dDrawing (originalMatrixMode);

		this->drawSingleGraph( kValues, kGraphLocation );
	}
}

//-----------------------------------------------------------------------------
void GraphPlot::draw( const GraphValuesArray& kValues, float sx, float sy, float width, float height )
{
	if( kValues.size() == 0 )
	{
		return;
	}

	GraphLocation kGraphLocation;
	kGraphLocation.sx = sx;
	kGraphLocation.sy = sy;
	kGraphLocation.width = width;
	kGraphLocation.height = height;
	GraphValue& kMin = kGraphLocation.kMin;
	GraphValue& kMax = kGraphLocation.kMax;
	GraphValue& kInterval = kGraphLocation.kInterval;
	GraphValue& kScale = kGraphLocation.kScale;
	kMin = GraphValue::ms_Max;
	kMax = GraphValue::ms_Min;
	kInterval = GraphValue::ms_Min;
	kScale = GraphValue::ms_Min;

	TGraphValuesArray::const_iterator kIter = kValues.begin();
	TGraphValuesArray::const_iterator kEnd = kValues.end();
	// iterate once to determine min max values
	while( kIter != kEnd )
	{
		const GraphValues& kGraphValues = *kIter;
		this->computeGraphLocation( kGraphValues, kGraphLocation );
		++kIter;
	}

	if( kInterval.x > 0 )
	{
		float sw( OpenSteer::drawGetWindowHeight() ), 
			sh( OpenSteer::drawGetWindowWidth() );
		const GLint originalMatrixMode = OpenSteer::begin2dDrawing (sw, sh);
		this->drawGraphFrame( kGraphLocation );
		OpenSteer::end2dDrawing (originalMatrixMode);

		kIter = kValues.begin();
		while( kIter != kEnd )
		{
			const GraphValues& kGraphValues = *kIter;
			this->drawSingleGraph( kGraphValues, kGraphLocation );
			++kIter;
		}

	}
}

//-----------------------------------------------------------------------------
void GraphPlot::drawGraphFrame( const GraphLocation& kGraphLocation ) const
{
	const GraphValue& kMin = kGraphLocation.kMin;
	const GraphValue& kScale = kGraphLocation.kScale;

	// graph border
	glColor3ub(0, 0, 0);
	GraphPlot::drawRectangle( 
		kGraphLocation.sx, 
		kGraphLocation.sy, 
		kGraphLocation.sx + kGraphLocation.width, 
		kGraphLocation.sy + kGraphLocation.height );

	if( kMin.y < 0 )
	{
		// baseline
		GraphValue kRenderValue( kGraphLocation.sx, kGraphLocation.sy - ( kMin.y * kScale.y ), 0 );
		glBegin(GL_LINE_STRIP);
		glVertex2f(kRenderValue.x,kRenderValue.y);
		kRenderValue.x += kGraphLocation.width;
		glVertex2f(kRenderValue.x,kRenderValue.y);
		glEnd();
	}
}

//-----------------------------------------------------------------------------
void GraphPlot::drawSingleGraph( const GraphValues& kValues, const GraphLocation& kGraphLocation ) const
{
	if( kValues.size() == 0 )
	{
		return;
	}

	const GraphValue& kMinDraw = kGraphLocation.kMinDraw;
	const GraphValue& kMin = kGraphLocation.kMin;
	const GraphValue& kMax = kGraphLocation.kMax;
	const GraphValue& kInterval = kGraphLocation.kInterval;
	const GraphValue& kScale = kGraphLocation.kScale;

	if( kInterval.x > 0 )
	{
		const GraphValue& kEndValue = kValues[kValues.size() - 1];

		const float sw( OpenSteer::drawGetWindowHeight() ), 
			sh( OpenSteer::drawGetWindowWidth() );
		const char* pszName = kValues.getName();
		if( NULL != pszName )
		{
			std::ostringstream sn;
			sn << std::setprecision(2);
			sn << pszName
				<< " Min: " << kMin.y
				<< " Max: " << kMax.y
				<< " Current: " << kEndValue.y
				<< " Interval: " << kInterval.y
				<< std::ends;

			osVector3 screenLocation( kGraphLocation.sx, kGraphLocation.sy - 23, 0 );
			OpenSteer::Color kColor = OpenSteer::gGray80;
			OpenSteer::draw2dTextAt2dLocation( sn, screenLocation, kColor, sw, sh);
		}

		const GLint originalMatrixMode = OpenSteer::begin2dDrawing (sw, sh);
		GraphPlot::setGraphColor( kValues.getId() );
		glBegin(GL_LINE_STRIP);

		GraphValue kRenderValue;
		TGraphValues::const_iterator kIter = kValues.begin();
		TGraphValues::const_iterator kEnd = kValues.end();
		const GraphValue& kStartValue = (*kIter);
		while( kIter != kEnd )
		{
			kRenderValue = (*kIter);
			// force the graph to begin at sx
			kRenderValue.x -= kStartValue.x;
			kRenderValue.x *= kScale.x;
			kRenderValue.x += kGraphLocation.sx;

			// offset the graph to begin at sy
			kRenderValue.y -= kMinDraw.y;
			kRenderValue.y *= kScale.y;
			kRenderValue.y += kGraphLocation.sy;

			glVertex2f(kRenderValue.x,kRenderValue.y);
			++kIter;
		}
		glEnd();
		OpenSteer::end2dDrawing (originalMatrixMode);
	}

}


//-----------------------------------------------------------------------------
void GraphPlot::computeGraphLocation( const GraphValues& kValues, GraphLocation& kGraphLocation ) const
{
	TGraphValues::const_iterator kIter = kValues.begin();
	TGraphValues::const_iterator kEnd = kValues.end();
	if( kIter == kEnd )
	{
		return;
	}

	GraphValue& kMin = kGraphLocation.kMin;
	GraphValue& kMax = kGraphLocation.kMax;
	GraphValue& kMinDraw = kGraphLocation.kMinDraw;
	GraphValue& kMaxDraw = kGraphLocation.kMaxDraw;
	GraphValue& kInterval = kGraphLocation.kInterval;
	GraphValue& kIntervalDraw = kGraphLocation.kIntervalDraw;
	GraphValue& kScale = kGraphLocation.kScale;

	// iterate once to determine min max values
	while( kIter != kEnd )
	{
		const GraphValue& kRenderValue = (*kIter);
		kMin.x = ::etMin( kMin.x, kRenderValue.x );
		kMin.y = ::etMin( kMin.y, kRenderValue.y );
		kMin.z = ::etMin( kMin.z, kRenderValue.z );
		kMax.x = ::etMax( kMax.x, kRenderValue.x );
		kMax.y = ::etMax( kMax.y, kRenderValue.y );
		kMax.z = ::etMax( kMax.z, kRenderValue.z );
		++kIter;
	}

	kMinDraw = kMin;
	kMaxDraw = kMax;

	// get a zero base for positive values
	if( kMinDraw.y >= 0 )
	{
		kMinDraw.y = 0;
		kMaxDraw.y *= 1.25f;
	}
	else
	{
		if( kMaxDraw.y < 0 )
		{
			kMaxDraw.y = 0;
			kMinDraw.y *= 1.25f;
		}
		else
		{
		}
	}

	kIntervalDraw.x = ::etInterval( kMinDraw.x, kMaxDraw.x );
	kIntervalDraw.y = ::etInterval( kMinDraw.y, kMaxDraw.y );
	kIntervalDraw.z = ::etInterval( kMinDraw.z, kMaxDraw.z );

	kInterval.x = ::etInterval( kMin.x, kMax.x );
	kInterval.y = ::etInterval( kMin.y, kMax.y );
	kInterval.z = ::etInterval( kMin.z, kMax.z );

	if( kIntervalDraw.x > 0 )
	{
		kScale.x = kGraphLocation.width / kIntervalDraw.x;
	}
	if( kIntervalDraw.y > 0 )
	{
		kScale.y = kGraphLocation.height / kIntervalDraw.y;
	}

}
