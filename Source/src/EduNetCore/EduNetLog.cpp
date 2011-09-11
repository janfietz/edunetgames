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

#include "EduNetLog.h"
#include <iostream>

#ifdef _WIN32
#include "EduNetCore/EduNetCore.h"
#include <windows.h>
#endif

using namespace EduNet;


//-----------------------------------------------------------------------------
void Log::setLogColor( ELogType eType )
{
#ifdef _WIN32
	HANDLE hOutput = ::GetStdHandle(STD_OUTPUT_HANDLE);
	if( NULL == hOutput )
	{
		return;
	}
	switch( eType )
	{
	case( ELogType_Status ):
		{
			::SetConsoleTextAttribute( hOutput, FOREGROUND_BLUE|FOREGROUND_RED|FOREGROUND_GREEN );
		}
		break;
	case( ELogType_Message ):
		{
			::SetConsoleTextAttribute( hOutput, FOREGROUND_GREEN|FOREGROUND_INTENSITY );
		}
		break;
	case( ELogType_Warning ):
		{
			::SetConsoleTextAttribute( hOutput, FOREGROUND_BLUE|FOREGROUND_INTENSITY );
		}
		break;
	case( ELogType_Error ):
		{
			::SetConsoleTextAttribute( hOutput, FOREGROUND_RED|FOREGROUND_INTENSITY );
		}
		break;
	}
#endif
}

//-----------------------------------------------------------------------------
void 
Log::printLine (const char* message)
{
	Log::setLogColor( ELogType_Message );
	std::cout << message << std::endl << std::flush;
}

//-----------------------------------------------------------------------------
void 
Log::printLine (const std::ostringstream& message)
{
	Log::printLine( message.str().c_str() );
}

//-----------------------------------------------------------------------------
void 
Log::printMessage (const char* message)
{
	Log::setLogColor( ELogType_Message );
	std::cout << "msg  : " <<  message << std::endl << std::flush;
}

//-----------------------------------------------------------------------------
void 
Log::printMessage (const std::ostringstream& message)
{
	Log::printMessage( message.str().c_str() );
}

//-----------------------------------------------------------------------------
void 
Log::printWarning (const char* message)
{
	Log::setLogColor( ELogType_Warning );
	std::cout << "warn : " <<  message << std::endl << std::flush;
}

//-----------------------------------------------------------------------------
void 
Log::printWarning (const std::ostringstream& message)
{
	Log::printWarning( message.str().c_str() );
}

//-----------------------------------------------------------------------------
void 
Log::printError (const char* message)
{
	Log::setLogColor( ELogType_Error );
	std::cout << "error: " <<  message << std::endl << std::flush;
}

//-----------------------------------------------------------------------------
void 
Log::printError (const std::ostringstream& message)
{
	Log::printError( message.str().c_str() );
}
