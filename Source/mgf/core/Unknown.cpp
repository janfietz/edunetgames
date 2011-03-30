#include "stdafx.h"
#include <mgf/core/Unknown.h>

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
// * Neither the name of RockNet nor the names of its contributors
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

//-----------------------------------------------------------------------------
namespace mgf
{
	//-----------------------------------------------------------------------------
	Unknown::Unknown( void ):m_uiRefcount(0)
	{

	}

	//-----------------------------------------------------------------------------
	Unknown::~Unknown( void )
	{

	}

	//-----------------------------------------------------------------------------
	RefCount Unknown::AddRef( void )
	{
		++this->m_uiRefcount;
		return this->m_uiRefcount;
	}

	//-----------------------------------------------------------------------------
	RefCount Unknown::GetRefCount( void ) const
	{
		return this->m_uiRefcount;
	}

	//-----------------------------------------------------------------------------
	RefCount Unknown::Release( void )
	{
		if( this->m_uiRefcount > 0 )
		{
			--this->m_uiRefcount;
			if( 0 == this->m_uiRefcount )
			{
				MGF_DELETE this;
				return 0;
			}
		}
		return this->m_uiRefcount;
	}
}

