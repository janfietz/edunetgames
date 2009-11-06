#include "EduNetOptions.h"

// command line parsing
#include "../../../ThirdParty/argtable2-11/src/argtable2.h" 

// To include EXIT_SUCCESS
#include <cstdlib>

//-----------------------------------------------------------------------------
EduNetOptions::EduNetOptions():m_bContinueProcess(true)
{

}

//-----------------------------------------------------------------------------
EduNetOptions::~EduNetOptions()
{

}

//-----------------------------------------------------------------------------
int setOptions( EduNetOptions& kOptions, 
		   const char **defines, int ndefines
		   )
{
	int i;

	for (i=0; i<ndefines; i++)
	{
		printf("selected plugin \"%s\"\n",defines[i]);
		kOptions.setSelectedPlugin( defines[i] );
	}

	return 0;
}

//-----------------------------------------------------------------------------
int EduNetOptions::parseCommandLine(int argc, char **argv)
{
	struct arg_str  *defines = arg_strn("pP","define","PLUGIN",0,argc+2,  "plugin selection");
	struct arg_lit  *help    = arg_lit0(NULL,"help",                    "print this help and exit");
	struct arg_lit  *version = arg_lit0(NULL,"version",                 "print version information and exit");
	struct arg_end  *end     = arg_end(20);
	void* argtable[] = {defines,help,version,end};
	const char* progname = EduNetOptions::getAppName();
	int nerrors;
	int exitcode=0;

	/* verify the argtable[] entries were allocated sucessfully */
	if (arg_nullcheck(argtable) != 0)
	{
		/* NULL entries were detected, some allocations must have failed */
		printf("%s: insufficient memory\n",progname);
		exitcode = 1;
		goto exit;
	}

	/* set any command line default values prior to parsing */

	/* Parse the command line as defined by argtable[] */
	nerrors = arg_parse(argc,argv,argtable);

	/* special case: '--help' takes precedence over error reporting */
	if (help->count > 0)
	{
		printf("Usage: %s", progname);
		arg_print_syntax(stdout,argtable,"\n");
		printf("\n");
		printf("OpenSteer plugin demonstration\n");
		printf("\n");
		arg_print_glossary(stdout,argtable,"  %-25s %s\n");
		exitcode = EXIT_SUCCESS;
		this->setContinueProcess( false );
		goto exit;
	}

	/* special case: '--version' takes precedence error reporting */
	if (version->count > 0)
	{
		printf("'%s'\n",progname);
		printf("Version 0.1\n");
		printf("example program to demonstrate different OpenSteer plugins.\n");
		printf("Copyright (c) 2009 Jan Fietz, Cyrus Preuss. All Rights Reserved.\n");
		exitcode = EXIT_SUCCESS;
		this->setContinueProcess( false );
		goto exit;
	}

	/* special case: uname with no command line options induces brief help */
	if (argc==1)
	{
		printf("Try '%s --help' for more information.\n",progname);
		exitcode = EXIT_SUCCESS;
		goto exit;
	}

	/* If the parser returned any errors then display them and exit */
	if (nerrors > 0)
	{
		/* Display the error details contained in the arg_end struct.*/
		arg_print_errors(stdout,end,progname);
		printf("Try '%s --help' for more information.\n",progname);
		exitcode = EXIT_FAILURE;
		goto exit;
	}


	/* normal case: set selected options to options object */
	exitcode = setOptions( *this,
		defines->sval, defines->count );

exit:
	/* deallocate each non-null entry in argtable[] */
	arg_freetable(argtable,sizeof(argtable)/sizeof(argtable[0]));

	return exitcode;

}

//-----------------------------------------------------------------------------
void EduNetOptions::setup( void )
{

}

//-----------------------------------------------------------------------------
const char* EduNetOptions::getAppName( void )
{
	static char pszFile[MAX_PATH + 1];
	static bool bDone = false;
	if( false == bDone )
	{
		bDone = true;
		char szAppPath[MAX_PATH + 1];
		char pszDrive[MAX_PATH + 1];
		char pszDir[MAX_PATH + 1];
		char pszExtension[MAX_PATH + 1];
		const DWORD nsize = sizeof(szAppPath)/sizeof(char);
		const DWORD n = ::GetModuleFileNameA(NULL, szAppPath, nsize);
		if( n > 0 )
		{
			// extension includes the dot
			::_splitpath_s( szAppPath, 
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
}