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

#include <iomanip>
#include <sstream>
#include "OpenSteer/SimpleVehicle.h"
#include "OpenSteer/Renderer.h"
#include "OpenSteer/Color.h"
#include "OpenSteer/UnusedParameter.h"
#include "OpenSteer/AABBox.h"

#include "OpenSteerUT/AbstractVehicleGroup.h"

#include "NetSoccerEntityFactory.h"
#include "NetSoccerGameLogic.h"

using namespace OpenSteer;
//-----------------------------------------------------------------------------
// now 1 global vehicle factory
NetSoccerEntityFactory gOfflineNetSoccerEntityFactory;

//-----------------------------------------------------------------------------
NetSoccerPlugin::NetSoccerPlugin ( bool bAddToRegistry ) :
	BaseClass ( bAddToRegistry ), 
	m_Ball(NULL), 
	m_bbox( NULL ), 
	m_TeamAGoal(NULL), 
	m_TeamBGoal(NULL),
	m_pCameraPlugin(NULL)
{
	this->setEntityFactory( &gOfflineNetSoccerEntityFactory );
}
//-----------------------------------------------------------------------------
void NetSoccerPlugin::open ( void )
{


    // Make a field
    m_bbox = ET_NEW OpenSteer::AABBox ( Vec3 ( -20,0,-10 ),  Vec3 ( 20,0,10 ) );
    // Red goal
    m_TeamAGoal = ET_NEW OpenSteer::AABBox ( Vec3 ( -21,0,-7 ),  Vec3 ( -19,0,7 ) );
    // Blue Goal
    m_TeamBGoal = ET_NEW OpenSteer::AABBox ( Vec3 ( 19,0,-7 ),  Vec3 ( 21,0,7 ) );
    // Make a ball

	AbstractVehicle* pkVehicle = this->createVehicle( ET_CID_NETSOCCER_BALL );
	if (NULL != pkVehicle)
	{		
		this->addVehicle( pkVehicle );
	}

	// Build team A
    m_PlayerCountA = 8;
	this->createTeam(this->m_PlayerCountA, m_kTeamA, true);
   
	// Build Team B
    m_PlayerCountB = 8;
	this->createTeam(this->m_PlayerCountB, m_kTeamB, false);
   
	m_pCameraPlugin = ET_NEW OpenSteer::CameraPlugin();

	if(NULL != this->m_Ball)
	{
		// initialize camera
		m_pCameraPlugin->position2dCamera ( *m_Ball );
		Camera& kCam(m_pCameraPlugin->accessCamera() );

		kCam.mode =  Camera::cmFixed;
		kCam.fixedTarget.set ( 10,  CameraPlugin::camera2dElevation, 10 );
		kCam.fixedPosition.set ( 40, 40, 40 );
	}
   
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
			this->addVehicle( pkVehicle );
		}	
	}
}
//-----------------------------------------------------------------------------
void NetSoccerPlugin::update ( const float currentTime, const float elapsedTime )
{
	if( false == this->isEnabled() )
	{
		return;
	}

	if( false == this->isRemoteObject() )
	{
		AbstractPlayerGroup& kAllPlayers = this->allPlayers( );
		AbstractPlayerGroup::iterator kIter = kAllPlayers.begin();
		AbstractPlayerGroup::iterator kEnd = kAllPlayers.end();
		while( kIter != kEnd )
		{
			AbstractPlayer* pkPlayer = *kIter;
			const AbstractController* pkController = pkPlayer->getController();
			OpenSteer::CastToAbstractUpdated( pkPlayer )->update( currentTime, elapsedTime );
			++kIter;
		}
	}
	NetSoccerGameLogic kGameLogic;
	kGameLogic.setPlugin( this );
	kGameLogic.update( currentTime, elapsedTime );

	AbstractVehicleGroup kVG( this->allVehicles() );
	kVG.setCustomUpdated( &kGameLogic );
	kVG.update( currentTime, elapsedTime );

	if(NULL != m_Ball)
	{
		this->checkForGoal();
	}
  
	m_pCameraPlugin->update( currentTime, elapsedTime);
}

//-----------------------------------------------------------------------------
bool NetSoccerPlugin::checkForGoal( void )
{
	bool bGoal(false);
	if ( m_TeamAGoal->insideX ( m_Ball->position() ) && m_TeamAGoal->insideZ ( m_Ball->position() ) )
	{
		m_Ball->reset();        // Ball in blue teams goal, red scores
		m_redScore++;
		bGoal = true;
	}
	if ( m_TeamBGoal->insideX ( m_Ball->position() ) && m_TeamBGoal->insideZ ( m_Ball->position() ) )
	{
		m_Ball->reset();        // Ball in red teams goal, blue scores
		m_blueScore++;
		bGoal = true;
	}
	return bGoal;
}
//-----------------------------------------------------------------------------
void NetSoccerPlugin::redraw ( OpenSteer::AbstractRenderer* pRenderer,
							  const float currentTime, const float elapsedTime )
{
	if( false == this->isVisible() )
	{
		return;
	}

	m_pCameraPlugin->redraw( pRenderer, currentTime, elapsedTime);

	/*this->drawTeam( this->m_kTeamA );
	this->drawTeam( this->m_kTeamB );
    
    if (NULL != this->m_Ball)
    {
		m_Ball->draw ( currentTime, elapsedTime );
    }*/

	AbstractVehicleGroup kVG( this->allVehicles() );
	kVG.redraw( pRenderer, currentTime, elapsedTime );

    m_bbox->draw(pRenderer);
    m_TeamAGoal->draw(pRenderer);
    m_TeamBGoal->draw(pRenderer);

	const float fWidth(pRenderer->drawGetWindowWidth());
	const float fHeight(pRenderer->drawGetWindowHeight());
    {
        std::ostringstream annote;
        annote << "Red: "<< m_redScore;
        pRenderer->draw2dTextAt3dLocation ( annote,  Vec3 ( 23,0,0 ),
                                 Color ( 1.0f,0.7f,0.7f ),
                                 fWidth,
                                 fHeight );
    }
    {
        std::ostringstream annote;
        annote << "Blue: "<< m_blueScore;
        pRenderer->draw2dTextAt3dLocation ( annote,
                                 Vec3 ( -23,0,0 ),
                                 Color ( 0.7f,0.7f,1.0f ),
                                 fWidth,
                                 fHeight );
    }
   
}

//-----------------------------------------------------------------------------
void NetSoccerPlugin::close ( void )
{
	unsigned int uiPlayerCount = m_kTeamA.size();
    for ( unsigned int i=0; i < uiPlayerCount ; i++ )
	{
		osAbstractVehicle* pkVehicle = m_kTeamA[i];
		this->removeVehicle( pkVehicle );
        delete pkVehicle;
	}
    m_kTeamA.clear ();

	uiPlayerCount = m_kTeamB.size();
    for ( unsigned int i=0; i < uiPlayerCount ; i++ )
	{
		osAbstractVehicle* pkVehicle = m_kTeamB[i];
		this->removeVehicle( pkVehicle );
        delete pkVehicle;
	}
    m_kTeamB.clear ();
    m_AllPlayers.clear();

	if( NULL != this->m_Ball )
	{
		osAbstractVehicle* pkVehicle = this->m_Ball;
		this->removeVehicle(pkVehicle);
		ET_SAFE_DELETE( this->m_Ball );
	}

	ET_SAFE_DELETE(m_pCameraPlugin);
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
	unsigned int uiPlayerCount = m_kTeamA.size();
    for ( unsigned int i=0; i < uiPlayerCount ; i++ )
    {
        NetSoccerPlayer* pkPlayer = m_kTeamA[i];       
        pkPlayer->reset ();
		pkPlayer->setHomeAndPosition ( playerPosition[i] );
    }

	uiPlayerCount = m_kTeamB.size();
    for ( unsigned int i=0; i < uiPlayerCount ; i++ )
    {
        NetSoccerPlayer* pkPlayer = m_kTeamB[i];
		pkPlayer->reset ();
        Vec3 kPos ( playerPosition[i] );
        kPos.x *= -1;		
        pkPlayer->setHomeAndPosition ( kPos );
        
    }

	if (NULL != this->m_Ball)
	{
		m_Ball->reset();
	}    
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

	AbstractVehicleGroup kVG( this->allVehicles() );
	kVG.addVehicleToPlugin( pkVehicle, this );

	if(false == pkVehicle->isRemoteObject() )
	{
		NetSoccerPlayer* pkPlayerVehicle = dynamic_cast<NetSoccerPlayer*>( pkVehicle );
		if( NULL != pkPlayerVehicle )
		{
			if (pkPlayerVehicle->b_ImTeamA)
			{
				this->m_kTeamA.push_back(pkPlayerVehicle );
			}else
			{
				this->m_kTeamB.push_back(pkPlayerVehicle );
			}		

			m_AllPlayers.push_back ( pkPlayerVehicle );
		}

		NetSoccerBall* pkBall = dynamic_cast<NetSoccerBall*>( pkVehicle );
		if( NULL != pkBall )
		{		
			this->m_Ball = pkBall;
			this->m_Ball->setBox(m_bbox);
		}
	}
	
}
//-----------------------------------------------------------------------------
void NetSoccerPlugin::removeVehicle ( osAbstractVehicle* pkVehicle)
{
	AbstractVehicleGroup kVG( this->allVehicles() );
	if( kVG.end() == kVG.findVehicle( pkVehicle ) )
	{
		return;
	}

	NetSoccerBall* pkBall = dynamic_cast<NetSoccerBall*>( pkVehicle );
	if( NULL != pkBall )
	{		
		this->m_Ball = NULL;		
	}

	NetSoccerPlayer* pkPlayerVehicle = dynamic_cast<NetSoccerPlayer*>( pkVehicle );
	if( NULL != pkPlayerVehicle )
	{

		NetSoccerPlayer::Group::iterator kIter = this->m_AllPlayers.begin();
		NetSoccerPlayer::Group::iterator kIterEnd = this->m_AllPlayers.end();
		while (kIterEnd != kIter)
		{
			NetSoccerPlayer* pkTmpVehicle = (*kIter);
			if (pkPlayerVehicle == pkTmpVehicle)
			{
				kIter = this->m_AllPlayers.erase( kIter );
				kIterEnd = this->m_AllPlayers.end();
			}else
			{
				++kIter;
			}
		}
			
	}

	kVG.removeVehicleFromPlugin( pkVehicle );
}
//-----------------------------------------------------------------------------
void NetSoccerPlugin::addPlayer (OpenSteer::AbstractPlayer* pkPlayer)
{
	printf( "add player");
	NetSoccerPlayer* pkFreeSoccer = this->findUncontrolledSoccer( this->m_kTeamA );
	if (NULL != pkFreeSoccer)
	{
		pkPlayer->play(pkFreeSoccer);
		if( SimpleVehicle::getSelectedVehicle() != pkFreeSoccer )
		{
			SimpleVehicle::setSelectedVehicle( pkFreeSoccer );
		}
	}
	BaseClass::addPlayer( pkPlayer );
}
//-----------------------------------------------------------------------------
NetSoccerPlayer* NetSoccerPlugin::findUncontrolledSoccer( 
	NetSoccerPlayer::Group& kTeam )
{
	NetSoccerPlayer::Group::iterator kIter = kTeam.begin();
	NetSoccerPlayer::Group::iterator kIterEnd = kTeam.end();
	while (kIterEnd != kIter)
	{
		NetSoccerPlayer* pkTmpVehicle = (*kIter);
		if (false == pkTmpVehicle->isPossessed())
		{
			return pkTmpVehicle;
		}
		++kIter;		
	}
	return NULL;
}

//-----------------------------------------------------------------------------
void NetSoccerPlugin::removePlayer(OpenSteer::AbstractPlayer* pkPlayer)
{
	printf( "remove player");
	pkPlayer->play(NULL);
	BaseClass::removePlayer( pkPlayer );
}
//-----------------------------------------------------------------------------

