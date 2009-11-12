#include "NetPedestrianPlugin.h"
#include "NetPedestrian.h"
#include "NetPedestrianFactory.h"
#include "OpenSteerUT/AbstractVehicleGroup.h"


#include "EduNetGames.h"

#include "EduNetCommon/EduNetDraw.h"

using namespace OpenSteer;

namespace
{
	//-------------------------------------------------------------------------
	// How many pedestrians to create when the plugin starts first?
	int const gPedestrianStartCount = 1; // 100
}

//-----------------------------------------------------------------------------
const char* NetPedestrianPlugin::name (void) const 
{
	return "NetPedestrians";
}

//-----------------------------------------------------------------------------
float NetPedestrianPlugin::selectionOrderSortKey (void) const 
{
	return 98.0f;
}

//-----------------------------------------------------------------------------
NetPedestrianPlugin::~NetPedestrianPlugin() 
{
	this->setNetPedestrianFactory( NULL );
	this->close();
}

//-----------------------------------------------------------------------------
void NetPedestrianPlugin::open (void)
{
	if(0 == this->m_pkPedestrianFactory)
	{
		this->setNetPedestrianFactory( new NetPedestrianFactory() );
	}

	// make the database used to accelerate proximity queries
	cyclePD = -1;
	this->nextPD ();

	// create the specified number of Pedestrians
	for (int i = 0; i < gPedestrianStartCount; i++)
	{
		this->addPedestrianToCrowd();
	}

	// initialize camera and selectedVehicle
	const AVGroup& kAllVehicles = this->allVehicles();
	if( kAllVehicles.size() > 0 )
	{
		AbstractVehicle* pkVehicle = *kAllVehicles.begin();
		if( NULL != pkVehicle )
		{
			AbstractVehicle& firstPedestrian = *pkVehicle;
			OpenSteerDemo::init3dCamera( firstPedestrian );
		}
	}
	OpenSteerDemo::camera.mode = Camera::cmFixedDistanceOffset;
	OpenSteerDemo::camera.fixedTarget.set (15, 0, 30);
	OpenSteerDemo::camera.fixedPosition.set (15, 70, -70);
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
	if( NULL != OpenSteerDemo::selectedVehicle )
	{
		OpenSteerDemo::position2dCamera (*OpenSteerDemo::selectedVehicle);
	}

	// make camera jump immediately to new position
	OpenSteerDemo::camera.doNotSmoothNextMove ();
}

//-----------------------------------------------------------------------------
void NetPedestrianPlugin::update (const float currentTime, const float elapsedTime)
{
	// update each Pedestrian
	AbstractVehicleGroup kVG( this->allVehicles() );
	kVG.update( currentTime, elapsedTime );
}

//-----------------------------------------------------------------------------
void NetPedestrianPlugin::redraw (const float currentTime, const float elapsedTime)
{
	// selected Pedestrian (user can mouse click to select another)
	AbstractVehicle* selected = OpenSteerDemo::selectedVehicle;

	// Pedestrian nearest mouse (to be highlighted)
	AbstractVehicle* nearMouse = OpenSteerDemo::vehicleNearestToMouse();

	// update camera
	if( NULL != selected )
	{
		OpenSteerDemo::updateCamera (currentTime, elapsedTime, *selected);

		// draw "ground plane"
		if (OpenSteerDemo::selectedVehicle) gridCenter = selected->position();
		OpenSteerDemo::gridUtility (gridCenter);
	}

	// draw and annotate each Pedestrian
	AbstractVehicleGroup kVG( this->allVehicles() );
	kVG.redraw( currentTime, elapsedTime );

	// draw the path they follow and obstacles they avoid
	drawPathAndObstacles ();

	if( ( NULL != nearMouse ) && ( NULL != selected ) )
	{
		// highlight Pedestrian nearest mouse
		OpenSteerDemo::highlightVehicleUtility (*nearMouse);

		// textual annotation (at the vehicle's screen position)
		serialNumberAnnotationUtility(*selected, *nearMouse);
	}

	// textual annotation for selected Pedestrian
	if (OpenSteerDemo::selectedVehicle && OpenSteer::annotationIsOn())
	{
		const Color color (0.8f, 0.8f, 1.0f);
		const osVector3 textOffset (0, 0.25f, 0);
		const osVector3 textPosition = selected->position() + textOffset;
		const osVector3 camPosition = OpenSteerDemo::camera.position();
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

	// display status in the upper left corner of the window
	std::ostringstream status;
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
	const float h = drawGetWindowHeight ();
	const osVector3 screenLocation (10, h-50, 0);
	draw2dTextAt2dLocation (status, screenLocation, gGray80, drawGetWindowWidth(), drawGetWindowHeight());
}

//-----------------------------------------------------------------------------
void NetPedestrianPlugin::serialNumberAnnotationUtility (const AbstractVehicle& selected,
	const AbstractVehicle& nearMouse)
{
	// display a Pedestrian's serial number as a text label near its
	// screen position when it is near the selected vehicle or mouse.
	if (&selected && &nearMouse && OpenSteer::annotationIsOn())
	{
		const AVGroup& kAllVehicles = this->allVehicles();
		for (AVGroup::const_iterator i = kAllVehicles.begin(); i != kAllVehicles.end(); i++)
		{
			AbstractVehicle* vehicle = *i;
			const float nearDistance = 6;
			const osVector3& vp = vehicle->position();
			const osVector3& np = nearMouse.position();
			if ((osVector3::distance (vp, selected.position()) < nearDistance)
				||
				(&nearMouse && (osVector3::distance (vp, np) < nearDistance)))
			{
				std::ostringstream sn;
				sn << "#"
					<< vehicle->getEntityId()
					<< std::ends;
				const Color textColor (0.8f, 1, 0.8f);
				const osVector3 textOffset (0, 0.25f, 0);
				const osVector3 textPos = vehicle->position() + textOffset;
				draw2dTextAt3dLocation (sn, textPos, textColor, drawGetWindowWidth(), drawGetWindowHeight());
			}
		}
	}
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
	OpenSteerDemo::printMessage (message);
	OpenSteerDemo::printMessage (message);
	OpenSteerDemo::printMessage ("  F1     add a pedestrian to the crowd.");
	OpenSteerDemo::printMessage ("  F2     remove a pedestrian from crowd.");
	OpenSteerDemo::printMessage ("  F3     use next proximity database.");
	OpenSteerDemo::printMessage ("  F4     toggle directed path follow.");
	OpenSteerDemo::printMessage ("  F5     toggle wander component on/off.");
	OpenSteerDemo::printMessage ("");
}

//-----------------------------------------------------------------------------
void NetPedestrianPlugin::addPedestrianToCrowd (void)
{
	NetPedestrian* pedestrian = this->m_pkPedestrianFactory->CreateNetPedestrian( *pd );
	this->addPedestrianToCrowd( pedestrian );
}

//-----------------------------------------------------------------------------
void NetPedestrianPlugin::addPedestrianToCrowd( NetPedestrian* pkVehicle )
{
	if( NULL == pkVehicle )
	{
		return;
	}
	crowd.push_back( pkVehicle );
	AbstractVehicleGroup kVG( this->allVehicles() );
	if (kVG.population() == 1) OpenSteerDemo::selectedVehicle = pkVehicle;
}

//-----------------------------------------------------------------------------
void NetPedestrianPlugin::removePedestrianFromCrowd (void)
{
	AbstractVehicleGroup kVG( this->allVehicles() );
	if (kVG.population() > 0)
	{
		// save pointer to last pedestrian, then remove it from the crowd
		const AbstractVehicle* pedestrian = crowd.back();
		crowd.pop_back();

		// if it is OpenSteerDemo's selected vehicle, unselect it
		if (pedestrian == OpenSteerDemo::selectedVehicle)
			OpenSteerDemo::selectedVehicle = NULL;

		// delete the Pedestrian
		this->m_pkPedestrianFactory->DestroyNetPedestrian( pedestrian );
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
			pd = new LQPDAV (center, dimensions, divisions);
			break;
		}
	case 1:
		{
			pd = new BruteForceProximityDatabase<AbstractVehicle*> ();
			break;
		}
	}

	// switch each vehicle to new PD
	AbstractVehicleGroup kVG( this->allVehicles() );
	kVG.newPD(*pd);

	// delete old PD (if any)
	ET_SAFE_DELETE( oldPD );
}

//-----------------------------------------------------------------------------
const AVGroup& NetPedestrianPlugin::allVehicles (void) const 
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
	GLUI* glui = ::getRootGLUI();
	GLUI_Panel* pluginPanel = static_cast<GLUI_Panel*>( pkUserdata );

	GLUI_Control* pkControl;
	pkControl = glui->add_button_to_panel( pluginPanel, "Add", -1, addPedestrian );
	pkControl->set_ptr_val( this );
	pkControl = glui->add_button_to_panel( pluginPanel, "Remove", -1, removePedestrian  );
	pkControl->set_ptr_val( this );



};

//-----------------------------------------------------------------------------
void NetPedestrianPlugin::setNetPedestrianFactory( class NetPedestrianFactory* pkFactory )
{
	if( this->m_pkPedestrianFactory != pkFactory )
	{
		ET_SAFE_DELETE( this->m_pkPedestrianFactory );
		this->m_pkPedestrianFactory = pkFactory;
	}
}

NetPedestrianPlugin gPedestrianPlugin;




