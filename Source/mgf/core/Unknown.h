#ifndef __MGFUNKNOWN_H__
#define __MGFUNKNOWN_H__

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

#include <mgf/core/IUnknown.h>
#include <mgf/core/Object.h>

//-----------------------------------------------------------------------------
namespace mgf
{

	//-----------------------------------------------------------------------------
	class Unknown : public Object, public virtual IUnknown
	{
		MGF_DECLARE_BASE( Object )
	public:
		Unknown( void );

		virtual RefCount AddRef( void ) MGF_OVERRIDE;
		virtual RefCount GetRefCount( void ) const MGF_OVERRIDE;
		virtual RefCount Release( void ) MGF_OVERRIDE;
	protected:
		virtual ~Unknown( void );
	private:
		RefCount m_uiRefcount;
	};
}

//-----------------------------------------------------------------------------
#define MGF_IMPLEMENT_UNKNOWN_NORELEASE( CLASS, PARENT_CLASS, INTERFACE ) \
private: \
	MGF_DECLARE_BASE( PARENT_CLASS ) \
protected: \
	virtual ~CLASS(); \
public: \
	virtual mgf::RefCount AddRef( void ) { return PARENT_CLASS::AddRef (); } \
	virtual mgf::RefCount GetRefCount( void ) const { return PARENT_CLASS::GetRefCount (); } \


#define MGF_IMPLEMENT_UNKNOWN( CLASS, PARENT_CLASS, INTERFACE )\
	MGF_IMPLEMENT_UNKNOWN_NORELEASE( CLASS, PARENT_CLASS, INTERFACE )\
	virtual mgf::RefCount Release( void ) { return PARENT_CLASS::Release(); }


#endif //  __MGFIUNKNOWN_H__
