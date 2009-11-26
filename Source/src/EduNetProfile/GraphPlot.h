#ifndef __GRAPHPLOT_H__
#define __GRAPHPLOT_H__

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

#include <vector>
#include <string>
#include "EduNetCommon/EduNetMath.h"

//-----------------------------------------------------------------------------
namespace OpenSteer
{
	class Color;
}

//-----------------------------------------------------------------------------
namespace Profile
{
	//-------------------------------------------------------------------------
	typedef struct TGraphValue
	{
		TGraphValue():scale_y(1){}
		TGraphValue( float _x, float _y, float _z ):x(_x), y(_y), z(_z), scale_y(1){}
		union {
			float m_fValues[4];
			struct  
			{
				float x,y,z;
				float scale_y;
			};
		};

		static const TGraphValue ms_Min;
		static const TGraphValue ms_Max;

	} GraphValue;

	typedef std::vector<GraphValue> TGraphValues;

	//-------------------------------------------------------------------------
	enum EGraphType
	{
		EGraphType_Lines,
		EGraphType_Dots,
		EGraphType_Count,
	};

	//-------------------------------------------------------------------------
	class GraphValues : public TGraphValues
	{
	public:
		GraphValues( size_t uiMaxRecords = 128 ):
		  m_uiMaxRecords( uiMaxRecords ),
		  m_eGrapType( EGraphType_Lines )
		{
			this->m_fColor[0] = 
				this->m_fColor[1] = 
				this->m_fColor[2] = 
				this->m_fColor[3] = 
				0;
			this->reserve( uiMaxRecords + 1 );
			this->reset();
		}

		virtual ~GraphValues()
		{
		}

		GraphValues& addValue( float x, float y )
		{
			// prevent adding two values for one x value
			const size_t uiSize = this->size();
			if( uiSize > 0 )
			{
				const GraphValue& cv = (*this)[ uiSize - 1 ];
				if( cv.x == x )
				{
					return (*this);
				}
			}
			GraphValue v( x, y, 0 );
// 			GraphValue& kMin = this->m_kMin;
// 			GraphValue& kMax = this->m_kMax;
// 			kMin.x = ::etMin( kMin.x, v.x );
// 			kMin.y = ::etMin( kMin.y, v.y );
// 			kMin.z = ::etMin( kMin.z, v.z );
// 			kMax.x = ::etMax( kMax.x, v.x );
// 			kMax.y = ::etMax( kMax.y, v.y );
// 			kMax.z = ::etMax( kMax.z, v.z );


			this->push_back( v );
			if( this->size() > this->m_uiMaxRecords )
			{
				this->erase( this->begin() );
			}
			return (*this);
		}

		size_t getMaxRecords( void ) const { return m_uiMaxRecords; }

		GraphValues& setMaxRecords( size_t uiValue ) 
		{ 
			m_uiMaxRecords = uiValue; 
			if( this->capacity() < m_uiMaxRecords )
			{
				this->reserve( m_uiMaxRecords );
			}
			return (*this);
		}

		GraphValues& setId( size_t id ) 
		{ 
			m_uiId = id; 
			return (*this);
		}
		size_t getId( void ) const { return m_uiId; }

		GraphValues& setName( const char* pszName )
		{
			if( ( NULL == pszName ) || ( 0 == pszName[0] ) )
			{
				this->m_kName.clear();
			}
			else
			{
				this->m_kName.assign( pszName );
			}
			return (*this);
		}

		const char* getName( void ) const
		{
			if( this->m_kName.empty() )
			{
				return NULL;
			}
			return this->m_kName.c_str();
		}

		bool hasColor( void ) const
		{
			if( ( this->m_fColor[0] != 0 ) ||
				( this->m_fColor[1] != 0 ) ||
				( this->m_fColor[2] != 0 ) ||
				( this->m_fColor[3] != 0 ) )
			{
				return true;
			}
			return false;
		}

		float const* const getColor( void ) const
		{
			return this->m_fColor;
		}

		GraphValues& setColor( float r, float g, float b, float a = 1.0f )
		{
			this->m_fColor[0] = r;
			this->m_fColor[1] = g;
			this->m_fColor[2] = b;
			this->m_fColor[3] = a;
			return (*this);
		}

		void setGraphType( EGraphType eType )
		{
			this->m_eGrapType = eType;
		}

		EGraphType getGraphType( void ) const
		{
			return this->m_eGrapType;
		}

		void reset()
		{
			this->m_kMin = GraphValue::ms_Max;
			this->m_kMax = GraphValue::ms_Min;
		}

		mutable GraphValue m_kMin;
		mutable GraphValue m_kMax;
		mutable GraphValue m_kScale;

	private:
		float m_fColor[4];
		size_t m_uiMaxRecords;
		size_t m_uiId;
		EGraphType m_eGrapType;
		std::string m_kName;
	};
	
	typedef std::vector<GraphValues> TGraphValuesArray;
	//-------------------------------------------------------------------------
	class GraphValuesArray : public TGraphValuesArray
	{
	public:
		GraphValuesArray()
		{
//			this->reserve( 10 );
			this->reset();
		}
		GraphValues& accessValues( size_t uiIdx )
		{
			size_t uiSize = this->size();
			while( this->size() < uiIdx + 1 )
			{
				this->push_back( GraphValues() );
				GraphValues& kValues = (*this)[uiIdx];
				kValues.setId( this->size() );
			}
			return (*this)[uiIdx];
		}

		void setColor( float r, float g, float b, float a = 1.0f )
		{
			TGraphValuesArray::iterator kIter = this->begin();
			TGraphValuesArray::iterator kEnd = this->end();
			while( kIter != kEnd )
			{
				GraphValues& kValues = (*kIter);
				if( false == kValues.hasColor() )
				{
					kValues.setColor( r, g, b, a );
				}
				++kIter;
			}
		}

		void reset()
		{
			this->m_kMin = GraphValue::ms_Max;
			this->m_kMax = GraphValue::ms_Min;
		}

		GraphValue m_kMin;
		GraphValue m_kMax;


	};

	typedef std::vector<GraphValuesArray*> TGraphPointerArray;


	//-------------------------------------------------------------------------
	class GraphPlot
	{
	public:
		GraphPlot( void );
		virtual ~GraphPlot();

		void draw( const TGraphPointerArray& kValues, 
			float sx, float sy, float width, float height ) const;
		void draw( const GraphValuesArray& kValues, 
			float sx, float sy, float width, float height ) const;
		void draw( const GraphValues& kValues, 
			float sx, float sy, float width, float height ) const;
		void drawGraphFrame( float sx, float sy, float width, float height, bool bRectangle = true ) const;
	private:
		typedef struct TGraphLocation
		{
			TGraphLocation():fGraphIndex(0)
			{
			}
			float sx,sy;
			float width, height;
			GraphValue kMin;
			GraphValue kMax;
			GraphValue kInterval;
			GraphValue kIntervalDraw;
			GraphValue kScale;
			GraphValue kMinDraw;
			GraphValue kMaxDraw;
			float fGraphIndex;

		} GraphLocation;
		void computeGraphLocation( const GraphValues& kValues, GraphLocation& kGraphLocation ) const;
		void draw( const GraphLocation& kMasterGraphLocation, const GraphValues& kValues, 
			float sx, float sy, float width, float height ) const;
		void drawSingleGraph( const GraphValues& kValues, const GraphLocation& kGraphLocation ) const;
		void drawGraphFrame( const GraphLocation& kGraphLocation, bool bRectangle = true ) const;

		static void drawRectangle( float x0, float y0, float x1, float y1);
		static void drawQuad( float x0, float y0, float x1, float y1);
		static void setGraphColor( const GraphValues& kValues );
		static void setGraphColor( size_t uiId, OpenSteer::Color* pkColor = NULL );
	};


}


#endif //  __GRAPHPLOT_H__