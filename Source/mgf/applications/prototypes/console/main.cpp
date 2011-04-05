// testConsole.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
#include <fstream>
#include <string>

#include <mgf/core/Object.h>
#include <mgf/memory/MemoryTracker.h>

// install debug memory tracking facility
mgf::MemoryDebug memoryDebug;

#define MGF_TEST_LEAK 1

//-----------------------------------------------------------------------------
int _tmain(int argc, _TCHAR* argv[])
{
	mgf::Object* o = MGF_NEW mgf::Object;
#if MGF_TEST_LEAK
#else
	MGF_DELETE o;
#endif
	return 0;
}
