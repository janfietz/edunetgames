#ifndef __SIMPLENETWORKVEHICLE_H__
#define __SIMPLENETWORKVEHICLE_H__

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

#include "NetworkVehicle.h"
#include "OpenSteerUT/SimplePhysicsVehicle.h"

//-----------------------------------------------------------------------------
namespace OpenSteer {

	//-------------------------------------------------------------------------
	class SimpleNetworkVehicleUpdate : public AbstractVehicleUpdate {
		OS_DECLARE_BASE(AbstractVehicleUpdate)
	public:
		SimpleNetworkVehicleUpdate( AbstractVehicle* pkVehicle ):
		BaseClass( pkVehicle )
		{
		}
		virtual ~SimpleNetworkVehicleUpdate(){}

		//-------------------------------------------------------------------
		// interface AbstractUpdated
		virtual void updateCustom( AbstractUpdated* pkParent, const osScalar currentTime, const osScalar elapsedTime );
		virtual void update( const osScalar currentTime, const osScalar elapsedTime );
	private:
	};

	//-------------------------------------------------------------------------
	class SimpleProxyVehicle : public SimplePhysicsVehicle
	{
	public:
		SimpleProxyVehicle():m_bHasNewData(false)
		{

		}
		virtual ~SimpleProxyVehicle()
		{

		}
		bool m_bHasNewData;
	private:
	};

	// SimpleNetworkVehicle_1 adds concrete NetworkVehicle methods to SimpleVehicle
	typedef NetworkVehicleMixin<SimplePhysicsVehicle> SimpleNetworkVehicle_1;

	//-------------------------------------------------------------------------
	class SimpleNetworkVehicle : public SimpleNetworkVehicle_1
	{
		ET_DECLARE_BASE( SimpleNetworkVehicle_1 )
	public:
		SimpleNetworkVehicle();
		virtual ~SimpleNetworkVehicle();

		OS_IMPLEMENT_CLASSNAME( SimpleNetworkVehicle )


		virtual void update (const float currentTime, const float elapsedTime);

		//---------------------------------------------------------------------------
		// AbstractNetworkVehicle interface
		virtual int serialize( RakNet::SerializeParameters *serializeParameters ) const;
		virtual void deserialize( RakNet::DeserializeParameters *deserializeParameters );

		SimpleProxyVehicle& accessProxyVehicle( void )
		{
			return this->m_kProxyVehicle;
		}
	private:
		SimpleNetworkVehicleUpdate m_kNetworkVehicleUpdate;
		SimpleProxyVehicle m_kProxyVehicle;

	};


} // namespace OpenSteer

#endif