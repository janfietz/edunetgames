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
#include "EduNetCommon/EduNetExternal.h"
#include "OpenSteerUT/SimplePhysicsVehicle.h"
#include "OpenSteerUT/AbstractVehicleMath.h"
#include "OpenSteer/AbstractPlayer.h"
#include "OpenSteerUT/AbstractVehicleUtilities.h"

using namespace OpenSteer;

//-----------------------------------------------------------------------------
#pragma warning(push)
#pragma warning(disable: 4355) // warning C4355: 'this' : used in base member initializer list
SimplePhysicsVehicle::SimplePhysicsVehicle():
	m_kEulerUpdate(this),
	m_kSteeringForceUpdate(this),
	m_fAccumulatedElapsedTime(0.0f)
{
}
#pragma warning(pop)

//-----------------------------------------------------------------------------
SimplePhysicsVehicle::~SimplePhysicsVehicle()
{

}

//-----------------------------------------------------------------------------
void SimplePhysicsVehicle::draw( const float currentTime, const float elapsedTime )
{
	if( ( SimpleVehicle::getNearestMouseVehicle() == this ) )
	{
		// highlight vehicle nearest mouse
		VehicleUtilities::highlightVehicleUtility( *this );
	}
	BaseClass::draw( currentTime, elapsedTime );

#if 0

	// highlight vehicle nearest mouse
	VehicleUtilities::drawCircleHighlightOnVehicle (nearMouse, 1, gGray70);

	// highlight selected vehicle
	VehicleUtilities::drawCircleHighlightOnVehicle (selected, 1, gGray50);

// extrapolation example
	PhysicsMotionState kMotionState;
	PhysicsMotionState kExtrapolatedMotionState;
	kMotionState.readLocalSpaceData( this->getLocalSpaceData() );

	kExtrapolatedMotionState = kMotionState;
	LocalSpace kLocalSpace;
	for( size_t i = 0; i < 51; ++i )
	{
		kMotionState.integrateMotionState(
			kExtrapolatedMotionState, this->getUpdateTickTime() );

		if( i % 10 == 0 )
		{
			kExtrapolatedMotionState.writeLocalSpaceData( kLocalSpace );
			drawBasic2dCircularLocalSpace( kLocalSpace.getLocalSpaceData(), gBlue, this->radius() );
		}
		kMotionState = kExtrapolatedMotionState;
	}
#endif
}

#if 0
//-----------------------------------------------------------------------------
void StudentVehicle::annotationUtility( void ) const
{
	osVector3 kLinearVelocity = this->m_kEulerUpdate.getMotionState().m_kLinearVelocity;
	kLinearVelocity *= 3.6f;
	osVector3 kAngularVelocity = this->m_kEulerUpdate.getMotionState().m_kAngularVelocity;
	kAngularVelocity /= OPENSTEER_M_2PI;


	osVector3 kLocalLinearVelocity = this->m_kEulerUpdate.getMotionState().m_kLocalLinearVelocity;
	kLocalLinearVelocity *= 3.6f;
	osVector3 kLocalAngularVelocity = this->m_kEulerUpdate.getMotionState().m_kLocalAngularVelocity;
	kLocalAngularVelocity /= OPENSTEER_M_2PI;

	std::ostringstream sn;
	sn << "#"
		<< this->getEntityId()
		<< std::endl;
	if( this == SimpleVehicle::getSelectedVehicle() )
	{
		sn << std::setprecision(2) << std::setiosflags(std::ios::fixed);
		sn << "--Worldspace--"
			<< std::endl;
		sn << "Position:" << this->position()
			<< std::endl;
		sn << "Linear  Vel:" << kLinearVelocity
			<< std::endl;
		sn << "Angular Vel:" << kAngularVelocity
			<< std::endl;

		sn << "--Localspace--"
			<< std::endl;
		sn << "Linear  Vel:" << kLocalLinearVelocity
			<< std::endl;
		sn << "Angular Vel:" << kLocalAngularVelocity
			<< std::endl;
	}
	sn << std::ends;
	const Color textColor (0.8f, 1, 0.8f);
	const osVector3 textOffset (0, 0.25f, 0);
	const osVector3 textPos = this->position() + textOffset;
	draw2dTextAt3dLocation( sn, textPos, textColor, drawGetWindowWidth(), drawGetWindowHeight() );


#if 0
	// textual annotation for selected Pedestrian
	if( SimpleVehicle::getSelectedVehicle() && OpenSteer::annotationIsOn() )
	{
		const Color color (0.8f, 0.8f, 1.0f);
		const osVector3 textOffset (0, 0.25f, 0);
		const osVector3 textPosition = selected->position() + textOffset;
		const osVector3 camPosition = Camera::accessInstance().position();
		const float camDistance = osVector3::distance (selected->position(),
			camPosition);
		const char* spacer = "      ";
		std::ostringstream annote;
		annote << std::setprecision (2);
		annote << std::setiosflags (std::ios::fixed);
		annote << spacer << "1: speed: " << selected->speed() << std::endl;
		annote << std::setprecision (1);
		annote << spacer << "2: cam dist: " << camDistance << std::endl;
		annote << spacer << "3: no third thing" << std::ends;
		draw2dTextAt3dLocation (annote, textPosition, color, drawGetWindowWidth(), drawGetWindowHeight());
	}

#endif
}

#endif

//-----------------------------------------------------------------------------
// per frame simulation update
void SimplePhysicsVehicle::update( const float currentTime, const float elapsedTime )
{
	// prevent any updates with zero or negative delta time
	if( elapsedTime <= 0 )
	{
		return;
	}
	this->m_fUpdateCurrentTime = currentTime;
	this->m_fUpdateElapsedTime = elapsedTime;

	if( NULL != this->getCustomUpdated() )
	{
		// in case the custom updater decides to call the base class
		// prevent infinite recursion, store the custom updater locally
		// and restore it once done with the update
		AbstractUpdated* pkCustomUpdated = this->getCustomUpdated();
		this->setCustomUpdated( NULL );
		pkCustomUpdated->updateCustom( this, currentTime, elapsedTime );
		this->setCustomUpdated( pkCustomUpdated );
		return;
	}

	{
		ET_PROFILE( updatePhysicsVehicle );
		if( this == SimpleVehicle::getSelectedVehicle() )
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

		// read client side controller data
		bool bHasControllerValues = false;
		osVector3 kControllerForce(osVector3::zero);
		AbstractPlayer* pkPlayer = this->getPlayer();
		if( NULL != pkPlayer )
		{
			const AbstractController* pkController = pkPlayer->getController();
			if( NULL != pkController )
			{
				Vec3 kLocalSteeringForce = pkController->getOutputForce();
				if( kLocalSteeringForce.length() > 0.1 )
				{
					bHasControllerValues = true;
					if( kLocalSteeringForce.x != 0.0f && this->maxForce() > 100 )
					{
						kLocalSteeringForce *= 0.1f;
					}
					OpenSteer::localToWorldSpace( *this, kLocalSteeringForce, kControllerForce );
					kControllerForce *= this->maxForce();

					const Vec3 c1 = this->position();
					const Vec3 c2 = this->position() + kControllerForce;
					const Color color = gCyan;
					this->annotationLine( c1, c2, color );
				}
				else
				{
					this->setSpeed( 0 );
				}
				bHasControllerValues = true;
			}
		}

		switch( this->m_kEulerUpdate.getUpdateMode() )
		{
		case( EEulerUpdateMode_Accelerate ):
			{
				// compute steering forces
				this->m_kSteeringForceUpdate.update( currentTime, elapsedTime );
				osVector3 kSteeringForce = this->m_kSteeringForceUpdate.getForce();

				// override steering force computation with controller values
				if( true == bHasControllerValues )
				{
					kSteeringForce.x = kControllerForce.x;
					kSteeringForce.z = kControllerForce.z;
				}
				this->m_kEulerUpdate.setForce( kSteeringForce );
			}
			break;
		case( EEulerUpdateMode_IntegrateMotionState ):
			{
			}
			break;
		}

		// integrate results
		this->m_kEulerUpdate.update( currentTime, elapsedTime );

		// finally update the current motion state
		this->m_kEulerUpdate.updateMotionState( currentTime, elapsedTime );

		// increment the updateTicks counter
		this->m_fAccumulatedElapsedTime += elapsedTime;

		const float fTickTime = this->getUpdateTickTime();
		while( this->m_fAccumulatedElapsedTime >= fTickTime )
		{
			++this->_updateTicks;
			this->m_fAccumulatedElapsedTime -= fTickTime;
		}

		// annotation
		this->annotationVelocityAcceleration (5, 0);
		this->recordTrailVertex (currentTime, position());

		if( NULL != this->m_pkProximityToken )
		{
			// notify proximity database that our position has changed
			this->m_pkProximityToken->updateForNewPosition( position() );
		}
	}
}


