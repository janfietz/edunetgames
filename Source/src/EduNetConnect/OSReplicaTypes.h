#ifndef __OSREPLICATYPES_H__
#define __OSREPLICATYPES_H__

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

#include "AbstractReplica.h"

//-----------------------------------------------------------------------------
template< class OSType >
class OSReplica : public EduNet::AbstractReplica
{
	ET_DECLARE_BASE( EduNet::AbstractReplica )
public:
	OSReplica():m_pkEntity( NULL ){};
	virtual ~OSReplica()
	{
		this->releaseEntity();
	};
	
	//-----------------------------------------------------------------------------
	// RakNet::Replica3 interface
	virtual RakNet::RM3ConstructionState QueryConstruction(
		RakNet::Connection_RM3 *destinationConnection,
		RakNet::ReplicaManager3 *replicaManager3)
	{
		return QueryConstruction_PeerToPeer( destinationConnection );
	}
	virtual bool QueryRemoteConstruction(RakNet::Connection_RM3 *sourceConnection)
	{
		return QueryRemoteConstruction_PeerToPeer(sourceConnection);
	}
	virtual RakNet::RM3QuerySerializationResult QuerySerialization(
		RakNet::Connection_RM3 *destinationConnection)
	{
		return QuerySerialization_PeerToPeer(destinationConnection);
	}
	virtual RakNet::RM3ActionOnPopConnection QueryActionOnPopConnection(
		RakNet::Connection_RM3 *droppedConnection) const
	{
		return QueryActionOnPopConnection_PeerToPeer( droppedConnection );
	}

	virtual void SetNetworkID( NetworkID id )
	{
		BaseClass::SetNetworkID( id );
		assert( NULL != this->accessEntity() );
		this->accessEntity()->setNetworkId( id.guid.g );
	}

	//-----------------------------------------------------------------------------
	EF_FORCEINLINE
	OSType* accessEntity( void )const
	{
		return this->m_pkEntity;
	};

	EF_FORCEINLINE
	OSType const* const getEntity( void )const
	{
		return this->m_pkEntity;
	};

	EF_FORCEINLINE
	void setEntity( OSType* pkEntity )
	{
		this->m_pkEntity = pkEntity;
	}

	EF_FORCEINLINE
	void releaseEntity( void )
	{
		// delete the hosted object
		// in case we change to a ref object model
		// release the reference here
		ET_SAFE_DELETE( this->m_pkEntity );
	}

private:
	OSType* m_pkEntity;
};




#endif //__OSREPLICATYPES_H__
