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

#include "NetPedestrianPlugin.h"
#include "NetPedestrian.h"
#include "NetPedestrianFactory.h"
#include "OpenSteerUT/AbstractVehicleGroup.h"
#include "OpenSteerUT/PluginArray.h"
#include "OpenSteerUT/GridPlugin.h"
#include "OpenSteerUT/AbstractVehicleMath.h"
#include "OpenSteerUT/ZonePlugin.h"
#include "OpenSteer/SimplePlayer.h"

#include "EduNetCommon/EduNetDraw.h"
#include "EduNetConnect/NetworkPlugin.h"

#include "OpenSteerUT/CameraPlugin.h"
#include "OpenSteerUT/ZonePlugin.h"


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
		if( Camera::getLocalSpaceToTrack() == &kVehicle )
		{
			// Camera::accessInstance().mode = Camera::cmFixedDistanceOffset;
			Camera::accessInstance().mode = Camera::cmStraightDown;
			Camera::accessInstance().fixedTarget.set( 15, 0, 0 );
			Camera::accessInstance().fixedPosition.set( 20, 20, 20 );
			Camera::accessInstance().lookdownDistance = 15;
			// make camera jump immediately to new position
			Camera::accessInstance().doNotSmoothNextMove ();
		}
	}

}


//-----------------------------------------------------------------------------
/**
* Creates a path of the form of an eight. Data provided by Nick Porcino.
*/
PolylineSegmentedPathwaySingleRadius* createPath (const osVector3& offset, float scale = 1.0f)
{
	PolylineSegmentedPathwaySingleRadius* path = NULL;
//	const float pathRadius = 2.0 * scale;
//	const float pathRadius = 1.0;
	const float pathRadius = 2.0 * scale;

	const PolylineSegmentedPathwaySingleRadius::size_type pathPointCount = 16;
	osVector3 pathPoints[pathPointCount] = {
		osVector3( -12.678730011f, 0.0144290002063f, 0.523285984993f ),
		osVector3( -10.447640419f, 0.0149269998074f, -3.44138407707f ),
		osVector3( -5.88988399506f, 0.0128290001303f, -4.1717581749f ),
		osVector3( 0.941263973713f, 0.00330199999735f, 0.350513994694f ),
		osVector3( 5.83484792709f, -0.00407700007781f, 6.56243610382f ),
		osVector3( 11.0144147873f, -0.0111180003732f, 10.175157547f ),
		osVector3( 15.9621419907f, -0.0129949999973f, 8.82364273071f ),
		osVector3( 18.697883606f, -0.0102310003713f, 2.42084693909f ),
		osVector3( 16.0552558899f, -0.00506500015035f, -3.57153511047f ),
		osVector3( 10.5450153351f, 0.00284500000998f, -9.92683887482f ),
		osVector3( 5.88374519348f, 0.00683500012383f, -8.51393127441f ),
		osVector3( 3.17790007591f, 0.00419700006023f, -2.33129906654f ),
		osVector3( 1.94371795654f, 0.00101799995173f, 2.78656601906f ),
		osVector3( -1.04967498779f, 0.000867999973707f, 5.57114219666f ),
		osVector3( -7.58111476898f, 0.00634300010279f, 6.13661909103f ),
		osVector3( -12.4111375809f, 0.0108559997752f, 3.5670940876f )
	};

	for( size_t i = 0; i < pathPointCount; ++i )
	{
		pathPoints[i] *= scale;
		pathPoints[i] += offset;
	}


	path = ET_NEW PolylineSegmentedPathwaySingleRadius (pathPointCount,
		pathPoints,
		pathRadius,
		false);

	// ------------------------------------ xxxcwr11-1-04 fixing steerToAvoid
	path->setStartPoint( pathPoints[0] );
	path->setEndPoint( pathPoints[pathPointCount-1] );
	return path;
}

//-----------------------------------------------------------------------------
OpenSteer::PolylineSegmentedPathwaySingleRadius* NetPedestrianPlugin::createTestPath( float scale )
{
	return createPath( osVector3::zero, scale );
}

//-----------------------------------------------------------------------------
void NetPedestrianPlugin::setPath( OpenSteer::PolylineSegmentedPathwaySingleRadius* path )
{
	ET_SAFE_DELETE( this->m_pkPath );
	this->m_pkPath = path;
}

//-----------------------------------------------------------------------------
float NetPedestrianPlugin::selectionOrderSortKey (void) const 
{
	return 98.0f;
}

//-----------------------------------------------------------------------------
NetPedestrianPlugin::NetPedestrianPlugin( bool bAddToRegistry, float pathScale ):
BaseClass( bAddToRegistry ),
pd(NULL),
m_fLastRenderTime(0.0f),
m_fPathScale( pathScale ),
m_pkPath(NULL),
pathColor( gRed ),
m_bWanderSwitch(0),
m_bUseDirectedPathFollowing(0)
{
	this->setEntityFactory( &this->m_kOfflinePedestrianFactory );
}

//-----------------------------------------------------------------------------
NetPedestrianPlugin::~NetPedestrianPlugin() 
{
	this->close();
	ET_SAFE_DELETE( this->m_pkPath );
}

//-----------------------------------------------------------------------------
void NetPedestrianPlugin::open (void)
{
	osVector3 offset(osVector3::zero);

	AbstractPlugin* pkParent = this->getParentPlugin();
	ZonePlugin* pkParentZone = dynamic_cast<ZonePlugin*>(pkParent);
	// the parent zone
	// we assume just one parent offset
	while( NULL != pkParentZone )
	{
		offset += pkParentZone->position();
		pkParent = pkParentZone->getParentPlugin();
		pkParentZone = dynamic_cast<ZonePlugin*>(pkParent);
	}

	this->setPath( createPath( offset, this->m_fPathScale ) );

	// camera setup
	Camera::accessInstance().mode = Camera::cmFixedDistanceOffset;
	Camera::accessInstance().fixedTarget.set (15, 0, 30);
	Camera::accessInstance().fixedPosition.set (15, 70, -70);
	Camera::accessInstance().lookdownDistance = 15;

	// make the database used to accelerate proximity queries
	cyclePD = -1;
	this->nextPD();

	// create the specified number of Pedestrians
	for( int i = 0; i < gPedestrianStartCount; i++ )
	{
		this->addPedestrianToCrowd();
	}

	// initialize camera and selectedVehicle
	initPluginCamera( *SimpleVehicle::getSelectedVehicle() );
}

//-----------------------------------------------------------------------------
void NetPedestrianPlugin::close (void)
{
	AbstractVehicleGroup kVG( this->allVehicles() );
	// delete all Pedestrians
	while (kVG.population() > 0) 
		removePedestrianFromCrowd ();
}

//-----------------------------------------------------------------------------
void NetPedestrianPlugin::reset (void)
{
	// reset each Pedestrian
	AbstractVehicleGroup kVG( this->allVehicles() );
	kVG.reset();

	// reset camera position
	CameraPlugin::position2dCamera( *SimpleVehicle::getSelectedVehicle() );

	// make camera jump immediately to new position
	Camera::accessInstance().doNotSmoothNextMove ();
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
	AbstractVehicle* selected = SimpleVehicle::getSelectedVehicle();

	// update grid center
	if( NULL != SimpleVehicle::getSelectedVehicle() ) 
	{
		GridPlugin::setGridCenter( SimpleVehicle::getSelectedVehicle()->position() );
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

	AbstractPlugin* pkParent = this->getParentPlugin();
	ZonePlugin* pkParentZone = dynamic_cast<ZonePlugin*>(pkParent);
	// the parent zone
	if( NULL != pkParentZone )
	{
		// add some out put ?
	}
	else
	{
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
			status << "\n";
			if( this->m_bUseDirectedPathFollowing )
				status << "Directed path following.";
			else
				status << "Stay on the path.";
			status << "\nWander: ";
			if (this->m_bWanderSwitch)
				status << "yes"; else status << "no";
			status << std::endl;
		}
		else
		{
			status << "Client Crowd size: " << kVG.population();
			screenLocation.y -= 60.0f;
			kColor = gGray50;
		}
		draw2dTextAt2dLocation (status, screenLocation, kColor, drawGetWindowWidth(), drawGetWindowHeight());
	}

	this->m_fLastRenderTime = currentTime;
}

//-----------------------------------------------------------------------------
void NetPedestrianPlugin::drawPathAndObstacles (void)
{
	osVector3 offset(osVector3::zero);

	typedef PolylineSegmentedPathwaySingleRadius::size_type size_type;

	// draw a line along each segment of path
	const PolylineSegmentedPathwaySingleRadius& path = *this->m_pkPath;
	for (size_type i = 1; i < path.pointCount(); ++i ) 
	{
		drawLine (path.point( i )+offset, path.point( i-1)+offset , pathColor);
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
// 	case 4: NetPedestrian::gUseDirectedPathFollowing = !NetPedestrian::gUseDirectedPathFollowing; break;
// 	case 5: NetPedestrian::gWanderSwitch = !NetPedestrian::gWanderSwitch;                         break;
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
// 	EduNet::Log::printMessage ("  F4     toggle directed path follow.");
// 	EduNet::Log::printMessage ("  F5     toggle wander component on/off.");
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
	// note: the vehicle might be NULL here
	//       a client for example will not create a vehicle
	if( NULL != pkVehicle )
	{
		NetPedestrian* pkPedestrian = dynamic_cast<NetPedestrian*>(pkVehicle);
		pkPedestrian->setPath( this->m_pkPath );
		// note: now the path is valid reset the vehicle again
		pkVehicle->setRadius( 0.5 * this->m_fPathScale );
		pkVehicle->reset();
		AbstractVehicleGroup kVG( this->allVehicles() );
		kVG.addVehicleToPlugin( pkVehicle, this );
	}
}

//-----------------------------------------------------------------------------
void NetPedestrianPlugin::removePedestrianFromCrowd (void)
{
	AbstractVehicleGroup kVG( this->allVehicles() );
	if (kVG.population() > 0)
	{
		// delete the Pedestrian
		const AbstractEntityFactory* pkFactory = this->getEntityFactory();
		AVGroup::iterator it = kVG.begin();
		AVGroup::iterator itEnd = kVG.end();
		while(it != itEnd)
		{
			AbstractVehicle* pkVehicle = *it;
			if( NULL != pkFactory )
			{
				if( true == pkFactory->destroyVehicle( pkVehicle ) )
				{
					kVG.removeVehicleFromPlugin ( pkVehicle );
					return;
				}
			}
			else
			{
				kVG.removeVehicleFromPlugin ( pkVehicle );
				ET_SAFE_DELETE(pkVehicle);
				return;
			}
		}
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

		glui->add_checkbox_to_panel( pluginPanel, "Wander randomly", &this->m_bWanderSwitch);
		glui->add_checkbox_to_panel( pluginPanel, "Use direct path following", &this->m_bUseDirectedPathFollowing);
		GLUI_Control* pkControl;
		pkControl = glui->add_button_to_panel( pluginPanel, "Add", -1, addPedestrian );
		pkControl->set_ptr_val( this );
		pkControl = glui->add_button_to_panel( pluginPanel, "Remove", -1, removePedestrian  );
		pkControl->set_ptr_val( this );
	}
};





