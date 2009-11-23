#ifndef __ABSTRACTENTITYREPLICA_H__
#define __ABSTRACTENTITYREPLICA_H__

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

#include "EduNetConnect/OSReplicaTypes.h"
#include "OpenSteerUT/OpenSteerUTTypes.h"
#include "OpenSteerUT/OpenSteerUTTypes.h"

//-----------------------------------------------------------------------------
class AbstractEntityReplica : public OSReplica< OpenSteer::AbstractEntity >
{
	ET_DECLARE_BASE( OSReplica< OpenSteer::AbstractEntity > )
public:
	AbstractEntityReplica();
	AbstractEntityReplica( OpenSteer::AbstractPlugin* pPlugin, OpenSteer::EntityClassId classId, bool bIsRemoteObject );

	//-------------------------------------------------------------------------
	// replica interface
	virtual RakNet::RakString GetName( void ) const;

	virtual void DeallocReplica( RakNet::Connection_RM3 *sourceConnection );

	virtual RakNet::RM3SerializationResult Serialize( RakNet::SerializeParameters *serializeParameters );
	virtual void SerializeConstruction(RakNet::BitStream *constructionBitstream,
		RakNet::Connection_RM3 *destinationConnection);

	virtual void Deserialize(RakNet::DeserializeParameters *deserializeParameters);
	virtual bool DeserializeConstruction(RakNet::BitStream *constructionBitstream,
		RakNet::Connection_RM3 *sourceConnection);

private:
	OpenSteer::AbstractPlugin* m_pkHostPlugin;
	RakNet::RakString m_kClassName;

	ET_IMPLEMENT_CLASS_NO_COPY( AbstractEntityReplica );
};


#endif //  __ABSTRACTENTITYREPLICA_H__