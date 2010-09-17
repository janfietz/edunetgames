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

#include "OpenSteer/GlobalSelection.h"

//-----------------------------------------------------------------------------
namespace OpenSteer {

	GlobalSelection* GlobalSelection::globalSelection = NULL;

	//-----------------------------------------------------------------------------
	void GlobalSelection::_SDMInitApp( void )
	{
		if( NULL == GlobalSelection::globalSelection )
		{
			static GlobalSelection kGlobalSelection;
			GlobalSelection::setInstance( &kGlobalSelection );
		}
	}

	//-----------------------------------------------------------------------------
	void GlobalSelection::_SDMInitDLL( GlobalSelection* selection )
	{
		GlobalSelection::setInstance( selection );
	}

	//-----------------------------------------------------------------------------
	void GlobalSelection::setInstance( GlobalSelection* selection )
	{
		GlobalSelection::globalSelection = selection;
	}

	//-----------------------------------------------------------------------------
	GlobalSelection* GlobalSelection::getInstance( void )
	{
		return GlobalSelection::globalSelection;
	}

	//-----------------------------------------------------------------------------
	GlobalSelection::GlobalSelection( void ):
		selectedVehicle(NULL),
		nearestMouseVehicle(NULL),
		cameraLocalSpaceToTrack(NULL)
	{
	}

	//-----------------------------------------------------------------------------
	GlobalSelection::~GlobalSelection()
	{
		if( this == GlobalSelection::globalSelection )
		{
			GlobalSelection::globalSelection = NULL;
		}
	}

	//-----------------------------------------------------------------------------
	void GlobalSelection::setCameraLocalSpaceToTrack( const AbstractLocalSpace* localSpace )
	{
		if( NULL != GlobalSelection::globalSelection )
		{
			if( localSpace != GlobalSelection::globalSelection->cameraLocalSpaceToTrack )
			{
				GlobalSelection::globalSelection->cameraLocalSpaceToTrack = localSpace;
			}
		}
		else
		{
			if( NULL != localSpace )
			{
				bool bTest = true;
				bTest = false;
				assert( NULL != GlobalSelection::globalSelection );
			}
		}
	}

	//-----------------------------------------------------------------------------
	void GlobalSelection::setSelectedVehicle( AbstractVehicle* vehicle )
	{
		if( NULL != GlobalSelection::globalSelection )
		{
			GlobalSelection::globalSelection->selectedVehicle = vehicle;
		}
		else
		{
			if( NULL != vehicle )
			{
				assert( NULL != GlobalSelection::globalSelection );
			}
		}
	}

	//-----------------------------------------------------------------------------
	void GlobalSelection::setNearestMouseVehicle( AbstractVehicle* vehicle )
	{
		if( NULL != GlobalSelection::globalSelection )
		{
			GlobalSelection::globalSelection->nearestMouseVehicle = vehicle;
		}
		else
		{
			if( NULL != vehicle )
			{
				bool bTest = true;
				bTest = false;
				assert( NULL != GlobalSelection::globalSelection );
			}
		}
	}

	//-----------------------------------------------------------------------------
	const AbstractLocalSpace* GlobalSelection::getCameraLocalSpaceToTrack( void )
	{
		if( NULL != GlobalSelection::globalSelection )
		{
			return GlobalSelection::globalSelection->cameraLocalSpaceToTrack;
		}
		return NULL;
	}

	//-----------------------------------------------------------------------------
	AbstractVehicle* GlobalSelection::getSelectedVehicle( void )
	{
		if( NULL != GlobalSelection::globalSelection )
		{
			return GlobalSelection::globalSelection->selectedVehicle;
		}
		return NULL;
	}

	//-----------------------------------------------------------------------------
	AbstractVehicle* GlobalSelection::getNearestMouseVehicle( void )
	{
		if( NULL != GlobalSelection::globalSelection )
		{
			return GlobalSelection::globalSelection->nearestMouseVehicle;
		}
		return NULL;
	}


} //! namespace OpenSteer    


