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
#include"NetSoccerPlayer.h"

using namespace OpenSteer;
//-----------------------------------------------------------------------------
NetSoccerPlayer::NetSoccerPlayer () :
  m_others(NULL), m_AllPlayers(NULL), m_Ball(NULL), b_ImTeamA(false), m_MyID(0)
{
  reset ();
}

//-----------------------------------------------------------------------------
void NetSoccerPlayer::possessBy( OpenSteer::AbstractEntity* pkEntity )
{
	BaseClass::possessBy( pkEntity );

}

//-----------------------------------------------------------------------------
// reset state
void NetSoccerPlayer::reset (void)
{
  BaseClass::reset (); // reset the vehicle
  setSpeed (0.0f);         // speed along Forward direction.
  setMaxForce (3000.7f);      // steering force is clipped to this magnitude
  setMaxSpeed (10);         // velocity is clipped to this magnitude

  setTrailParameters (10, 60);
}

//-----------------------------------------------------------------------------
// per frame simulation update
// (parameter names commented out to prevent compiler warning from "-W")
osVector3 NetSoccerPlayer::determineCombinedSteering( const float elapsedTime )
{
	osVector3 kSteeringForce( osVector3::zero );
    // otherwise consider avoiding collisions with others
    osVector3 collisionAvoidance = steerToAvoidNeighbors(1, (OpenSteer::AVGroup&)*m_AllPlayers);
    if(collisionAvoidance != osVector3::zero)
	{
		kSteeringForce = collisionAvoidance;
	}
	else
	{
		float distHomeToBall = osVector3::distance (m_home, m_Ball->position());
		if( distHomeToBall < 12.0f)
		{
			// go for ball if I'm on the 'right' side of the ball
			if( b_ImTeamA ? position().x > m_Ball->position().x : position().x < m_Ball->position().x)
			{
				osVector3 seekTarget = xxxsteerForSeek(m_Ball->position());
				kSteeringForce = seekTarget;
			}
			else
			{
				if( distHomeToBall < 12.0f)
				{
					float Z = m_Ball->position().z - position().z > 0 ? -1.0f : 1.0f;
					osVector3 behindBall = m_Ball->position() + (b_ImTeamA ? osVector3(2.0f,0.0f,Z) : osVector3(-2.0f,0.0f,Z));
					osVector3 behindBallForce = xxxsteerForSeek(behindBall);
					annotationLine (position(), behindBall ,OpenSteer::Color(0.0f,1.0f,0.0f));
					osVector3 evadeTarget = xxxsteerForFlee(m_Ball->position());
					kSteeringForce = behindBallForce*10.0f + evadeTarget;
				}
			}
		}
		else	// Go home
		{
			osVector3 seekTarget = xxxsteerForSeek(m_home);
			osVector3 seekHome = xxxsteerForSeek(m_home);
			kSteeringForce = seekTarget + seekHome;
		}
	}
	// return steering constrained to global XZ "ground" plane
	return kSteeringForce.setYtoZero();
}

// if I hit the ball, kick it.
//-----------------------------------------------------------------------------
void NetSoccerPlayer::update(const float currentTime, const float elapsedTime)
{
	BaseClass::update( currentTime, elapsedTime );
	const float distToBall = osVector3::distance (position(), m_Ball->position());
	// make it a little bit easyer for stupid humans 
	const float controlledPlayerOffset = this->isPossessed() ? 0.5f : 0.0f;
	const float sumOfRadii = radius() + m_Ball->radius()+ controlledPlayerOffset;
	if (distToBall < sumOfRadii)
	{
		if (true == this->isKicking() )
		{
			m_Ball->kick((m_Ball->position()-position())*50, elapsedTime);
		}
	}
}

//-----------------------------------------------------------------------------
// draw this character/vehicle into the scene
void NetSoccerPlayer::draw (const float currentTime, const float elapsedTime)
{
	BaseClass::draw( currentTime, elapsedTime );
    drawBasic2dCircularVehicle (*this, b_ImTeamA ? OpenSteer::Color(1.0f,0.0f,0.0f):OpenSteer::Color(0.0f,0.0f,1.0f));
	if(true == this->isPossessed() )
	{
		drawXZCircle (1.2f, this->position(), gOrange, 20);
		if (true == this->isKicking())
		{
			drawXZCircle (1.3f, this->position(), gMagenta, 20);
		}
	}
    drawTrail();
}
//-----------------------------------------------------------------------------
bool  NetSoccerPlayer::isKicking( void ) const
{
	AbstractPlayer* pkOwner = this->getPlayer();
	if (NULL != pkOwner)
	{
		const AbstractController* pkController = pkOwner->getController();
		return 0.0f != pkController->getActionValue(EControllerAction_Kick);	
	}
	return true;
	
}

//-----------------------------------------------------------------------------
void NetSoccerPlayer::setTeamIdAndPlayerNumber( bool isTeamA, unsigned int id)
{
	this->b_ImTeamA = isTeamA;
	this->m_MyID = id;
}

//-----------------------------------------------------------------------------
void NetSoccerPlayer::setHomeAndPosition(  const osVector3& kPos )
{
	this->m_home = kPos;
	this->setPosition( this->m_home );
	this->setForward( -kPos.normalized() );
	this->regenerateLocalSpace( this->forward(), 0.0f);
}

//-----------------------------------------------------------------------------
int NetSoccerPlayer::serialize( RakNet::SerializeParameters *serializeParameters ) const
{
	return BaseClass::serialize( serializeParameters );	
}

//-----------------------------------------------------------------------------
void NetSoccerPlayer::deserialize( RakNet::DeserializeParameters *deserializeParameters )
{
	BaseClass::deserialize( deserializeParameters );	
}

//-----------------------------------------------------------------------------
void NetSoccerPlayer::serializeConstruction(
	RakNet::BitStream *constructionBitstream)
{
	constructionBitstream->Write( this->b_ImTeamA );
	constructionBitstream->Write( this->m_MyID );
}

//-----------------------------------------------------------------------------
bool NetSoccerPlayer::deserializeConstruction(
	RakNet::BitStream *constructionBitstream )
{
	bool bResult = BaseClass::deserializeConstruction( constructionBitstream );
	if (true == bResult)
	{
		constructionBitstream->Read( this->b_ImTeamA );
		constructionBitstream->Read( this->m_MyID );
	}
	return bResult;
}
