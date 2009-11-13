#ifndef __ABSTRACTVEHICLEFACTORY_H__
#define __ABSTRACTVEHICLEFACTORY_H__
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

// TODO OpenSteerCollector include
#include "EduNetCommon/EduNetCommon.h"

//-----------------------------------------------------------------------------
namespace OpenSteer
{
	//-----------------------------------------------------------------------------
	class AbstractVehicleFactory
	{
	public:
		AbstractVehicleFactory():m_pkMasterVehicle(NULL)
		  {
		  }

		  virtual ~AbstractVehicleFactory() {}

		  void setMasterVehicle( OpenSteer::AbstractVehicle* pkVehicle ) const
		  {
			  this->m_pkMasterVehicle = pkVehicle;
		  }

		  OpenSteer::AbstractVehicle* createVehicle( OpenSteer::ProximityDatabase* pkProximityDatabase ) const
		  {
			  if( NULL != this->m_pkMasterVehicle )
			  {
				  return this->m_pkMasterVehicle->cloneVehicle( pkProximityDatabase );
			  }
			  return NULL;
		  };

		  void destroyVehicle( OpenSteer::AbstractVehicle* pkVehicle ) const
		  {
			  ET_SAFE_DELETE( pkVehicle );
		  }
	private:
		mutable OpenSteer::AbstractVehicle* m_pkMasterVehicle;
	};
}


#endif // __ABSTRACTVEHICLEFACTORY_H__
