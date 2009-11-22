#ifndef __NETCTFPLUGIN_H__
#define __NETCTFPLUGIN_H__
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
#include "EduNetCommon/EduNetCommon.h"

//-----------------------------------------------------------------------------
class NetCtfPlugin : public OpenSteer::Plugin
{
	ET_DECLARE_BASE(OpenSteer::Plugin);
public:
	NetCtfPlugin( bool bAddToRegistry = true );
	virtual ~NetCtfPlugin() {} // be more "nice" to avoid a compiler warning

	OS_IMPLEMENT_CLASSNAME( NetCtfPlugin )
	//-------------------------------------------------------------------------
	// OpenSteer::Plugin interface
	virtual const char* name() const { return this->getClassName(); };

	virtual float selectionOrderSortKey (void) const {return 0.01f;}

	virtual void open (void);

	virtual void update (const float currentTime, const float elapsedTime);

	virtual void redraw (const float currentTime, const float elapsedTime);

	virtual void close (void);

	virtual void reset (void);

	virtual void handleFunctionKeys (int keyNumber);

	virtual void printMiniHelpForFunctionKeys (void) const;

	virtual const osAVGroup& allVehicles (void) const {return all;}
	virtual osAVGroup& allVehicles (void) {return all;}

	void NetCtfPlugin::initGui( void* pkUserdata );

	// implement to create a vehicle of the specified class
	virtual osAbstractVehicle* createVehicle( osEntityClassId ) const;
	virtual void addVehicle( osAbstractVehicle* pkVehicle );

	//-------------------------------------------------------------------------
	void drawObstacles( void );
	void addOneObstacle( void );
	void removeOneObstacle( void );
	void randomizeStartingPositionAndHeading( osAbstractVehicle* pkVehicle );

	float minDistanceToObstacle( const osVector3& point ) const;

	// a group (STL vector) of all vehicles in the Plugin
	std::vector<osAbstractVehicle*> all;
	int resetCount;
	class NetCtfSeekerVehicle* m_pkSeeker;

};



#endif // __NETCTFPLUGIN_H__