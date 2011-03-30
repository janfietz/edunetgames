#include "stdafx.h"
#include <mgf/core/Module.h>
#include <mgf/core/os/Windows.h>

//-----------------------------------------------------------------------------
namespace mgf
{
	//-------------------------------------------------------------------------
	HModule loadModule( const char* pszModuleName )
	{
		HModule hModule = NULL;
		const HMODULE hInstDll = ::LoadLibraryA( pszModuleName );
		hModule = hInstDll;
		return hModule;
	}

	//-------------------------------------------------------------------------
	void unloadModule( HModule hModule )
	{
		if ( hModule )
		{
			::FreeLibrary( (HMODULE)hModule );
		}
	}

	//-------------------------------------------------------------------------
	Handle getModuleFunction( HModule hModule, const char* pszFunc )
	{
		HMODULE	hInstance = (HMODULE) hModule;
		Handle pFunction = NULL;
		if ( hInstance )
		{
			pFunction = ::GetProcAddress( hInstance, pszFunc );
		}
		return pFunction;
	}
}

