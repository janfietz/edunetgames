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

#include "VehicleModule.h"
#include "OpenSteerUT/OpenSteerUT.h"

#define EDUNET_INCLUDE_OPENSTEER 1
#include "EduNetCommon/EduNetCommon.h"
//-----------------------------------------------------------------------------
void EduNetConnect::queryConnectionsSettings( ConnectSettings& kSettings )
{
}

//-----------------------------------------------------------------------------
void OpenSteer::handleGlobalDataInstanceFailure( void )
{
	// in case this happens the whole thing will simply not work
	assert( true == GlobalData::hasInstance() );
}

namespace EduNet	{

	class SingleVehiclePlugin : public EmptyPlugin
	{
		ET_DECLARE_BASE(EmptyPlugin)
	public:

		// construction / destruction
		SingleVehiclePlugin (bool bAddToRegistry = true):BaseClass(bAddToRegistry),
			m_enableLocalControl(0),m_showSamplePlot(0)
		{
		}

		virtual ~SingleVehiclePlugin() {}

		virtual const char* name (void) const {return "SingleVehiclePlugin";}

		//-----------------------------------------------------------------------------
		virtual void initGui( void* pkUserdata )
		{
			GLUI* glui = ::getRootGLUI();
			GLUI_Panel* pluginPanel = static_cast<GLUI_Panel*>( pkUserdata );
			glui->add_checkbox_to_panel( pluginPanel, "Show SamplePlot", &this->m_showSamplePlot);
			glui->add_checkbox_to_panel( pluginPanel, "Enable local control", &this->m_enableLocalControl);
			glui->add_separator_to_panel( pluginPanel );
			BaseClass::initGui( pkUserdata );
		}

		//-----------------------------------------------------------------------------
		virtual void update (const float currentTime, const float elapsedTime)
		{
			if( this->m_enableLocalControl )
			{
				this->updatePlayerController( currentTime, elapsedTime );
			}
			BaseClass::update(currentTime,elapsedTime);
		}

		virtual void redraw (const float currentTime, const float elapsedTime)
		{
			BaseClass::redraw(currentTime,elapsedTime);
			if( this->m_showSamplePlot )
			{
				this->drawSamplePlot();
			}
		}

	private:
		int m_enableLocalControl;
		int m_showSamplePlot;
		void updatePlayerController( const float currentTime, const float elapsedTime )
		{
			AbstractPlayer* pkPlayer = SimplePlayer::accessLocalPlayer();
			AbstractController* pkController = pkPlayer->accessController();
			if ( NULL != pkController )
			{
				pkController->setCustomUpdated ( LocalPlayerController::accessLocalPlayerController() );
			}
			pkPlayer->update ( currentTime, elapsedTime );
			AbstractEntity* pkControlledEntity = pkPlayer->getControlledEntity();
			AbstractEntity* pkNewControlledEntity = pkControlledEntity;
			if ( SimpleVehicle::getSelectedVehicle() != pkNewControlledEntity )
			{
				pkNewControlledEntity = SimpleVehicle::getSelectedVehicle();
			}
			if ( pkNewControlledEntity != pkControlledEntity )
			{
				if ( NULL != pkNewControlledEntity )
				{
					// right now only authorities
					if ( pkNewControlledEntity->isRemoteObject() )
					{
						pkNewControlledEntity = NULL;
					}
				}
				pkPlayer->play ( pkNewControlledEntity );
			}
		}

		void drawSamplePlot( void )
		{
			const float fGraphHeight = 175;
			// draw a test graph
			{
				Profile::GraphValues kValues;
				const size_t uiMaxRecords = kValues.getMaxRecords();
				float fX(0), fY(0);
				for( size_t i = 0; i < uiMaxRecords; ++i )
				{
					fY = fX * fX;
					fY = sinf( fX );
					//		fY = sinf( fX ) * ( ( 0.5f * fX ) * ( 0.5f * fX ) );
					kValues.addValue( fX, fY );
					fX += 0.10f;
				}
				Profile::GraphPlot kPlot;
				kPlot.draw( kValues, 50, 175, 300, fGraphHeight );
			}

			// draw a test graph
			{
				Profile::GraphValuesArray kValuesArray;

				for( size_t i = 0; i < 3; ++i )
				{
					Profile::GraphValues& kValues = kValuesArray.accessValues(i);
					const size_t uiMaxRecords = kValues.getMaxRecords();
					float fX(0), fY(0);
					for( size_t j = 0; j < uiMaxRecords; ++j )
					{
						fY = fX * fX;
						switch( i )
						{
						case(0):
							fY = sinf( fX ) * fX;
							break;
						case(1):
							fY = cos( fX ) * fX;
							break;
						case(2):
							fY = ( sinf( fX ) * ( ( 0.5f * fX ) * ( 0.5f * fX ) ) );
							break;
						}
						kValues.addValue( fX, fY );
						fX += 0.10f;
					}
				}
				Profile::GraphPlot kPlot;
				kPlot.draw( kValuesArray, 50, 175 + fGraphHeight + 20, 300, fGraphHeight );
			}

		}
	};

	//-------------------------------------------------------------------------
	VehicleModulePluginFactory::VehicleModulePluginFactory()
	{

	}

	//-------------------------------------------------------------------------
	VehicleModulePluginFactory::~VehicleModulePluginFactory()
	{

	}

	//-------------------------------------------------------------------------
	void VehicleModulePluginFactory::fillStringArrayWithPluginName( enStringArray_t& kNames ) const
	{
		// TODO: add supported plugin names to this array
		kNames.push_back( "EmptyPlugin" );
		kNames.push_back( "SingleVehiclePlugin" );
	}

	//-------------------------------------------------------------------------
	OpenSteer::AbstractPlugin* VehicleModulePluginFactory::createPluginByNameInternal(
		const char* pszName ) const
	{
		std::string kName(pszName);
		// TODO: dynamically allocate specific plugins here
		if( kName == "EmptyPlugin" )
		{
			OpenSteer::AbstractPlugin* plugin = ET_NEW EduNet::EmptyPlugin();

			return plugin;  
		}
		else if( kName == "SingleVehiclePlugin" )
		{
			OpenSteer::AbstractPlugin* plugin = ET_NEW EduNet::SingleVehiclePlugin();

			return plugin;  
		}
		return NULL;
	}

	//-------------------------------------------------------------------------
	const char* VehicleModule::getName( void ) const
	{
		return "ModVehicle";
	}

	//-------------------------------------------------------------------------
	const char* VehicleModule::getAbout( void ) const
	{
		return "Provides plugins which implements a simple vehicle and user control";
	}

	//-------------------------------------------------------------------------
	void VehicleModule::setOpenSteerUTData( OpenSteerUTData* data ) const
	{
		OpenSteerUTData::_SDMInitDLL( data );
	}

	//-------------------------------------------------------------------------
	PluginFactory* VehicleModule::createPluginFactory( void ) const
	{
		return ET_NEW VehicleModulePluginFactory();
	}
}

ET_IMPLEMENT_MODULE_ENTRYFUNC(EduNet::VehicleModule)

