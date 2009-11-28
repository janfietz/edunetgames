#ifndef __SerilizablePlayer_H__
#define __SerilizablePlayer_H__
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
#include "AbstractNetworkVehicle.h"
#include "OpenSteer/SimplePlayer.h"
#include "OpenSteerUT/AbstractEntityFactory.h"

//-------------------------------------------------------------------------
template <class Super>
class SerializablePlayerMixin : public Super, public OpenSteer::AbstractNetworkVehicle
{
	ET_DECLARE_BASE( Super )
public:

	// constructor
	SerializablePlayerMixin ();

	// destructor
	virtual ~SerializablePlayerMixin ();

	// AbstractNetworkVehicle interface
	virtual void testFunction();

	

private:

};

//----------------------------------------------------------------------------
// Constructor and destructor
template<class Super>
SerializablePlayerMixin<Super>::SerializablePlayerMixin(void)
{
}


template<class Super>
SerializablePlayerMixin<Super>::~SerializablePlayerMixin(void)
{
}

//----------------------------------------------------------------------------
// interface
template<class Super>
void
SerializablePlayerMixin<Super>::testFunction(void)
{
	bool btest = true;
	btest = false;
}

//---------------------------------------------------------------------------
class ClientSerializablePlayer : public SerializablePlayerMixin<OpenSteer::SimplePlayer>
{	
	ET_DECLARE_BASE( SerializablePlayerMixin<OpenSteer::SimplePlayer> )
public:

	ClientSerializablePlayer(){};
	virtual ~ClientSerializablePlayer(){};

	OS_IMPLEMENT_CLASSNAME( ClientSerializablePlayer )

	//---------------------------------------------------------------------------
	// AbstractNetworkVehicle interface
	virtual int serialize( RakNet::SerializeParameters *serializeParameters ) const {};
	virtual void deserialize( RakNet::DeserializeParameters *deserializeParameters ) {};
	virtual void serializeConstruction(RakNet::BitStream *constructionBitstream) {};
	virtual bool deserializeConstruction(RakNet::BitStream *constructionBitstream ) {};


};

//////-----------------------------------------------------------------------------
//class ClientSerializablePlayerFactory : public OpenSteer::TEntityFactory<ClientSerializablePlayer>
//{
//	ET_DECLARE_BASE( OpenSteer::TEntityFactory<ClientSerializablePlayer> );
//public:
//	ClientSerializablePlayerFactory(){};
//	virtual ~ClientSerializablePlayerFactory(){};
//};

#endif // __SerilizablePlayer_H__