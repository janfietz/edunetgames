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

namespace Profile
{
	//-------------------------------------------------------------------------
	typedef struct TGraphValue
	{
		TGraphValue(){}
		TGraphValue( float _x, float _y, float _z ):x(_x), y(_y), z(_z){}
		union {
			float m_fValues[3];
			struct  
			{
				float x,y,z;
			};
		};

		static const TGraphValue ms_Min;
		static const TGraphValue ms_Max;

	} GraphValue;

	typedef std::vector<GraphValue> TGraphValues;

	//-------------------------------------------------------------------------
	class GraphValues : public TGraphValues
	{
	public:
		GraphValues( size_t uiMaxRecords = 128 ):m_uiMaxRecords( uiMaxRecords )
		{
			this->reserve( uiMaxRecords + 1 );
		}

		void addValue( float x, float y )
		{
			// prevent adding two values for one x value
			const size_t uiSize = this->size();
			if( uiSize > 0 )
			{
				const GraphValue& cv = (*this)[ uiSize - 1 ];
				if( cv.x == x )
				{
					return;
				}
			}
			GraphValue v( x, y, 0 );
			this->push_back( v );
			if( this->size() > this->m_uiMaxRecords )
			{
				this->erase( this->begin() );
			}
		}
		size_t getMaxRecords( void ) const { return m_uiMaxRecords; }

		void setId( size_t id ) { m_uiId = id; }
		size_t getId( void ) const { return m_uiId; }

		void setName( const char* pszName )
		{
			this->m_kName.assign( pszName );
		}

		const char* getName( void ) const
		{
			return this->m_kName.c_str();
		}
	private:
		size_t m_uiMaxRecords;
		size_t m_uiId;
		std::string m_kName;
	};
	
	typedef std::vector<GraphValues> TGraphValuesArray;
	//-------------------------------------------------------------------------
	class GraphValuesArray : public TGraphValuesArray
	{
	public:
		GraphValues& accessValues( size_t uiIdx )
		{
			while( this->size() < uiIdx + 1 )
			{
				this->push_back( GraphValues() );
				GraphValues& kValues = (*this)[uiIdx];
				kValues.setId( this->size() );
			}
			return (*this)[uiIdx];
		}
	};

	//-------------------------------------------------------------------------
	class GraphPlot
	{
	public:
		GraphPlot( void );
		virtual ~GraphPlot();

		void draw( const GraphValuesArray& kValues, float sx, float sy, float width, float height );
		void draw( const GraphValues& kValues, float sx, float sy, float width, float height ) const;
		typedef struct
		{
			float sx,sy;
			float width, height;
			GraphValue kMin;
			GraphValue kMax;
			GraphValue kInterval;
			GraphValue kIntervalDraw;
			GraphValue kScale;
			GraphValue kMinDraw;
			GraphValue kMaxDraw;

		} GraphLocation;
	private:
		void computeGraphLocation( const GraphValues& kValues, GraphLocation& kGraphLocation ) const;
		void drawSingleGraph( const GraphValues& kValues, const GraphLocation& kGraphLocation ) const;
		void drawGraphFrame( const GraphLocation& kGraphLocation ) const;

		static void drawRectangle( float x0, float y0, float x1, float y1);
		static void drawQuad( float x0, float y0, float x1, float y1);
		static void setGraphColor( size_t uiId );
	};


}


#endif //  __GRAPHPLOT_H__