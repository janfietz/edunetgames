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

#include "stdafx.h"
#include "MemoryTracker.h"
#include <mgf/core/Macros.h>
#include <mgf/core/os/Windows.h>


#ifdef MGF_DEBUG
#  include <crtdbg.h>
#endif

namespace mgf
{
	//-------------------------------------------------------------------------
	void MemoryTracker::sdmInit( void )
	{
	}

	//-------------------------------------------------------------------------
	void MemoryTracker::sdmShutdown( void )
	{
	}

	void MemoryDebugBegin();
	void MemoryDebugEnd();

	MemoryDebug::MemoryDebug( bool isDll ):
		m_isDll(isDll)
	{
		MemoryDebugBegin();
	}

	MemoryDebug::~MemoryDebug( void )
	{
		if( true == m_isDll )
		{
			bool test(true);
			test = false;
		}
		MemoryDebugEnd();
	}

	void MemoryDebug::exitMain( void )
	{
		printf( "exit main.\n" );
	}

	//-----------------------------------------------------------------------------
	int __cdecl g_DebugAllocHook(
		int      nAllocType,
		void   * pvData,
		size_t   nSize,
		int      nBlockUse,
		long     lRequest,
		const unsigned char * szFileName,
		int      nLine
		)
	{
		if( nSize == 8 )
		{
			bool bTest = true;
			bTest = false;
		}
		else if( nSize == 48 )
		{
			bool bTest = true;
			bTest = false;
		}
		else if( nSize == 408 )
		{
			bool bTest = true;
			bTest = false;
		}
		else if( nSize == 588 )
		{
			bool bTest = true;
			bTest = false;
		}
		return( TRUE );         // Allow the memory operation to proceed
	}

	// install memory debugging facilities
//	etMemoryDebug g_MemoryDebug;

	//-----------------------------------------------------------------------------
	MGF_FORCEINLINE
	void MemoryDebugBegin()
	{
	#ifdef MGF_DEBUG
	#ifdef WIN32
		int tmpDbgFlag;
		tmpDbgFlag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
		tmpDbgFlag |= _CRTDBG_ALLOC_MEM_DF;
		tmpDbgFlag |= _CRTDBG_LEAK_CHECK_DF;
		_CrtSetDbgFlag(tmpDbgFlag);
		// uncomment this line to see a sample leak output in the output
		// console at program exit
	//	int* piaLeak = MGF_NEW int[10];
		// break on a specific memory allocation
	//	_CrtSetBreakAlloc(48);
		_CrtSetAllocHook( g_DebugAllocHook );

	#endif
	#endif
	}

	//-----------------------------------------------------------------------------
	MGF_FORCEINLINE
	void MemoryDebugEnd()
	{
	#ifdef MGF_DEBUG
	#ifdef WIN32
		_CrtSetReportMode( _CRT_WARN, _CRTDBG_MODE_DEBUG);
		_CrtSetReportMode( _CRT_ERROR, _CRTDBG_MODE_DEBUG );

		//	_CrtDumpMemoryLeaks();
	#endif
	#endif
	}
}