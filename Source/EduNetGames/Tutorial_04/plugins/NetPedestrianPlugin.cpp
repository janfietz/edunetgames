#include "NetPedestrianPlugin.h"

namespace OpenSteer
{

}
using namespace OpenSteer;
#include "NetPedestrian.h"


#include "EduNetGames.h"

namespace
{
	// ----------------------------------------------------------------------------
	// How many pedestrians to create when the plugin starts first?
	int const gPedestrianStartCount = 1; // 100

}

class NetPedestrianPlugIn : public Plugin
{
public:

	const char* name (void) const {return "NetPedestrians";}

	float selectionOrderSortKey (void) const {return 98.0f;}

	virtual ~NetPedestrianPlugIn() {}// be more "nice" to avoid a compiler warning

	void open (void)
	{
		// make the database used to accelerate proximity queries
		cyclePD = -1;
		nextPD ();

		// create the specified number of Pedestrians
		population = 0;
		for (int i = 0; i < gPedestrianStartCount; i++) addPedestrianToCrowd ();

		// initialize camera and selectedVehicle
		NetPedestrian& firstPedestrian = **crowd.begin();
		OpenSteerDemo::init3dCamera (firstPedestrian);
		OpenSteerDemo::camera.mode = Camera::cmFixedDistanceOffset;
		OpenSteerDemo::camera.fixedTarget.set (15, 0, 30);
		OpenSteerDemo::camera.fixedPosition.set (15, 70, -70);
	}

	void update (const float currentTime, const float elapsedTime)
	{
		// update each NetPedestrian
		for (iterator i = crowd.begin(); i != crowd.end(); i++)
		{
			(**i).update (currentTime, elapsedTime);
		}
	}

	void redraw (const float currentTime, const float elapsedTime)
	{
		// selected NetPedestrian (user can mouse click to select another)
		AbstractVehicle& selected = *OpenSteerDemo::selectedVehicle;

		// NetPedestrian nearest mouse (to be highlighted)
		AbstractVehicle& nearMouse = *OpenSteerDemo::vehicleNearestToMouse ();

		// update camera
		OpenSteerDemo::updateCamera (currentTime, elapsedTime, selected);

		// draw "ground plane"
		if (OpenSteerDemo::selectedVehicle) gridCenter = selected.position();
		OpenSteerDemo::gridUtility (gridCenter);

		// draw and annotate each NetPedestrian
		for (iterator i = crowd.begin(); i != crowd.end(); i++) (**i).draw (); 

		// draw the path they follow and obstacles they avoid
		drawPathAndObstacles ();

		// highlight NetPedestrian nearest mouse
		OpenSteerDemo::highlightVehicleUtility (nearMouse);

		// textual annotation (at the vehicle's screen position)
		serialNumberAnnotationUtility (selected, nearMouse);

		// textual annotation for selected NetPedestrian
		if (OpenSteerDemo::selectedVehicle && OpenSteer::annotationIsOn())
		{
			const Color color (0.8f, 0.8f, 1.0f);
			const Vec3 textOffset (0, 0.25f, 0);
			const Vec3 textPosition = selected.position() + textOffset;
			const Vec3 camPosition = OpenSteerDemo::camera.position();
			const float camDistance = Vec3::distance (selected.position(),
				camPosition);
			const char* spacer = "      ";
			std::ostringstream annote;
			annote << std::setprecision (2);
			annote << std::setiosflags (std::ios::fixed);
			annote << spacer << "1: speed: " << selected.speed() << std::endl;
			annote << std::setprecision (1);
			annote << spacer << "2: cam dist: " << camDistance << std::endl;
			annote << spacer << "3: no third thing" << std::ends;
			draw2dTextAt3dLocation (annote, textPosition, color, drawGetWindowWidth(), drawGetWindowHeight());
		}

		// display status in the upper left corner of the window
		std::ostringstream status;
		status << "[F1/F2] Crowd size: " << population;
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
		const Vec3 screenLocation (10, h-50, 0);
		draw2dTextAt2dLocation (status, screenLocation, gGray80, drawGetWindowWidth(), drawGetWindowHeight());
	}


	void serialNumberAnnotationUtility (const AbstractVehicle& selected,
		const AbstractVehicle& nearMouse)
	{
		// display a NetPedestrian's serial number as a text label near its
		// screen position when it is near the selected vehicle or mouse.
		if (&selected && &nearMouse && OpenSteer::annotationIsOn())
		{
			for (iterator i = crowd.begin(); i != crowd.end(); i++)
			{
				AbstractVehicle* vehicle = *i;
				const float nearDistance = 6;
				const Vec3& vp = vehicle->position();
				const Vec3& np = nearMouse.position();
				if ((Vec3::distance (vp, selected.position()) < nearDistance)
					||
					(&nearMouse && (Vec3::distance (vp, np) < nearDistance)))
				{
					std::ostringstream sn;
					sn << "#"
						<< ((NetPedestrian*)vehicle)->serialNumber
						<< std::ends;
					const Color textColor (0.8f, 1, 0.8f);
					const Vec3 textOffset (0, 0.25f, 0);
					const Vec3 textPos = vehicle->position() + textOffset;
					draw2dTextAt3dLocation (sn, textPos, textColor, drawGetWindowWidth(), drawGetWindowHeight());
				}
			}
		}
	}

	void drawPathAndObstacles (void)
	{
		typedef PolylineSegmentedPathwaySingleRadius::size_type size_type;

		// draw a line along each segment of path
		const PolylineSegmentedPathwaySingleRadius& path = *getTestPath ();
		for (size_type i = 1; i < path.pointCount(); ++i ) {
			drawLine (path.point( i ), path.point( i-1) , gRed);
		}

	}

	void close (void)
	{
		// delete all Pedestrians
		while (population > 0) removePedestrianFromCrowd ();
	}

	void reset (void)
	{
		// reset each NetPedestrian
		for (iterator i = crowd.begin(); i != crowd.end(); i++) (**i).reset ();

		// reset camera position
		OpenSteerDemo::position2dCamera (*OpenSteerDemo::selectedVehicle);

		// make camera jump immediately to new position
		OpenSteerDemo::camera.doNotSmoothNextMove ();
	}

	void handleFunctionKeys (int keyNumber)
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

	void printMiniHelpForFunctionKeys (void) const
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


	void addPedestrianToCrowd (void)
	{
		population++;
		NetPedestrian* pedestrian = new NetPedestrian (*pd);
		crowd.push_back (pedestrian);
		if (population == 1) OpenSteerDemo::selectedVehicle = pedestrian;
	}


	void removePedestrianFromCrowd (void)
	{
		if (population > 0)
		{
			// save pointer to last pedestrian, then remove it from the crowd
			const NetPedestrian* pedestrian = crowd.back();
			crowd.pop_back();
			population--;

			// if it is OpenSteerDemo's selected vehicle, unselect it
			if (pedestrian == OpenSteerDemo::selectedVehicle)
				OpenSteerDemo::selectedVehicle = NULL;

			// delete the NetPedestrian
			delete pedestrian;
		}
	}


	// for purposes of demonstration, allow cycling through various
	// types of proximity databases.  this routine is called when the
	// OpenSteerDemo user pushes a function key.
	void nextPD (void)
	{
		// save pointer to old PD
		ProximityDatabase* oldPD = pd;

		// allocate new PD
		const int totalPD = 2;
		switch (cyclePD = (cyclePD + 1) % totalPD)
		{
		case 0:
			{
				const Vec3 center;
				const float div = 20.0f;
				const Vec3 divisions (div, 1.0f, div);
				const float diameter = 80.0f; //XXX need better way to get this
				const Vec3 dimensions (diameter, diameter, diameter);
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

		// switch each boid to new PD
		for (iterator i=crowd.begin(); i!=crowd.end(); i++) (**i).newPD(*pd);

		// delete old PD (if any)
		delete oldPD;
	}


	const AVGroup& allVehicles (void) const {return (const AVGroup&) crowd;}

	// crowd: a group (STL vector) of all Pedestrians
	NetPedestrian::groupType crowd;
	typedef NetPedestrian::groupType::const_iterator iterator;

	Vec3 gridCenter;

	// pointer to database used to accelerate proximity queries
	ProximityDatabase* pd;

	// keep track of current flock size
	int population;

	// which of the various proximity databases is currently in use
	int cyclePD;
};


NetPedestrianPlugIn gPedestrianPlugIn;
