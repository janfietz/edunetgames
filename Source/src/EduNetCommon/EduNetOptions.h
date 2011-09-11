#ifndef __EDUNETOPTIONS_H__
#define __EDUNETOPTIONS_H__

//-----------------------------------------------------------------------------
// Copyright (c) 2011, Jan Fietz, Cyrus Preuss
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

#include "EduNetCore/EduNetCore.h"

namespace EduNet	{

	//-------------------------------------------------------------------------
	class Options
	{
	public:

		int parseCommandLine ( int argc, char **argv );

		void setup ( void );

		bool continueProcess ( void ) const;
		void setContinueProcess ( bool bValue );

		static const char* getAppName ( void );

		void setSelectedPlugin ( const char* pluginName )
		{
			this->m_kPluginName.assign ( pluginName );
		}
		const char* getSelectedPlugin ( void )
		{
			return this->m_kPluginName.c_str();
		}
		enStringArray_t& accessModuleNameList ( void )
		{
			return this->m_kModuleNames;
		};

		static Options& accessOptions ( void );


	private:
		Options();
		virtual ~Options();
		bool m_bContinueProcess;
		enString_t m_kPluginName;
		enStringArray_t m_kModuleNames;
	};

	//-------------------------------------------------------------------------
	inline
		bool Options::continueProcess ( void ) const
	{
		return this->m_bContinueProcess;
	}

	//-------------------------------------------------------------------------
	inline
		void Options::setContinueProcess ( bool bValue )
	{
		this->m_bContinueProcess = bValue;
	}

}

#endif  // __EDUNETOPTIONS_H__
