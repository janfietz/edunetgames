//-----------------------------------------------------------------------------
//
//
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//
//
//-----------------------------------------------------------------------------
//
// Simple soccer game by Michael Holm, IO Interactive A/S
//
// I made this to learn opensteer, and it took me four hours. The players will
// hunt the ball with no team spirit, each player acts on his own accord.
//
// I challenge the reader to change the behavour of one of the teams, to beat my
// team. It should not be too hard. If you make a better team, please share the
// source code so others get the chance to create a team that'll beat yours :)
//
// You are free to use this code for whatever you please.
//
// (contributed on July 9, 2003)
//
//-----------------------------------------------------------------------------

// 10-30-09 cp/jf: modified for educational purpose

#include "NetSoccerPlugin.h"
#include "EduNetApplication/EduNetGames.h"
#include "OpenSteerUT/CameraPlugin.h"

#include <iomanip>
#include <sstream>
#include "OpenSteer/SimpleVehicle.h"
#include "OpenSteer/Draw.h"
#include "OpenSteer/Color.h"
#include "OpenSteer/UnusedParameter.h"
#include "OpenSteerUT/AbstractVehicleGroup.h"

#include "NetSoccerEntityFactory.h"
#include "NetSoccerGameLogic.h"

using namespace OpenSteer;
//-----------------------------------------------------------------------------
// now 1 global vehicle factory
NetSoccerEntityFactory gOfflineNetSoccerEntityFactory;

//-----------------------------------------------------------------------------
NetSoccerPlugin::NetSoccerPlugin ( bool bAddToRegistry ) :
	BaseClass ( bAddToRegistry )
{
	this->setEntityFactory( &gOfflineNetSoccerEntityFactory );
}
//-----------------------------------------------------------------------------
void NetSoccerPlugin::open ( void )
{


    // Make a field
    m_bbox = new NetSoccerGame::AABBox ( Vec3 ( -20,0,-10 ),  Vec3 ( 20,0,10 ) );
    // Red goal
    m_TeamAGoal = new NetSoccerGame::AABBox ( Vec3 ( -21,0,-7 ),  Vec3 ( -19,0,7 ) );
    // Blue Goal
    m_TeamBGoal = new NetSoccerGame::AABBox ( Vec3 ( 19,0,-7 ),  Vec3 ( 21,0,7 ) );
    // Make a ball

	AbstractVehicle* pkVehicle = this->createVehicle( ET_CID_NETSOCCER_BALL );
	if (NULL != pkVehicle)
	{
		this->m_Ball= dynamic_cast<NetSoccerBall*>( pkVehicle );
		this->m_Ball->setBox(m_bbox);
		this->addVehicle( pkVehicle );
	}

	// Build team A
    m_PlayerCountA = 8;
	this->createTeam(this->m_PlayerCountA, m_kTeamA, true);
   
	// Build Team B
    m_PlayerCountB = 8;
	this->createTeam(this->m_PlayerCountB, m_kTeamB, false);
   
    // initialize camera
    CameraPlugin::position2dCamera ( *m_Ball );
    Camera::camera.mode =  Camera::cmFixed;
    Camera::camera.fixedTarget.set ( 10,  CameraPlugin::camera2dElevation, 10 );
    Camera::camera.fixedPosition.set ( 40, 40, 40 );
    m_redScore = 0;
    m_blueScore = 0;

	this->reset();
}
//-----------------------------------------------------------------------------
// createTeam
//-----------------------------------------------------------------------------
void NetSoccerPlugin::createTeam(unsigned int uiTeamMemberCount,
				NetSoccerPlayer::Group& kPlayerGroup, bool bTeamId)
{
	for ( unsigned int i=0; i < uiTeamMemberCount ; ++i)
	{
		AbstractVehicle* pkVehicle = this->createVehicle( ET_CID_NETSOCCER_PLAYER );
		if (NULL != pkVehicle)
		{
			NetSoccerPlayer* pkPlayerVehicle = dynamic_cast<NetSoccerPlayer*>( pkVehicle );
			pkPlayerVehicle->setTeamIdAndPlayerNumber(bTeamId, i);
			kPlayerGroup.push_back ( pkPlayerVehicle );
			m_AllPlayers.push_back ( pkPlayerVehicle );
		}	
	}
}
//-----------------------------------------------------------------------------
void NetSoccerPlugin::update ( const float currentTime, const float elapsedTime )
{

	NetSoccerGameLogic kGameLogic;
	kGameLogic.setPlugin( this );
	kGameLogic.update( currentTime, elapsedTime );

	AbstractVehicleGroup kVG( this->allVehicles() );
	kVG.setCustomUpdated( &kGameLogic );
	kVG.update( currentTime, elapsedTime );


 //   // update simulation of test vehicle
	//this->updateTeam(m_kTeamA, currentTime, elapsedTime);
	//this->updateTeam(m_kTeamB, currentTime, elapsedTime);
 //   
 //   m_Ball->update ( currentTime, elapsedTime );

    if ( m_TeamAGoal->InsideX ( m_Ball->position() ) && m_TeamAGoal->InsideZ ( m_Ball->position() ) )
    {
        m_Ball->reset();        // Ball in blue teams goal, red scores
        m_redScore++;
    }
    if ( m_TeamBGoal->InsideX ( m_Ball->position() ) && m_TeamBGoal->InsideZ ( m_Ball->position() ) )
    {
        m_Ball->reset();        // Ball in red teams goal, blue scores
        m_blueScore++;
    }
}

//-----------------------------------------------------------------------------
void NetSoccerPlugin::redraw ( const float currentTime, const float elapsedTime )
{
    // draw test vehicle
    for ( unsigned int i=0; i < m_PlayerCountA ; i++ )
        m_kTeamA[i]->draw ();
    for ( unsigned int i=0; i < m_PlayerCountB ; i++ )
        m_kTeamB[i]->draw ();
    m_Ball->draw ( currentTime, elapsedTime );
    m_bbox->draw();
    m_TeamAGoal->draw();
    m_TeamBGoal->draw();
    {
        std::ostringstream annote;
        annote << "Red: "<< m_redScore;
        draw2dTextAt3dLocation ( annote,  Vec3 ( 23,0,0 ),
                                 Color ( 1.0f,0.7f,0.7f ),
                                 drawGetWindowWidth(),
                                 drawGetWindowHeight() );
    }
    {
        std::ostringstream annote;
        annote << "Blue: "<< m_blueScore;
        draw2dTextAt3dLocation ( annote,
                                 Vec3 ( -23,0,0 ),
                                 Color ( 0.7f,0.7f,1.0f ),
                                 drawGetWindowWidth(),
                                 drawGetWindowHeight() );
    }

    // draw "ground plane"
    // OpenSteerDemo::gridUtility (  Vec3 ( 0,0,0 ) );
}
//-----------------------------------------------------------------------------
void NetSoccerPlugin::close ( void )
{
	unsigned int uiPlayerCount = m_kTeamA.size();
    for ( unsigned int i=0; i < uiPlayerCount ; i++ )
        delete m_kTeamA[i];
    m_kTeamA.clear ();

	uiPlayerCount = m_kTeamB.size();
    for ( unsigned int i=0; i < uiPlayerCount ; i++ )
        delete m_kTeamB[i];
    m_kTeamB.clear ();
    m_AllPlayers.clear();
}
//-----------------------------------------------------------------------------
void NetSoccerPlugin::reset ( void )
{
    static Vec3 playerPosition[9] = {Vec3 ( 4,0,0 ),
                                     Vec3 ( 7,0,-5 ),
                                     Vec3 ( 7,0,5 ),
                                     Vec3 ( 10,0,-3 ),
                                     Vec3 ( 10,0,3 ),
                                     Vec3 ( 15,0, -8 ),
                                     Vec3 ( 15,0,0 ),
                                     Vec3 ( 15,0,8 ),
                                     Vec3 ( 4,0,0 )
                                    };
    // reset vehicle
    for ( unsigned int i=0; i < m_PlayerCountA ; i++ )
    {
        NetSoccerPlayer* pkPlayer = m_kTeamA[i];       
        pkPlayer->reset ();
		pkPlayer->setHomeAndPosition ( playerPosition[i] );
    }

    for ( unsigned int i=0; i < m_PlayerCountB ; i++ )
    {
        NetSoccerPlayer* pkPlayer = m_kTeamB[i];
		pkPlayer->reset ();
        Vec3 kPos ( playerPosition[i] );
        kPos.x *= -1;		
        pkPlayer->setHomeAndPosition ( kPos );
        
    }

    m_Ball->reset();
}

//-----------------------------------------------------------------------------
osAbstractVehicle* NetSoccerPlugin::createVehicle ( osEntityClassId  classId) const
{
	AbstractVehicle* pkVehicle = NULL;
	const AbstractEntityFactory* pkFactory = this->getEntityFactory();
	if( NULL != pkFactory )
	{
		pkVehicle = pkFactory->createVehicle( classId );
	}
	return pkVehicle;
}

//-----------------------------------------------------------------------------
void NetSoccerPlugin::addVehicle( AbstractVehicle* pkVehicle )
{
	if( NULL == pkVehicle )
	{
		return;
	}

	NetSoccerPlayer* pkPlayerVehicle = dynamic_cast<NetSoccerPlayer*>( pkVehicle );
	if( NULL != pkPlayerVehicle )
	{
		//pkBaseVehicle->m_pkObstacles = &this->allObstacles();
	}

	NetSoccerBall* pkBall = dynamic_cast<NetSoccerBall*>( pkVehicle );
	if( NULL != pkBall )
	{
		
	}

	/*AbstractVehicleGroup kVG( this->allVehicles() );
	kVG.addVehicle( pkVehicle );*/
}



//-----------------------------------------------------------------------------

