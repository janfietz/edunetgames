#pragma  once
#ifndef EduNetPlayerPlugin_h__
#define EduNetPlayerPlugin_h__

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

#include "OpenSteer\Plugin.h"
#include "OpenSteerUT\PluginArray.h"
#include "EduNetApplication\EduNetModulePluginLoader.h"


namespace EduNet
{
	namespace Player
	{
		//-------------------------------------------------------------------------
		class HostPlugin : public OpenSteer::PluginArray
		{
			ET_DECLARE_BASE( OpenSteer::PluginArray )
		public:
			HostPlugin ( bool bAddToRegistry = false );
		
			OS_IMPLEMENT_CLASSNAME( HostPlugin )
			//----------------------------------------------------------------------------
			// OpenSteer::Plugin interface
			
			// required methods:
			const char* name( void ) const { return this->getClassName(); }
			virtual void prepareOpen (void);			
			virtual void close( void );			
			
		private:
			OpenSteer::AbstractPlugin* createChildPluginByName( const enString_t& pluginName );
			void addChildPlugin( OpenSteer::AbstractPlugin* pPlugin );

			boost::shared_ptr<ModulePluginLoader> m_PluginLoader;			
			ET_IMPLEMENT_CLASS_NO_COPY(HostPlugin)	
				

		};
	}
}

#endif // EduNetPlayerPlugin_h__
