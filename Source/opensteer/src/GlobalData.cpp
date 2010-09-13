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

#include "OpenSteer/GlobalData.h"
#include "OpenSteer/AbstractPlugin.h"
#include "OpenSteer/AbstractVehicle.h"
#include "OpenSteer/SimplePlayer.h"

//-----------------------------------------------------------------------------
namespace OpenSteer {

	GlobalData* GlobalData::ms_pkGlobalDataInstance = NULL;
	GlobalData GlobalData::ms_kGlobalDataInstance;

	//-----------------------------------------------------------------------------
	void GlobalData::_SDMInitApp( EduNet::IProfile* pkProfile )
	{
		GlobalData::ms_kGlobalDataInstance.initializeLocalPlayer();
		GlobalData::ms_kGlobalDataInstance.m_pkProfile = pkProfile;
		GlobalData::setInstance( &GlobalData::ms_kGlobalDataInstance );
	}

	//-----------------------------------------------------------------------------
	void GlobalData::_SDMInitDLL( GlobalData* pkGlobaleData )
	{
		GlobalData::setInstance( pkGlobaleData );
	}

	//-----------------------------------------------------------------------------
	void GlobalData::setInstance( GlobalData* pkGlobaleData )
	{
		GlobalData::ms_pkGlobalDataInstance = pkGlobaleData;
	}

	//-----------------------------------------------------------------------------
	GlobalData* GlobalData::getInstance( void )
	{
		assert( NULL != GlobalData::ms_pkGlobalDataInstance );
		return GlobalData::ms_pkGlobalDataInstance;
	}

	//-----------------------------------------------------------------------------
	AbstractPlayer* GlobalData::accessSimpleLocalPlayer( void )
	{
		return GlobalData::getInstance()->m_pkSimpleLocalPlayer;
	}

	//-----------------------------------------------------------------------------
	EduNet::IProfile* GlobalData::accessProfile( void )
	{
		return GlobalData::getInstance()->m_pkProfile;
	}

	//-----------------------------------------------------------------------------
	GlobalData::GlobalData( void ):
		m_pkSimpleLocalPlayer( NULL ),
		m_pkSimpleController( NULL )
	{
	}

	//-----------------------------------------------------------------------------
	GlobalData::~GlobalData()
	{
		if( this == GlobalData::ms_pkGlobalDataInstance )
		{
			GlobalData::ms_pkGlobalDataInstance = NULL;
		}
	}

	void GlobalData::initializeLocalPlayer( void )
	{
		static SimplePlayer kPlayer(true);
		this->m_pkSimpleLocalPlayer = &kPlayer;
		if( NULL == kPlayer.getController() )
		{
			static SimpleController kController;
			this->m_pkSimpleController = &kController;
			kPlayer.setController( &kController );
		}
	}

} //! namespace OpenSteer    


