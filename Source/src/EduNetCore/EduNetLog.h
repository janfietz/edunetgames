#ifndef __EDUNETLOG_H__
#define	__EDUNETLOG_H__
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

#include <sstream>

//-----------------------------------------------------------------------------
namespace EduNet
{
	//-------------------------------------------------------------------------
	enum ELogType
	{
		ELogType_Status,
		ELogType_Message,
		ELogType_Warning,
		ELogType_Error,
	};

	//-------------------------------------------------------------------------
	class Log
	{
	public:
		static void setLogColor( ELogType eType );

		//! print a line on the console
		static void printLine (const char* message);
		static void printLine (const std::ostringstream& message);

		//! print a line on the console
		static void printMessage (const char* message);
		static void printMessage (const std::ostringstream& message);

		//! like printMessage but prefix as warning
		static void printWarning (const char* message);
		static void printWarning (const std::ostringstream& message);

		//! like printMessage but prefix as error
		static void printError (const char* message);
		static void printError (const std::ostringstream& message);
	};

}


#endif // __EDUNETLOG_H__