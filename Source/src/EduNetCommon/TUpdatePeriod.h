#ifndef __TUPDATEPERIOD_H__
#define __TUPDATEPERIOD_H__

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
 #include <float.h>
 #include <stddef.h>
 #include <stdlib.h>

//-----------------------------------------------------------------------------
template <class TFLOAT, class TMathLimits>
struct TUpdatePeriod
{
	TUpdatePeriod():
m_fAccumTime( TFLOAT(0.0) ),
m_fLastAccumTime( TFLOAT(0.0) ),
m_fAccumPeriodTime( TFLOAT(0.0) ),
m_fPeriodTime( TFLOAT(-1.0) ),
m_fPeriodFrequency(  TFLOAT(0.0) ),
m_uiLoops(0)
{
}

TUpdatePeriod( const TUpdatePeriod& kOther  )
{
	this->CopyFrom( kOther );
}

TUpdatePeriod& operator=( const TUpdatePeriod& kOther ){ this->CopyFrom( kOther ); return (*this);}

void CopyFrom( const TUpdatePeriod& kOther  )
{
	m_fLastAccumTime = ( TFLOAT(0.0) );
	m_fAccumTime = ( TFLOAT(0.0) );
	m_fAccumPeriodTime = ( TFLOAT(0.0) );
	m_fPeriodTime = ( kOther.m_fPeriodTime );
	m_fPeriodFrequency = ( kOther.m_fPeriodFrequency );
	m_uiLoops = (0);
}

TFLOAT GetDeltaTime( size_t uiTicks ) const
{
	return static_cast<TFLOAT>(uiTicks) * this->m_fPeriodTime;
}

void Reset( void )
{
	// do not reset the duration here
	// explicitly set it to the desired value
	this->m_fAccumTime = TFLOAT(0.0);
	this->m_fLastAccumTime = TFLOAT(0.0);
	this->m_fAccumPeriodTime = TFLOAT(0.0);
	this->m_uiLoops = 0;
}

TFLOAT GetPeriodFrequency( void ) const
{
	return this->m_fPeriodFrequency;
}

TFLOAT GetPeriodTime( void ) const
{
	return this->m_fPeriodTime;
}

TFLOAT GetAccumTime( void ) const
{
	return this->m_fAccumTime;
}

void SetPeriodFrequency( TFLOAT fFrequency,
						bool bRandomizeStart = false,
						TFLOAT fRandomFactor = TFLOAT( 0.75 ) );

void SetPeriodTime( TFLOAT fPeriodTime,
				   bool bRandomizeStart = false,
				   TFLOAT fRandomFactor = TFLOAT( 0.75 ) );

void SetAccumPeriodTime( TFLOAT fAccumTime )
{
	this->m_fAccumPeriodTime = fAccumTime;
}

size_t UpdateAccumTime( TFLOAT fAccumTime )
{
	size_t uiLoops = 0;
	if( this->m_fPeriodTime <= TFLOAT(0.0) )
		return uiLoops;

	if( this->m_fLastAccumTime == TFLOAT(0.0) )
	{
		// prevent initial overflow caused by 'huge'
		// initial accumulated times
		if( fAccumTime > TFLOAT( 0.5 ) )
		{
			this->m_fLastAccumTime = fAccumTime;
		}
	}
	TFLOAT fDeltaTime = fAccumTime - this->m_fLastAccumTime;
	// prevent deltatime overflow ?
	if( ( fDeltaTime > TFLOAT( 5.0 ) ) && ( fDeltaTime <= this->m_fPeriodTime ) )
	{
		fDeltaTime = this->m_fPeriodTime;
	}
	this->m_fLastAccumTime = fAccumTime;

	this->m_fAccumPeriodTime += fDeltaTime;
	while( this->m_fAccumPeriodTime >= this->m_fPeriodTime )
	{
		this->m_fAccumPeriodTime -= this->m_fPeriodTime;
		this->m_fAccumTime += this->m_fPeriodTime;
		++uiLoops;
	}
	this->m_uiLoops += uiLoops;
	return uiLoops;
}

size_t UpdateDeltaTime( TFLOAT fDeltaTime )
{
	TFLOAT fAccumTime = this->m_fLastAccumTime + fDeltaTime;
	return this->UpdateAccumTime( fAccumTime );
}

void RandomizeStart( TFLOAT fRandomFactor = TFLOAT( 0.75 ) );

TFLOAT RemainingTime( void ) const
{
	if( this->m_fPeriodTime > TFLOAT(0.0) )
	{
		if( this->PeriodElapsed(  ) )
		{
			return TFLOAT(0.0);
		}
		else
		{
			return this->m_fPeriodTime - this->m_fAccumPeriodTime;
		}
	}
	else
	{
		return TFLOAT(0.0);
	}
}

TFLOAT AccumPeriodTime( void ) const
{
	return this->m_fAccumPeriodTime;
}

TFLOAT Period( void ) const
{
	return this->GetPeriodTime();
}

bool PeriodElapsed( void ) const
{
	return (0 != this->m_uiLoops);
}

private:
	// Returns a btScalar randomly distributed between 0 and 1
	TFLOAT Random01(void)
	{
		return (((TFLOAT) rand ()) / ((TFLOAT) RAND_MAX));
	}

	TFLOAT m_fLastAccumTime;
	TFLOAT m_fAccumTime;
	TFLOAT m_fAccumPeriodTime;
	TFLOAT m_fPeriodTime;
	TFLOAT m_fPeriodFrequency;
	size_t m_uiLoops;

};

//-----------------------------------------------------------------------------
template <class TFLOAT, class TMathLimits>
void TUpdatePeriod<TFLOAT, TMathLimits>::SetPeriodFrequency(
	TFLOAT fFrequency,
	bool bRandomizeStart,
	TFLOAT fRandomFactor )
{
	if( fFrequency == this->m_fPeriodFrequency )
	{
		return;
	}
	if( fFrequency > TFLOAT( 0.0 ) )
	{
		this->m_fPeriodTime = TFLOAT( 1.0 ) / fFrequency;
		this->m_fPeriodFrequency = fFrequency;
		if( true == bRandomizeStart )
		{
			this->RandomizeStart( fRandomFactor );
		}
	}
	else
	{
		this->m_fPeriodFrequency =  TFLOAT( 0.0 );
		this->m_fPeriodTime = TMathLimits::GetInfinity();
	}
}

//-----------------------------------------------------------------------------
template <class TFLOAT, class TMathLimits>
void TUpdatePeriod<TFLOAT, TMathLimits>::SetPeriodTime( TFLOAT fPeriodTime,
													   bool bRandomizeStart,
													   TFLOAT fRandomFactor )
{
	if( fPeriodTime == this->m_fPeriodTime )
	{
		return;
	}
	this->m_fPeriodTime = fPeriodTime;
	if( this->m_fPeriodTime > TFLOAT( 0.0 ) )
	{
		this->m_fPeriodFrequency = TFLOAT( 1.0 ) / this->m_fPeriodTime;;
		if( true == bRandomizeStart )
		{
			this->RandomizeStart( fRandomFactor );
		}
	}
	else
	{
		this->m_fPeriodFrequency = TMathLimits::GetInfinity();
	}
}

//-----------------------------------------------------------------------------
template <class TFLOAT, class TMathLimits>
void TUpdatePeriod<TFLOAT, TMathLimits>::RandomizeStart( TFLOAT fRandomFactor )
{
	if( TFLOAT( 0.0 ) == this->m_fLastAccumTime )
	{
		// compute a random start offset related to the period time
		TFLOAT fDeltaTime = this->Random01() * this->Period() * fRandomFactor;
		this->UpdateDeltaTime( fDeltaTime );
	}
}

//-----------------------------------------------------------------------------
class FloatMathLimits
{
public:
	static float GetInfinity(){ return FLT_MAX; };
};

//-----------------------------------------------------------------------------
class DoubleMathLimits
{
public:
	static double GetInfinity(){ return DBL_MAX; };
};

//-----------------------------------------------------------------------------
namespace EduNet	{
typedef struct TUpdatePeriod<float, FloatMathLimits> UpdatePeriodFloat;
typedef struct TUpdatePeriod<double, DoubleMathLimits> UpdatePeriodDouble;
} // namespace EduNet


#endif // __PHYSICSUPDATEPERIOD_H__
