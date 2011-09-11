//-----------------------------------------------------------------------------
// Copyright (c) 2011, Jan Fietz, Cyrus Preuss
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


#include "EduNetProcessProfile.h"
#include "EduNetProfile/EduNetProfile.h"
#include "EduNetCommon/EduNetExternal.h"

using namespace EduNet;


//-----------------------------------------------------------------------------
#define EN_PROFILESCOPE(a) \
class ProfileScope_##a : public IProfileScope \
{ \
public: \
	Prof_Zone* zone;\
	virtual void open(){ \
	static Prof_Define(a);\
	zone = &Prof_region_##a;\
	Prof_Begin_Cache(a);  Prof_Begin_Code( *zone ); } \
	virtual void close(){ Prof_Zone& Prof_region_##a = *zone; Prof_End } }; \
	static ProfileScope_##a s_scope_##a; \
	IProfileNodePtr spNode ( ET_NEW ProfileNode( &s_scope_##a ) ); \
	return spNode;

//-----------------------------------------------------------------------------
class ProfileNode : public IProfileNode
{
public:

	ProfileNode( IProfileScope* pkScope )
	{
		m_pkScope = pkScope;
		m_pkScope->open();	
	}
	virtual ~ProfileNode()
	{
		m_pkScope->close();	
	}

private:
	IProfileScope* m_pkScope;
};
	
//-----------------------------------------------------------------------------
namespace EduNet	{

	//-------------------------------------------------------------------------
	IProfileNodePtr ProcessProfile::allocNode( const char* pszName )
	{
		// TODO: change to a node based profiler
		enString_t kName(pszName);
		if ( strcmp( pszName, "EulerVehicleUpdate" ) )
		{
			EN_PROFILESCOPE(EulerVehicleUpdate);						
		}
		else if ( strcmp( pszName, "determineCombinedSteeringForce" ) )
		{
			EN_PROFILESCOPE(determineCombinedSteeringForce);						
		}
		else if ( strcmp( pszName, "SteeringForceVehicleUpdate" ) )
		{
			EN_PROFILESCOPE(SteeringForceVehicleUpdate);						
		}
		else if ( strcmp( pszName, "updatePhysicsVehicle" ) )
		{
			EN_PROFILESCOPE(updatePhysicsVehicle);						
		}
		else if ( strcmp( pszName, "updateNetworkPlugin" ) )
		{
			EN_PROFILESCOPE(updateNetworkPlugin);						
		}
		else if ( strcmp( pszName, "ReceivePackets" ) )
		{
			EN_PROFILESCOPE(ReceivePackets);						
		}
		else if ( strcmp( pszName, "serializeNetworkVehicle" ) )
		{
			EN_PROFILESCOPE(serializeNetworkVehicle);						
		}
		else if ( strcmp( pszName, "deserializeNetworkVehicle" ) )
		{
			EN_PROFILESCOPE(deserializeNetworkVehicle);						
		}
		IProfileNodePtr spNode ( (IProfileNode*)NULL );
		return spNode;
	}

}
