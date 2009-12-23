#ifndef __DLLTESTPLUGIN_H__
#define __DLLTESTPLUGIN_H__
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

#include "OpenSteerUT/EmptyPlugin.h"
#include "OpenSteerUT/PluginArray.h"
#include "OpenSteerUT/CameraPlugin.h"
#include "OpenSteerUT/GridPlugin.h"
#include "EduNetModule/EduNetModule.h"

//-----------------------------------------------------------------------------
class DllTestPlugin : 
	public OpenSteer::PluginArrayPluginMixin<EduNet::EmptyPlugin>
{
	ET_DECLARE_BASE( OpenSteer::PluginArrayPluginMixin<EduNet::EmptyPlugin> )
public:
	DllTestPlugin( bool bAddToRegistry = true ):
	BaseClass( bAddToRegistry ) {}

	virtual ~DllTestPlugin() {};

	OS_IMPLEMENT_CLASSNAME( DllTestPlugin )

	virtual float selectionOrderSortKey (void) const { return 1.0f ;}

	virtual void initGui( void* pkUserdata )
	{
		this->prepare();
		BaseClass::initGui( pkUserdata );
	}

	void prepare(void)
	{
		this->addPlugin( ET_NEW OpenSteer::GridPlugin() );		
		this->addPlugin( ET_NEW OpenSteer::CameraPlugin() );
	}

	virtual void open(void);
	
	virtual void close(void)
	{		
		BaseClass::close();
		this->removeAllPlugins();
	}
private:
	
	void listAllFiles(const char* pszDirectory);
	bool addFile(const char* pszFileName);	

	typedef std::vector<EduNetRawModulePtr> EduNetRawModules;
	EduNetRawModules m_kModules;
};

#endif //__DLLTESTPLUGIN_H__