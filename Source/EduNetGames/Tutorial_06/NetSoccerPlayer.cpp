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
#include"NetSoccerPlayer.h"


//-----------------------------------------------------------------------------
NetSoccerPlayer::NetSoccerPlayer (NetSoccerPlayer::Group others,
  NetSoccerPlayer::Group allplayers,
  NetSoccerBall* ball, bool isTeamA, int id) :
  m_others(others), m_AllPlayers(allplayers), m_Ball(ball), b_ImTeamA(isTeamA), m_MyID(id)
{
  reset ();
}

//-----------------------------------------------------------------------------
// reset state
void NetSoccerPlayer::reset (void)
{
  SimpleVehicle::reset (); // reset the vehicle
  setSpeed (0.0f);         // speed along Forward direction.
  setMaxForce (3000.7f);      // steering force is clipped to this magnitude
  setMaxSpeed (10);         // velocity is clipped to this magnitude

  m_home = position();
  setTrailParameters (10, 60);
}
//-----------------------------------------------------------------------------
// per frame simulation update
// (parameter names commented out to prevent compiler warning from "-W")
void NetSoccerPlayer::update (const float /*currentTime*/, const float elapsedTime)
{
    // if I hit the ball, kick it.

    const float distToBall = OpenSteer::Vec3::distance (position(), m_Ball->position());
    const float sumOfRadii = radius() + m_Ball->radius();
    if (distToBall < sumOfRadii)
	m_Ball->kick((m_Ball->position()-position())*50, elapsedTime);


    // otherwise consider avoiding collisions with others
    OpenSteer::Vec3 collisionAvoidance = steerToAvoidNeighbors(1, (OpenSteer::AVGroup&)m_AllPlayers);
    if(collisionAvoidance != OpenSteer::Vec3::zero)
	applySteeringForce (collisionAvoidance, elapsedTime);
    else
	{
	float distHomeToBall = OpenSteer::Vec3::distance (m_home, m_Ball->position());
	if( distHomeToBall < 12.0f)
	    {
	    // go for ball if I'm on the 'right' side of the ball
		if( b_ImTeamA ? position().x > m_Ball->position().x : position().x < m_Ball->position().x)
		{
		OpenSteer::Vec3 seekTarget = xxxsteerForSeek(m_Ball->position());
		applySteeringForce (seekTarget, elapsedTime);
		}
	    else
		{
		if( distHomeToBall < 12.0f)
		    {
		    float Z = m_Ball->position().z - position().z > 0 ? -1.0f : 1.0f;
		    OpenSteer::Vec3 behindBall = m_Ball->position() + (b_ImTeamA ? OpenSteer::Vec3(2.0f,0.0f,Z) : OpenSteer::Vec3(-2.0f,0.0f,Z));
		    OpenSteer::Vec3 behindBallForce = xxxsteerForSeek(behindBall);
		    annotationLine (position(), behindBall ,OpenSteer::Color(0.0f,1.0f,0.0f));
		    OpenSteer::Vec3 evadeTarget = xxxsteerForFlee(m_Ball->position());
		    applySteeringForce (behindBallForce*10.0f + evadeTarget, elapsedTime);
		    }
		}
	    }
	else	// Go home
	    {
	    OpenSteer::Vec3 seekTarget = xxxsteerForSeek(m_home);
	    OpenSteer::Vec3 seekHome = xxxsteerForSeek(m_home);
	    applySteeringForce (seekTarget+seekHome, elapsedTime);
	    }

	}
}
//-----------------------------------------------------------------------------
// draw this character/vehicle into the scene
void NetSoccerPlayer::draw (void)
{
    drawBasic2dCircularVehicle (*this, b_ImTeamA ? OpenSteer::Color(1.0f,0.0f,0.0f):OpenSteer::Color(0.0f,0.0f,1.0f));
    drawTrail ();
}
