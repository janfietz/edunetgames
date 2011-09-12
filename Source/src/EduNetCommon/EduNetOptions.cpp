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

#include "EduNetOptions.h"
#include "EduNetCore/EduNetLog.h"

#ifdef _WIN32
#include <windows.h>
#endif

// command line parsing
#include "argtable2.h"

// To include EXIT_SUCCESS
#include <cstdlib>

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/errors.hpp>

using namespace boost;

namespace EduNet
{
	

	int setOptions ( EduNet::Options& kOptions,
		const char **defines, int ndefines
		);


	//-------------------------------------------------------------------------
	Options::Options() :m_bContinueProcess ( true )
	{

	}

	//-------------------------------------------------------------------------
	Options::~Options()
	{

	}

	//-------------------------------------------------------------------------
	Options& Options::accessOptions ( void )
	{
		static Options kOptions;
		return kOptions;
	}

	//-------------------------------------------------------------------------
	int Options::parseCommandLine ( int argc, char **argv )
	{
		int exitcode=0;
		program_options::options_description desc("Allowed options");
		desc.add_options()
			("help,h", "print this help and exit")
			("version,V", "print version information and exit")
			("list,L", "available modules and plugins")
			("modules,M", program_options::value< enStringArray_t >(), "specify one ore more modules to load")
			("plugin,P", program_options::value< enString_t  >(), "preselct a plugin by name from load modules")
			;
		program_options::variables_map vm;
		try
		{
	
			program_options::store(program_options::command_line_parser(argc, argv).
				options(desc).run(), vm);
			program_options::notify(vm);
		}
		catch (program_options::unknown_option unk)
		{			
			std::cout << unk.what() << "\n";
			std::cout << desc << "\n";
			return EXIT_FAILURE;
		}
		catch (...)
		{
			std::cout << desc << "\n";
			return EXIT_FAILURE;
		}

		if (vm.count("help")) {
			std::cout << desc << "\n";
			return EXIT_FAILURE;
		}

		if (vm.count("version")) 
		{
			printVersion();		
			this->setContinueProcess ( false );
			return EXIT_FAILURE;
		}

		m_bListModules = vm.count("list") > 0;

		if (vm.count("modules")) 
		{
			m_kModuleNames = vm["modules"].as< enStringArray_t >();			
		}

		if (vm.count("plugin")) 
		{
			m_kPluginName = vm["plugin"].as< enString_t >();			
		}		

		return EXIT_SUCCESS;
	}

	void Options::printVersion()
	{
		printf ( "'%s'\n", getAppName() );
		printf ( "Version 0.5\n" );
		printf ( "example program to demonstrate different OpenSteer plugins.\n" );
		printf ( "Copyright (c) 2011 Jan Fietz, Cyrus Preuss. All Rights Reserved.\n" );		
	}

	//-------------------------------------------------------------------------
	void Options::setup ( void )
	{

	}

	//-------------------------------------------------------------------------
	const char* Options::getAppName ( void )
	{
#ifdef _WIN32
		static char pszFile[MAX_PATH + 1];
		static bool bDone = false;
		if ( false == bDone )
		{
			bDone = true;
			char szAppPath[MAX_PATH + 1];
			char pszDrive[MAX_PATH + 1];
			char pszDir[MAX_PATH + 1];
			char pszExtension[MAX_PATH + 1];
			const DWORD nsize = sizeof ( szAppPath ) /sizeof ( char );
			const DWORD n = ::GetModuleFileNameA ( NULL, szAppPath, nsize );
			if ( n > 0 )
			{
				// extension includes the dot
				::_splitpath_s ( szAppPath,
					pszDrive, MAX_PATH,
					pszDir, MAX_PATH,
					pszFile, MAX_PATH,
					pszExtension, MAX_PATH );
				return pszFile;
			}
			else
			{
				pszFile[0] = 0;
			}
		}
		return pszFile;
#else
		return "EduNetGamesApp";
#endif // _WIN32
	}

}
