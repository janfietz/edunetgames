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

#include "NetCtfGameLogic.h"
#include "NetCtfPlugin.h"
#include "NetCtfVehicles.h"

osVector3 NetCtfGameLogic::ms_kHomeBaseCenter(0, 0, 0);
float NetCtfGameLogic::ms_fHomeBaseRadius = 1.5f;

//-----------------------------------------------------------------------------
NetCtfGameLogic::NetCtfGameLogic( osAbstractVehicle* pkVehicle ):
	BaseClass(pkVehicle), m_pkPlugin(NULL),m_bGameOver(false)
{

}

//-----------------------------------------------------------------------------
NetCtfGameLogic::~NetCtfGameLogic()
{

}


#if ET_ACTIVE_CTF_HACK
const float resetDelay = 4;
const float resetTime = lastRunningTime + resetDelay;
if( currentTime > resetTime ) 
{
	// xxx a royal hack(should do this internal to CTF):
	OpenSteerDemo::queueDelayedResetPluginXXX();
}
#endif

//-----------------------------------------------------------------------------
void NetCtfGameLogic::update( const float currentTime, const float elapsedTime )
{
	if( this->isVehicleUpdate() )
	{
		osAbstractVehicle& kVehicle = this->vehicle();
		kVehicle.update( currentTime, elapsedTime );
		NetCtfSeekerVehicle* pkSeeker = dynamic_cast<NetCtfSeekerVehicle*>( &kVehicle );
		if( NULL != pkSeeker )
		{
			if( pkSeeker->state != NetCtfSeekerVehicle::running )
			{
				this->m_bGameOver = true;
			}
		}
		else
		{

		}
	}
	else
	{
		// game logic update
		// query seeker vehicle
		NetCtfSeekerVehicle* pkSeeker = NULL;
		NetCtfEnemyVehicle* pkEnemy = NULL;
		osAVGroup kEnemies;
		const osAVGroup& kVehicles = this->m_pkPlugin->allVehicles();
		{
			osAVCIterator kIter = kVehicles.begin();
			osAVCIterator kEnd = kVehicles.end();
			while( kIter != kEnd )
			{
				osAbstractVehicle* pkVehicle = (*kIter);
				// right now only one seeker
				if( NULL == pkSeeker )
				{
					pkSeeker = dynamic_cast<NetCtfSeekerVehicle*>( pkVehicle );
				}
				pkEnemy = dynamic_cast<NetCtfEnemyVehicle*>( pkVehicle );
				if( NULL != pkEnemy )
				{
					kEnemies.push_back( pkEnemy );
				}
				++kIter;
			}
		}

		// pass seeker to enemies and enemies to seeker
		{
			osAVCIterator kIter = kEnemies.begin();
			osAVCIterator kEnd = kEnemies.end();
			while( kIter != kEnd )
			{
				osAbstractVehicle* pkVehicle = (*kIter);
				pkEnemy = dynamic_cast<NetCtfEnemyVehicle*>( pkVehicle );
				if( NULL != pkEnemy )
				{
					pkEnemy->setSeeker( pkSeeker );
				}
				++kIter;
			}

			if( pkSeeker != NULL )
			{
				pkSeeker->accessEnemies().swap( kEnemies );
			}
		}

	}
}
