#ifndef __NETWORKENTITY_H__
#define __NETWORKENTITY_H__

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

#include "AbstractNetSerializable.h"
#include "OpenSteerUT/OpenSteerUTTypes.h"

namespace OpenSteer {



	//-------------------------------------------------------------------------
	class NetworkEntitySerializer : public AbstractNetSerializable
	{
	public:
		NetworkEntitySerializer( OpenSteer::AbstractEntity* pkEntity );
		NetworkEntitySerializer( OpenSteer::AbstractVehicle* pkVehicle );
		virtual ~NetworkEntitySerializer(){};
		virtual void querySendParameters( RakNet::PRO& kPro ) const
		{

		}
		virtual void querySendParameters( OpenSteer::AbstractNetSerializable* pkVehicle, RakNet::PRO& kPro ) const;
		virtual int serialize( RakNet::SerializeParameters *serializeParameters ) const;
		virtual void deserialize( RakNet::DeserializeParameters *deserializeParameters );
		virtual void serializeConstruction(RakNet::BitStream *constructionBitstream);
		virtual bool deserializeConstruction(RakNet::BitStream *constructionBitstream );

		static bool setLocalSpaceVariable( ESerializeDataType eVariable, AbstractLocalSpace* pkLocalSpace, const osVector3& kValue );
		static bool setLocalSpaceDataVariable( ESerializeDataType eVariable, LocalSpaceData& pkLocalSpace, const osVector3& kValue );
	private:
		OpenSteer::AbstractVehicle* m_pkVehicle;
		OpenSteer::AbstractEntity* m_pkEntity;

	};



} // namespace OpenSteer

#endif