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

#include "LectureModule.h"
#include "EduNetConnect/NetworkPlugin.h"
#include "OpenSteerUT/OpenSteerUT.h"

//-----------------------------------------------------------------------------
void EduNetConnect::queryConnectionsSettings( ConnectSettings& kSettings )
{
	kSettings.uiClientStartPort = CLIENT_PORT + 1000;
	kSettings.uiServerStartPort = SERVER_PORT + 1000;
	kSettings.sessionPassword = "Lecture00";
	kSettings.uiPortPongCount = 10;
}

//-----------------------------------------------------------------------------
void OpenSteer::handleGlobalDataInstanceFailure( void )
{
	// in case this happens the whole thing will simply not work
	assert( true == GlobalData::hasInstance() );
}

namespace EduNet	{

	//-------------------------------------------------------------------------
	LectureModulePluginFactory::LectureModulePluginFactory()
	{

	}

	//-------------------------------------------------------------------------
	LectureModulePluginFactory::~LectureModulePluginFactory()
	{

	}

	//-------------------------------------------------------------------------
	void LectureModulePluginFactory::fillStringArrayWithPluginName( enStringArray_t& kNames ) const
	{
		// TODO: add supported plugin names to this array
	}

	//-------------------------------------------------------------------------
	OpenSteer::AbstractPlugin* LectureModulePluginFactory::createPluginByNameInternal(
		const char* pszName ) const
	{
		std::string kName(pszName);
		// TODO: dynamically allocate specific plugins here
		return NULL;
	}

	//-------------------------------------------------------------------------
	const char* LectureModule::getName( void ) const
	{
		return "ModLecture";
	}

	//-------------------------------------------------------------------------
	const char* LectureModule::getAbout( void ) const
	{
		return "Provides plugins from edunet lecture students";
	}

	//-------------------------------------------------------------------------
	void LectureModule::setOpenSteerUTData( OpenSteerUTData* data ) const
	{
		OpenSteerUTData::_SDMInitDLL( data );
	}

	//-------------------------------------------------------------------------
	PluginFactory* LectureModule::createPluginFactory( void )
	{
		return ET_NEW LectureModulePluginFactory();
	}

	void LectureModule::destroyPluginFactory( PluginFactory* pFactory )
	{
		ET_SAFE_DELETE(pFactory);
	}

	LectureModule::~LectureModule()
	{

	}

	LectureModule::LectureModule()
	{

	}
}

//#include <mgf/memory/MemoryTracker.h>

// install debug memory tracking facility
//mgf::MemoryDebug memoryDebug(true);

ET_IMPLEMENT_MODULE_ENTRYFUNC(EduNet::LectureModule)

