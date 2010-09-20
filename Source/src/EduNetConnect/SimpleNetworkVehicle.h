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

#include "EduNetConnect/NetworkVehicle.h"
#include "EduNetConnect/SimpleNetworkVehicleUpdate.h"

//-----------------------------------------------------------------------------
namespace OpenSteer {


	//-------------------------------------------------------------------------
	class SimpleProxyVehicle : public SimplePhysicsVehicle
	{
		ET_DECLARE_BASE( SimplePhysicsVehicle )
	public:
		SimpleProxyVehicle():m_bHasNewData(false)
		{

		}
		virtual ~SimpleProxyVehicle()
		{

		}
		virtual void draw( const float currentTime, const float elapsedTime );
		virtual void update (const float currentTime, const float elapsedTime);

		bool m_bHasNewData;
		bool m_bSerializedDataTypes[ESerializeDataType_Count];
		PhysicsMotionState m_kProxyMotionState;
		LocalSpaceDataArray m_kLocalSpaceData; // received / send data cue
		LocalSpace m_kextrapolatedLocalSpace;
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


		virtual void collect3DTextAnnotation( std::ostringstream& kStream );
		virtual void draw( const float currentTime, const float elapsedTime );
		virtual void update (const float currentTime, const float elapsedTime);

		//---------------------------------------------------------------------------
		// AbstractNetSerializable interface
		virtual int serialize( RakNet::SerializeParameters *serializeParameters ) const;
		virtual void deserialize( RakNet::DeserializeParameters *deserializeParameters );
		virtual void serializeConstruction(RakNet::BitStream *constructionBitstream);
		virtual bool deserializeConstruction(RakNet::BitStream *constructionBitstream );

		//---------------------------------------------------------------------------
		// extension

		SimpleProxyVehicle& accessProxyVehicle( void ) const
		{
			return this->m_kProxyVehicle;
		}

		const SimpleProxyVehicle& getProxyVehicle( void ) const
		{
			return this->m_kProxyVehicle;
		}

		void updateBase(const float currentTime, const float elapsedTime);

		bool willSendData( void ) const
		{
			return this->m_bWillSendData;
		}

		void forceSendData( void )
		{
			this->m_bWillSendData = true;
		}

		void setIsZoneMember( size_t zoneId, bool value );

		bool getIsZoneMember( size_t zoneId ) const;

		void setSupportZoning( bool value )
		{ this->m_bSupportZoning = value; };

		bool getSupportZoning( ) const
		{ return this->m_bSupportZoning; };

	private:
		// the core object responsible to create smoth moves
		SimpleNetworkVehicleUpdate m_kNetworkVehicleUpdate;
		// in case of a client the data receiver
		// in case of a server the extrapolation vehicle
		mutable SimpleProxyVehicle m_kProxyVehicle;
		TUpdatePeriod<osScalar, FloatMathLimits> m_kNetWriteUpdatePeriod;
		mutable bool m_bWillSendData;
		mutable bool m_bHasBeenSerialized;
		mutable bool m_bCollectsAnnotations;
		bool m_bSupportZoning;
		enum {
			MAX_ZONES = 16
		};
		bool m_bIsZoneMember[MAX_ZONES];
	};


} // namespace OpenSteer

#endif