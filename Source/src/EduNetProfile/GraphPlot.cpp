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
#include "OpenSteer/Renderer.h"

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
void GraphPlot::setGraphColor( const GraphValues& kValues )
{
	if( kValues.hasColor() )
	{
		const float* pColor = kValues.getColor();
		glColor4f( pColor[0], pColor[1], pColor[2], pColor[3] );
	}
	else
	{
		GraphPlot::setGraphColor( kValues.getId() );
	}
}

//-----------------------------------------------------------------------------
void GraphPlot::setGraphColor( size_t uiId, OpenSteer::Color* pkColor )
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
		if( NULL != pkColor )
		{
			pkColor->setR( ((r & 127) + 80) / 255.0f );
			pkColor->setG( ((g & 127) + 80) / 255.0f );
			pkColor->setB( ((b & 127) + 80) / 255.0f );
			pkColor->setA( 1.0f );
		}
		else
		{
			glColor3ub((r & 127) + 80, (g & 127) + 80, (b & 127) + 80);
		}
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
	glBegin( GL_QUADS );
	glVertex2f( x0, y0 );
	glVertex2f( x1, y0 );
	glVertex2f( x1, y1 );
	glVertex2f( x0, y1 );
	glEnd();
}

//-----------------------------------------------------------------------------
void GraphPlot::draw( const GraphLocation& kMasterGraphLocation, const GraphValues& kValues, 
		  float sx, float sy, float width, float height ) const
{
	if( kValues.size() < 2 )
	{
		return;
	}

	GraphLocation kGraphLocation = kMasterGraphLocation;
	kGraphLocation.sx = sx;
	kGraphLocation.sy = sy;
	kGraphLocation.width = width;
	kGraphLocation.height = height;
	GraphValue& kMin = kGraphLocation.kMin;
	GraphValue& kMax = kGraphLocation.kMax;
	GraphValue& kInterval = kGraphLocation.kInterval;
	GraphValue& kScale = kGraphLocation.kScale;
/*
	kMin = GraphValue::ms_Max;
	kMax = GraphValue::ms_Min;
	kInterval = GraphValue::ms_Min;
	kScale = GraphValue::ms_Min;

	this->computeGraphLocation( kValues, kGraphLocation );
*/
	if( kInterval.x > 0 )
	{
		const float sw( OpenSteer::drawGetWindowWidth() ), 
			sh( OpenSteer::drawGetWindowHeight() );
		const GLint originalMatrixMode = OpenSteer::begin2dDrawing (sw, sh);
		this->drawGraphFrame( kGraphLocation );
		OpenSteer::end2dDrawing (originalMatrixMode);

		this->drawSingleGraph( kValues, kGraphLocation );
	}
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
	kScale = GraphValue::ms_Max;

	this->computeGraphLocation( kValues, kGraphLocation );

	if( kInterval.x > 0 )
	{
		const float sw( OpenSteer::drawGetWindowWidth() ), 
			sh( OpenSteer::drawGetWindowHeight() );
		const GLint originalMatrixMode = OpenSteer::begin2dDrawing (sw, sh);
		this->drawGraphFrame( kGraphLocation );
		OpenSteer::end2dDrawing (originalMatrixMode);

		this->drawSingleGraph( kValues, kGraphLocation );
	}
}

//-----------------------------------------------------------------------------
void GraphPlot::draw( const GraphValuesArray& kValues, float sx, float sy, float width, float height ) const
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
//	kScale = GraphValue::ms_Min;
	kScale = GraphValue::ms_Max;
	kGraphLocation.m_bWireFrame = kValues.m_bWireFrame;

	TGraphValuesArray::const_iterator kIter = kValues.begin();
	TGraphValuesArray::const_iterator kEnd = kValues.end();
	// iterate once to determine min max values
	while( kIter != kEnd )
	{
		const GraphValues& kGraphValues = *kIter;
		const size_t i = kGraphValues.size();
		this->computeGraphLocation( kGraphValues, kGraphLocation );
		++kIter;
	}

//	if( kInterval.x > 0 )
	{
		const float sw( OpenSteer::drawGetWindowWidth() ), 
			sh( OpenSteer::drawGetWindowHeight() );

		kIter = kValues.begin();
		while( kIter != kEnd )
		{
			const GraphValues& kGraphValues = *kIter;
			this->drawSingleGraph( kGraphValues, kGraphLocation );
			kGraphLocation.fGraphIndex += 1.0f;
			++kIter;
		}
		const GLint originalMatrixMode = OpenSteer::begin2dDrawing (sw, sh);
		this->drawGraphFrame( kGraphLocation, kGraphLocation.m_bWireFrame );
		OpenSteer::end2dDrawing (originalMatrixMode);
	}
}

//-----------------------------------------------------------------------------
void GraphPlot::draw( const TGraphPointerArray& kValues, float sx, float sy, float width, float height ) const
{
	if( kValues.size() == 0 )
	{
		return;
	}

	GraphLocation kGraphLocation;
	kGraphLocation.sx = sx;
	kGraphLocation.sy = sy;
	kGraphLocation.width = width;
	kGraphLocation.height = height /= kValues.size();

	float fGraphOffSet = kGraphLocation.height;

	kGraphLocation.sy += 20;
	kGraphLocation.height -= 20;

	TGraphPointerArray::const_iterator kIter = kValues.begin();
	TGraphPointerArray::const_iterator kEnd = kValues.end();
	while( kIter != kEnd )
	{
		const GraphValuesArray* pkGraphValues = *kIter;
		if( NULL != pkGraphValues )
		{
			pkGraphValues->m_bWireFrame = kValues.m_bWireFrame;
			GraphValue& kMin = kGraphLocation.kMin;
			GraphValue& kMax = kGraphLocation.kMax;
			GraphValue& kInterval = kGraphLocation.kInterval;
			GraphValue& kScale = kGraphLocation.kScale;
			kMin = GraphValue::ms_Max;
			kMax = GraphValue::ms_Min;
			kInterval = GraphValue::ms_Min;
			kScale = GraphValue::ms_Min;

			this->draw( *pkGraphValues, kGraphLocation.sx, kGraphLocation.sy, kGraphLocation.width, kGraphLocation.height );

			kGraphLocation.sy += fGraphOffSet;
		}
		++kIter;
	}
}

//-----------------------------------------------------------------------------
void GraphPlot::drawGraphFrame( float sx, float sy, float width, float height, bool bRectangle ) const
{
	const float sw( OpenSteer::drawGetWindowWidth() ), 
		sh( OpenSteer::drawGetWindowHeight() );
	const GLint originalMatrixMode = OpenSteer::begin2dDrawing (sw, sh);
	GraphLocation kGraphLocation;
	kGraphLocation.sx = sx;
	kGraphLocation.sy = sy;
	kGraphLocation.width = width;
	kGraphLocation.height = height;
	kGraphLocation.kMin.y = 0;
	this->drawGraphFrame( kGraphLocation, bRectangle );
	OpenSteer::end2dDrawing (originalMatrixMode);
}

//-----------------------------------------------------------------------------
void GraphPlot::drawGraphFrame( const GraphLocation& kGraphLocation, bool bRectangle ) const
{
	const GraphValue& kMin = kGraphLocation.kMin;
	const GraphValue& kScale = kGraphLocation.kScale;

	// graph border
	if( true == bRectangle )
	{
		glColor3ub(0, 0, 0);
		GraphPlot::drawRectangle( 
			kGraphLocation.sx, 
			kGraphLocation.sy, 
			kGraphLocation.sx + kGraphLocation.width, 
			kGraphLocation.sy + kGraphLocation.height );
	}
	else
	{
		OpenSteer::Color kColor( 0.2f, 0.2f, 0.2f, 0.5f );
		glColor4f( kColor.r(), kColor.g(), kColor.b(), kColor.a() );
		GraphPlot::drawQuad( 
			kGraphLocation.sx, 
			kGraphLocation.sy, 
			kGraphLocation.sx + kGraphLocation.width, 
			kGraphLocation.sy + kGraphLocation.height );
	}

	if( kMin.y < 0 )
	{
		// baseline
		GraphValue kRenderValue( kGraphLocation.sx, kGraphLocation.sy - ( kMin.y * kScale.y ), 0 );
		glColor3ub(0, 0, 0);
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
	const GraphValue& kMin = kValues.m_kMin; //kGraphLocation.kMin;
	const GraphValue& kMax = kValues.m_kMax; //kGraphLocation.kMax;
	const GraphValue& kInterval = kGraphLocation.kInterval;
	//const 
	GraphValue kScale = kValues.m_kScale;//kGraphLocation.kScale;
 	kScale.y = kGraphLocation.kScale.y;

	const GraphValue& kEndValue = kValues[kValues.size() - 1];

	OpenSteer::Color kColor = OpenSteer::gGray80;
	if( kValues.hasColor() )
	{
		const float* pColor = kValues.getColor();
		kColor.setR( pColor[0] );
		kColor.setG( pColor[1] );
		kColor.setB( pColor[2] );
		kColor.setA( pColor[3] );
	}
	else
	{
		GraphPlot::setGraphColor( kValues.getId(), &kColor );
	}

	const float sw( OpenSteer::drawGetWindowWidth() ), 
		sh( OpenSteer::drawGetWindowHeight() );
	const char* pszName = kValues.getName();
	if( NULL != pszName )
	{
		TGraphValues::const_iterator kIter = kValues.begin();
		TGraphValues::const_iterator kEnd = kValues.end();
		float fAvg = 0;
		while( kIter != kEnd )
		{
			const GraphValue& kValue = (*kIter);
			fAvg += kValue.y;
			++kIter;
		}
		if( kValues.size() )
		{
			fAvg /= kValues.size();
		}

		std::ostringstream sn;
		sn << pszName;
//		sn	<< "(" << kValues.size() << ")";
		sn << std::setprecision(2) << std::setiosflags(std::ios::fixed);
		const bool bLegendXY = false;
		if( true == bLegendXY )
		{
			sn	<< " X/Y: " << kInterval.x << ";"  << kInterval.y;
			sn	<< " Min: " << kMin.x << ";" << kMin.y;
			sn	<< " Max: " << kMax.x << ";"  << kMax.y;
			sn	<< " Avg: " << fAvg;
			sn	<< " Cur: " << kEndValue.x << ";"  << kEndValue.y;
		}
		else
		{
			sn	<< " X: " << kInterval.x;
			sn	<< " Min: " << kMin.y;
			sn	<< " Max: " << kMax.y;
			sn	<< " Avg: " << fAvg;
			sn	<< " Cur: " << kEndValue.y;
//			sn	<< " Scale: " << kScale.y;
		}
		sn	<< std::ends;

		// GLUT_BITMAP_8_BY_13
		const float fTextHeight = 16;
		osVector3 screenLocation( 
			kGraphLocation.sx, 
			kGraphLocation.sy - ( fTextHeight * ( 1 - kGraphLocation.fGraphIndex ) ), 
			0 );

		screenLocation.y += 3;
		OpenSteer::draw2dTextAt2dLocation( sn, screenLocation, kColor, sw, sh);
	}

	if( kInterval.x > 0 )
	{
		const GLint originalMatrixMode = OpenSteer::begin2dDrawing (sw, sh);
		GraphPlot::setGraphColor( kValues );

		switch( kValues.getGraphType() )
		{
		case(EGraphType_Lines):
			{
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
					kRenderValue.y *= kRenderValue.scale_y;
					kRenderValue.y += kGraphLocation.sy;

					glVertex2f(kRenderValue.x,kRenderValue.y);
					++kIter;
				}
				glEnd();
			}
			break;
		case(EGraphType_Dots):
			{
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

					if( kRenderValue.y == 0.0f )
					{

					}
					else
					{
						// offset the graph to begin at sy
						kRenderValue.y -= kMinDraw.y;
						kRenderValue.y *= kScale.y;
						kRenderValue.y *= kRenderValue.scale_y;
						kRenderValue.y += kGraphLocation.sy;

						const float fDotSize = 2.0f;
						GraphPlot::drawQuad(
							kRenderValue.x - fDotSize, kRenderValue.y - fDotSize, 
							kRenderValue.x + fDotSize, kRenderValue.y + fDotSize 
							);
					}

					++kIter;
				}
			}
			break;
		}
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
	GraphValue& kScale = kValues.m_kScale; //kGraphLocation.kScale;
	GraphValue& kGraphLocationScale = kGraphLocation.kScale;

	const bool bGlobalStats = false;
	if( true == bGlobalStats )
	{
		kMin.y = ::etMin( kMin.y, kValues.m_kMin.y );
		kMin.z = ::etMin( kMin.z, kValues.m_kMin.z );
		kMax.y = ::etMax( kMax.y, kValues.m_kMax.y );
		kMax.z = ::etMax( kMax.z, kValues.m_kMax.z );
	}

	GraphValue& kMinG = kValues.m_kMin;
	GraphValue& kMaxG = kValues.m_kMax;
	kMinG = GraphValue::ms_Max;
	kMaxG = GraphValue::ms_Min;

	// iterate once to determine min max values
	while( kIter != kEnd )
	{
		const GraphValue& kValue = (*kIter);
		kMin.x = ::etMin( kMin.x, kValue.x );
		kMax.x = ::etMax( kMax.x, kValue.x );
		if( false == bGlobalStats )
		{
			if( kValues.getGraphType() == EGraphType_Dots )
			{
				if( kValue.y != 0.0 )
				{
					kMin.y = ::etMin( kMin.y, kValue.y );
					kMax.y = ::etMax( kMax.y, kValue.y );
				}
			}
			else
			{
				kMin.y = ::etMin( kMin.y, kValue.y );
				kMax.y = ::etMax( kMax.y, kValue.y );
			}

			kMin.z = ::etMin( kMin.z, kValue.z );
			kMax.z = ::etMax( kMax.z, kValue.z );
		}

		{
			kMinG.x = ::etMin( kMinG.x, kValue.x );
			kMinG.y = ::etMin( kMinG.y, kValue.y );
			kMinG.z = ::etMin( kMinG.z, kValue.z );
			kMaxG.x = ::etMax( kMaxG.x, kValue.x );
			kMaxG.y = ::etMax( kMaxG.y, kValue.y );
			kMaxG.z = ::etMax( kMaxG.z, kValue.z );
		}

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

	GraphValue kIntervalDrawLocal, kIntervalLocal;

	kIntervalDrawLocal.x = ::etInterval( kMinDraw.x, kMaxDraw.x );
	kIntervalDrawLocal.y = ::etInterval( kMinDraw.y, kMaxDraw.y );
	kIntervalDrawLocal.z = ::etInterval( kMinDraw.z, kMaxDraw.z );

	kIntervalLocal.x = ::etInterval( kMin.x, kMax.x );
	kIntervalLocal.y = ::etInterval( kMin.y, kMax.y );
	kIntervalLocal.z = ::etInterval( kMin.z, kMax.z );

	kIntervalDraw.x = ::etMax( kIntervalDraw.x, kIntervalDrawLocal.x );
	kIntervalDraw.y = ::etMax( kIntervalDraw.y, kIntervalDrawLocal.y );
	kIntervalDraw.z = ::etMax( kIntervalDraw.z, kIntervalDrawLocal.z );

	kInterval.x = ::etMax( kInterval.x, kIntervalLocal.x );
	kInterval.y = ::etMax( kInterval.y, kIntervalLocal.y );
	kInterval.z = ::etMax( kInterval.z, kIntervalLocal.z );

	if( kIntervalDraw.x > 0 )
	{
		kGraphLocationScale.x = ::etMin( kGraphLocationScale.x, kGraphLocation.width / kIntervalDraw.x );
		kScale.x = kGraphLocation.width / kIntervalDraw.x;
	}
	if( kIntervalDraw.y > 0 )
	{
		kGraphLocationScale.y = ::etMin( kGraphLocationScale.y, kGraphLocation.height / kIntervalDraw.y );
		kScale.y = kGraphLocation.height / kIntervalDraw.y;
	}
}
