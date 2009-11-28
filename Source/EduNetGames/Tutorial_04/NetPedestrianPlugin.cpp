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

#include "NetPedestrianPlugin.h"
#include "NetPedestrian.h"
#include "NetPedestrianFactory.h"
#include "OpenSteerUT/AbstractVehicleGroup.h"
#include "OpenSteerUT/PluginArray.h"
#include "OpenSteerUT/GridPlugin.h"
#include "OpenSteerUT/AbstractVehicleMath.h"
#include "OpenSteer/SimplePlayer.h"

#include "EduNetCommon/EduNetDraw.h"
#include "EduNetConnect/NetworkPlugin.h"

#include "EduNetApplication/EduNetGames.h"
#include "OpenSteerUT/CameraPlugin.h"


using namespace OpenSteer;

namespace
{
	//-------------------------------------------------------------------------
	// How many pedestrians to create when the plugin starts first?
	int const gPedestrianStartCount = 1; // 100

	void initPluginCamera( osAbstractVehicle& kVehicle )
	{
		// camera setup
		CameraPlugin::init2dCamera( kVehicle );
		// Camera::camera.mode = Camera::cmFixedDistanceOffset;
		Camera::camera.mode = Camera::cmStraightDown;
		Camera::camera.fixedTarget.set( 15, 0, 0 );
		Camera::camera.fixedPosition.set( 20, 20, 20 );
		Camera::camera.lookdownDistance = 15;
		// make camera jump immediately to new position
		Camera::camera.doNotSmoothNextMove ();

	}

}

//-----------------------------------------------------------------------------
float NetPedestrianPlugin::selectionOrderSortKey (void) const 
{
	return 98.0f;
}

//-----------------------------------------------------------------------------
NetPedestrianPlugin::NetPedestrianPlugin( bool bAddToRegistry ):
BaseClass( bAddToRegistry ),
pd(NULL),
m_fLastRenderTime(0.0f)
{
	this->setEntityFactory( &this->m_kOfflinePedestrianFactory );
}

//-----------------------------------------------------------------------------
NetPedestrianPlugin::~NetPedestrianPlugin() 
{
	this->close();
}

//-----------------------------------------------------------------------------
void NetPedestrianPlugin::open (void)
{
	// camera setup
	Camera::camera.mode = Camera::cmFixedDistanceOffset;
	Camera::camera.fixedTarget.set (15, 0, 30);
	Camera::camera.fixedPosition.set (15, 70, -70);
	Camera::camera.lookdownDistance = 15;

	// make the database used to accelerate proximity queries
	cyclePD = -1;
	this->nextPD();

	// create the specified number of Pedestrians
	for( int i = 0; i < gPedestrianStartCount; i++ )
	{
		this->addPedestrianToCrowd();
	}

	// initialize camera and selectedVehicle
	initPluginCamera( *SimpleVehicle::selectedVehicle );
}

//-----------------------------------------------------------------------------
void NetPedestrianPlugin::close (void)
{
	AbstractVehicleGroup kVG( this->allVehicles() );
	// delete all Pedestrians
	while (kVG.population() > 0) removePedestrianFromCrowd ();
}

//-----------------------------------------------------------------------------
void NetPedestrianPlugin::reset (void)
{
	// reset each Pedestrian
	AbstractVehicleGroup kVG( this->allVehicles() );
	kVG.reset();

	// reset camera position
	CameraPlugin::position2dCamera( *SimpleVehicle::selectedVehicle );

	// make camera jump immediately to new position
	Camera::camera.doNotSmoothNextMove ();
}

//-----------------------------------------------------------------------------
void NetPedestrianPlugin::update (const float currentTime, const float elapsedTime)
{
	if( false == this->isEnabled() )
	{
		return;
	}
	// update each Pedestrian
	AbstractVehicleGroup kVG( this->allVehicles() );
	kVG.update( currentTime, elapsedTime );
}

//-----------------------------------------------------------------------------
void NetPedestrianPlugin::redraw (const float currentTime, const float elapsedTime)
{
	if( false == this->isVisible() )
	{
		return;
	}
	// selected Pedestrian (user can mouse click to select another)
	AbstractVehicle* selected = SimpleVehicle::selectedVehicle;

	// update grid center
	if( NULL != SimpleVehicle::selectedVehicle ) 
	{
		GridPlugin::setGridCenter( SimpleVehicle::selectedVehicle->position() );
	}

	// draw and annotate each Pedestrian
	AbstractVehicleGroup kVG( this->allVehicles() );
	kVG.redraw( currentTime, elapsedTime );

	// draw the path they follow and obstacles they avoid
	// but only once
	if( this->m_fLastRenderTime != currentTime )
	{
		this->drawPathAndObstacles ();
	}

	// display status in the upper left corner of the window
	std::ostringstream status;
	const float h = drawGetWindowHeight ();
	osVector3 screenLocation (10, h - 50, 0);
	Color kColor = gGray80;
	if( false == this->isRemoteObject() )
	{
		status << "[F1/F2] Crowd size: " << kVG.population();
		status << "\n[F3] PD type: ";
		switch (cyclePD)
		{
		case 0: status << "LQ bin lattice"; break;
		case 1: status << "brute force";    break;
		}
		status << "\n[F4] ";
		if (NetPedestrian::gUseDirectedPathFollowing)
			status << "Directed path following.";
		else
			status << "Stay on the path.";
		status << "\n[F5] Wander: ";
		if (NetPedestrian::gWanderSwitch) status << "yes"; else status << "no";
		status << std::endl;
	}
	else
	{
		status << "Client Crowd size: " << kVG.population();
		screenLocation.y -= 60.0f;
		kColor = gGray50;
	}
	draw2dTextAt2dLocation (status, screenLocation, kColor, drawGetWindowWidth(), drawGetWindowHeight());


	this->m_fLastRenderTime = currentTime;
}

//-----------------------------------------------------------------------------
void NetPedestrianPlugin::drawPathAndObstacles (void)
{
	typedef PolylineSegmentedPathwaySingleRadius::size_type size_type;

	// draw a line along each segment of path
	const PolylineSegmentedPathwaySingleRadius& path = *getTestPath ();
	for (size_type i = 1; i < path.pointCount(); ++i ) {
		drawLine (path.point( i ), path.point( i-1) , gRed);
	}
}

//-----------------------------------------------------------------------------
void NetPedestrianPlugin::handleFunctionKeys (int keyNumber)
{
	switch (keyNumber)
	{
	case 1:  addPedestrianToCrowd ();                               break;
	case 2:  removePedestrianFromCrowd ();                          break;
	case 3:  nextPD ();                                             break;
	case 4: NetPedestrian::gUseDirectedPathFollowing = !NetPedestrian::gUseDirectedPathFollowing; break;
	case 5: NetPedestrian::gWanderSwitch = !NetPedestrian::gWanderSwitch;                         break;
	}
}

//-----------------------------------------------------------------------------
void NetPedestrianPlugin::printMiniHelpForFunctionKeys (void) const
{
	std::ostringstream message;
	message << "Function keys handled by ";
	message << '"' << name() << '"' << ':' << std::ends;
	EduNet::Log::printMessage (message);
	EduNet::Log::printMessage (message);
	EduNet::Log::printMessage ("  F1     add a pedestrian to the crowd.");
	EduNet::Log::printMessage ("  F2     remove a pedestrian from crowd.");
	EduNet::Log::printMessage ("  F3     use next proximity database.");
	EduNet::Log::printMessage ("  F4     toggle directed path follow.");
	EduNet::Log::printMessage ("  F5     toggle wander component on/off.");
	EduNet::Log::printMessage ("");
}

//-----------------------------------------------------------------------------
AbstractVehicle* NetPedestrianPlugin::createVehicle( EntityClassId classId ) const
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
void NetPedestrianPlugin::addPedestrianToCrowd (void)
{
	osAbstractVehicle* pkVehicle = this->createVehicle( ET_CID_NETPEDESTRIAN );
	AbstractVehicleGroup kVG( this->allVehicles() );
	kVG.addVehicleToPlugin( pkVehicle, this );
}

//-----------------------------------------------------------------------------
void NetPedestrianPlugin::removePedestrianFromCrowd (void)
{
	AbstractVehicleGroup kVG( this->allVehicles() );
	if (kVG.population() > 0)
	{
		// save pointer to last pedestrian, then remove it from the crowd
		AbstractVehicle* pedestrian = crowd.back();
		crowd.pop_back();

		// if it is SimpleVehicle's selected vehicle, unselect it
		if (pedestrian == SimpleVehicle::selectedVehicle)
			SimpleVehicle::selectedVehicle = NULL;

		// delete the Pedestrian
		const AbstractEntityFactory* pkFactory = this->getEntityFactory();
		if( NULL != pkFactory )
		{
			pkFactory->destroyVehicle( pedestrian );
		}
		pedestrian = NULL;
	}
}

//-----------------------------------------------------------------------------
// for purposes of demonstration, allow cycling through various
// types of proximity databases.  this routine is called when the
// OpenSteerDemo user pushes a function key.
void NetPedestrianPlugin::nextPD (void)
{
	// save pointer to old PD
	ProximityDatabase* oldPD = pd;

	// allocate new PD
	const int totalPD = 2;
	switch (cyclePD = (cyclePD + 1) % totalPD)
	{
	case 0:
		{
			const osVector3 center;
			const float div = 20.0f;
			const osVector3 divisions (div, 1.0f, div);
			const float diameter = 80.0f; //XXX need better way to get this
			const osVector3 dimensions (diameter, diameter, diameter);
			typedef LQProximityDatabase<AbstractVehicle*> LQPDAV;
			pd = ET_NEW LQPDAV (center, dimensions, divisions);
			break;
		}
	case 1:
		{
			pd = ET_NEW BruteForceProximityDatabase<AbstractVehicle*> ();
			break;
		}
	}

	// switch each vehicle to new PD
	AbstractVehicleGroup kVG( this->allVehicles() );
	kVG.allocateProximityToken(pd);

	// delete old PD (if any)
	ET_SAFE_DELETE( oldPD );
}

//-----------------------------------------------------------------------------
const AVGroup& NetPedestrianPlugin::allVehicles (void) const 
{
	return crowd;
}
//-----------------------------------------------------------------------------
AVGroup& NetPedestrianPlugin::allVehicles (void) 
{
	return crowd;
}

//-----------------------------------------------------------------------------
void addPedestrian(GLUI_Control* pkControl )
{
	NetPedestrianPlugin* pkPlugin = (NetPedestrianPlugin*)pkControl->ptr_val;
	pkPlugin->addPedestrianToCrowd();
}

//-----------------------------------------------------------------------------
void removePedestrian(GLUI_Control* pkControl )
{
	NetPedestrianPlugin* pkPlugin = (NetPedestrianPlugin*)pkControl->ptr_val;
	pkPlugin->removePedestrianFromCrowd();
}

//-----------------------------------------------------------------------------
// implement to initialize additional gui functionality
void NetPedestrianPlugin::initGui( void* pkUserdata ) 
{
	if( false == this->isRemoteObject() )
	{		

		GLUI* glui = ::getRootGLUI();
		GLUI_Panel* pluginPanel = static_cast<GLUI_Panel*>( pkUserdata );

		GLUI_Control* pkControl;
		pkControl = glui->add_button_to_panel( pluginPanel, "Add", -1, addPedestrian );
		pkControl->set_ptr_val( this );
		pkControl = glui->add_button_to_panel( pluginPanel, "Remove", -1, removePedestrian  );
		pkControl->set_ptr_val( this );
	}
};





