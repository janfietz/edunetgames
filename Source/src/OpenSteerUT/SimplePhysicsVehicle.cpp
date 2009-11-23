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

#include "SimplePhysicsVehicle.h"
#include "AbstractVehicleMath.h"
#include "OpenSteer/AbstractPlayer.h"

using namespace OpenSteer;

//-----------------------------------------------------------------------------
#pragma warning(push)
#pragma warning(disable: 4355) // warning C4355: 'this' : used in base member initializer list
SimplePhysicsVehicle::SimplePhysicsVehicle():
	m_kEulerUpdate(this),
	m_kSteeringForceUpdate(this)
{ 
}
#pragma warning(pop)

//-----------------------------------------------------------------------------
SimplePhysicsVehicle::~SimplePhysicsVehicle()
{

}

//-----------------------------------------------------------------------------
// per frame simulation update
void SimplePhysicsVehicle::update (const float currentTime, const float elapsedTime)
{
	if( this == SimpleVehicle::selectedVehicle )
	{
		if( false == this->isRemoteObject() )
		{
			this->setAnnotationMode( OpenSteer::EAnnotationMode_local );
		}
		else
		{
			this->setAnnotationMode( OpenSteer::EAnnotationMode_global );
		}
	}
	else
	{
		this->setAnnotationMode( OpenSteer::EAnnotationMode_global );
	}

	// craigs way ...
	// apply steering force to our momentum
	//	applySteeringForce (determineCombinedSteering (elapsedTime),
	//		elapsedTime);

	// read client side controller data
	bool bHasController = false;
	osVector3 kSteeringForce, kControllerForce(osVector3::zero);
	AbstractPlayer* pkPlayer = this->getPlayer();
	if( NULL != pkPlayer )
	{
		const AbstractController* pkController = pkPlayer->getController();
		if( NULL != pkController )
		{
			bHasController = true;
			Vec3 kLocalSteeringForce = pkController->getOutputForce();
			if( kLocalSteeringForce.length() > 0.1 )
			{
				localToWorldSpace( *this, kLocalSteeringForce, kControllerForce );
				kControllerForce *= this->maxForce();

				const Vec3 c1 = this->position();
				const Vec3 c2 = this->position() + kControllerForce;
				const Color color = gCyan;
				annotationLine (c1, c2, color);
			}
		}
	}

	// compute steering forces
	this->m_kSteeringForceUpdate.update( currentTime, elapsedTime );
	kSteeringForce = this->m_kSteeringForceUpdate.getForce();

	// override steering force computation with controller values
	if( true == bHasController )
	{
		kSteeringForce.x = kControllerForce.x;
		kSteeringForce.z = kControllerForce.z;
	}

	// integrate results
	this->m_kEulerUpdate.setForce( kSteeringForce );
	this->m_kEulerUpdate.update( currentTime, elapsedTime );

#if 0
	// reverse direction when we reach an endpoint
	if (gUseDirectedPathFollowing)
	{
		const Color darkRed (0.7f, 0, 0);
		float const pathRadius = path->radius();

		if (osVector3::distance (position(), gEndpoint0) < pathRadius )
		{
			pathDirection = +1;
			annotationXZCircle (pathRadius, gEndpoint0, darkRed, 20);
		}
		if (osVector3::distance (position(), gEndpoint1) < pathRadius )
		{
			pathDirection = -1;
			annotationXZCircle (pathRadius, gEndpoint1, darkRed, 20);
		}
	}
#endif

	// annotation
	annotationVelocityAcceleration (5, 0);
	recordTrailVertex (currentTime, position());

	if( NULL != this->m_pkProximityToken )
	{
		// notify proximity database that our position has changed
		m_pkProximityToken->updateForNewPosition (position());
	}
}


