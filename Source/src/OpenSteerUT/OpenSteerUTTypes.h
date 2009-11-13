#ifndef __OPENSTEERUTTYPES_H__
#define __OPENSTEERUTTYPES_H__

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
namespace OpenSteer{

	// Include names declared in the OpenSteer namespace into the
	// namespace to search to find names.
	using namespace OpenSteer;

	//-------------------------------------------------------------------------
	typedef OpenSteer::AbstractProximityDatabase<AbstractVehicle*> ProximityDatabase;
	typedef OpenSteer::AbstractTokenForProximityDatabase<AbstractVehicle*> ProximityToken;




	float drawGetWindowHeight (void);
	float drawGetWindowWidth (void);

	class AbstractVehicle;

}

typedef OpenSteer::Vec3 osVector3;
typedef OpenSteer::AbstractVehicle osAbstractVehicle;
typedef OpenSteer::AVGroup osAVGroup;
typedef OpenSteer::ProximityDatabase osProximityDatabase;
typedef OpenSteer::ProximityToken osProximityToken;

typedef OpenSteer::EntityClassId osEntityClassId;

#endif //  __OPENSTEERUTTYPES_H__