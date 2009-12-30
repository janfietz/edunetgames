#ifndef __ABSTRACTNETSERIALIZABLE_H__
#define __ABSTRACTNETSERIALIZABLE_H__

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

//-----------------------------------------------------------------------------
#include "EduNetConnect/EduNetConnect.h"

//-----------------------------------------------------------------------------
namespace RakNet
{
	class BitStream;
	struct SerializeParameters;
	struct DeserializeParameters;
	struct PRO;
}

//-----------------------------------------------------------------------------
namespace OpenSteer {

	//-------------------------------------------------------------------------
	enum ESerializeDataType
	{
		ESerializeDataType_Position,
		ESerializeDataType_Forward,
		ESerializeDataType_Side,
		ESerializeDataType_Up,
		ESerializeDataType_Force,
		ESerializeDataType_Radius,
		ESerializeDataType_Speed,
		ESerializeDataType_Orientation,
		ESerializeDataType_CompressedOrientation1,
		ESerializeDataType_CompressedOrientation2,
		ESerializeDataType_CompressedForce,
		ESerializeDataType_AngularVelocity,
		ESerializeDataType_LinearVelocity,
		ESerializeDataType_UpdateTicks,
		ESerializeDataType_ControllerAction,
		ESerializeDataType_Count,
	};

	//-------------------------------------------------------------------------
	class AbstractNetSerializable
	{
	public:

		virtual ~AbstractNetSerializable() { /* Nothing to do. */ };

		virtual void querySendParameters( RakNet::PRO& kPro ) const ET_ABSTRACT;
		virtual int serialize( RakNet::SerializeParameters *serializeParameters ) const ET_ABSTRACT;
		virtual void deserialize( RakNet::DeserializeParameters *deserializeParameters ) ET_ABSTRACT;
		virtual void serializeConstruction( RakNet::BitStream *constructionBitstream ) ET_ABSTRACT;
		virtual bool deserializeConstruction( RakNet::BitStream *constructionBitstream ) ET_ABSTRACT;
	};



} // namespace OpenSteer




#endif // __ABSTRACTNETSERIALIZABLE_H__