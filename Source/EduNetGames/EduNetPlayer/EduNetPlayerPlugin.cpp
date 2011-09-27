//-----------------------------------------------------------------------------
//
//
// OpenSteer -- Steering Behaviors for Autonomous Characters
//
// Copyright (c) 2002-2005, Sony Computer Entertainment America
// Original author: Craig Reynolds <craig_reynolds@playstation.sony.com>
//
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//
//
//-----------------------------------------------------------------------------
#include "EduNetPlayerPlugin.h"

#include "EduNetApplication/EduNetModulePluginLoader.h"
#include "EduNetApplication/EduNetModuleManager.h"
#include "EduNetCommon/EduNetOptions.h"
#include <boost/foreach.hpp>
#include "OpenSteerUT/OpenSteerUTTypes.h"
#include "EduNetCommon/EduNetDraw.h"

using namespace boost;
namespace EduNet
{
	namespace Player
	{	
		HostPlugin::HostPlugin( bool bAddToRegistry /*= false */ ):BaseClass(bAddToRegistry)
		{
			m_PluginLoader = boost::shared_ptr<ModulePluginLoader>(ET_NEW ModulePluginLoader());			
		}
	
		void HostPlugin::prepareOpen( void )
		{
			ModuleManager kModuleManager;			
			m_PluginLoader->loadModules(kModuleManager.getCurrentModulePath());


			Options& etOptions = Options::accessOptions();
			const program_options::variables_map& variables = etOptions.Variables();

			enStringArray_t pluginNames;
			if (variables.count("create")) 
			{
				pluginNames = variables["create"].as< enStringArray_t >();			
			}
			
			if( pluginNames.empty() )
			{
				m_PluginLoader->queryAvailablePlugins(pluginNames);
			}

			BOOST_FOREACH(enString_t pluginName, pluginNames)
			{
				OpenSteer::AbstractPlugin* pPlugin = createChildPluginByName(pluginName);
				if (pPlugin != NULL)
				{
					addChildPlugin(pPlugin);
				}
				else
				{
					std::ostringstream message;
					message << "\"" << pluginName << "\" is not a valid name to create a plugin";
					message << std::ends;
					EduNet::Log::printMessage ( message );
				}
			}
						
			BaseClass::prepareOpen();
			
		}		
	
	
		void HostPlugin::close( void )
		{
			GLUI_Master.closenow_all();		
			OpenSteer::Camera::accessInstance().reset();

			removeAllPlugins();
			m_PluginLoader->unloadModules();			
		}
	

		AbstractPlugin* HostPlugin::createChildPluginByName( const enString_t& pluginName )
		{
			return m_PluginLoader->createPluginByName( pluginName.c_str() );
		}

		void HostPlugin::addChildPlugin( AbstractPlugin* pPlugin )
		{
			// for now just draw the first plugins
			bool bVisible = (getPluginCount()==0);
			osAbstractEntity* pkPluginEntity = OpenSteer::CastToAbstractEntity( pPlugin );
			pkPluginEntity->setVisible( bVisible );
			addPlugin(pPlugin);
		}	

		void HostPlugin::reset( void )
		{
			BaseClass::close();
			BaseClass::open();
		}

	}
}


