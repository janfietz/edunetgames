//-----------------------------------------------------------------------------
//
//
// OpenSteer -- Steering Behaviors for Autonomous Characters
//
// Copyright (c) 2002-2005, Sony Computer Entertainment America
// Original author: Craig Reynolds <craig_reynolds@playstation.sony.com>
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
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//
//
//-----------------------------------------------------------------------------
//
//
// OpenSteer Boids
// 
// 09-26-02 cwr: created 
//
//
//-----------------------------------------------------------------------------
#include "BoidsPlugin.h"

// 10-30-09 cp/jf: modified for educational purpose

#include "EduNetApplication/EduNetGames.h"
#include "EduNetCommon/EduNetDraw.h"
#include "OpenSteerUT/CameraPlugin.h"

#include <sstream>

#include "OpenSteer/Color.h"
#include "OpenSteer/UnusedParameter.h"
#include "OpenSteerUT/AbstractVehicleGroup.h"


#ifdef WIN32
// Windows defines these as macros :(
#undef min
#undef max
#endif

#ifndef NO_LQ_BIN_STATS
#include <iomanip> // for setprecision
#include <limits> // for numeric_limits::max()
#endif // NO_LQ_BIN_STATS

// Include names declared in the OpenSteer namespace into the
// namespaces to search to find names.
using namespace OpenSteer;

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void BoidsPlugin::open (void)
{	
    // make the database used to accelerate proximity queries
    cyclePD = -1;
	this->pd = NULL;
    nextPD ();

    // make default-sized flock
    for (int i = 0; i < 100; i++) addBoidToFlock ();

    // initialize camera
    CameraPlugin::init3dCamera( *SimpleVehicle::selectedVehicle );
    Camera::camera.mode = Camera::cmFixed;
    Camera::camera.fixedDistDistance = CameraPlugin::cameraTargetDistance;
    Camera::camera.fixedDistVOffset = 0;
    Camera::camera.lookdownDistance = 20;
    Camera::camera.aimLeadTime = 0.5;
    Camera::camera.povOffset.set (0, 0.5, -2);

    // set up obstacles
    initObstacles ();
}

//-----------------------------------------------------------------------------
void BoidsPlugin::update (const float currentTime, const float elapsedTime)
{
#ifndef NO_LQ_BIN_STATS
    Boid::maxNeighbors = Boid::totalNeighbors = 0;
    Boid::minNeighbors = std::numeric_limits<int>::max();
#endif // NO_LQ_BIN_STATS

    // update flock simulation for each boid
    for (iterator i = flock.begin(); i != flock.end(); i++)
    {
		Boid* pkBoid = (*i);
		pkBoid->setParentPlugin( this );
        pkBoid->update (currentTime, elapsedTime);
    }
}

//-----------------------------------------------------------------------------
void BoidsPlugin::redraw (const float currentTime, const float elapsedTime)
{
    // selected vehicle (user can mouse click to select another)
    AbstractVehicle& selected = *SimpleVehicle::selectedVehicle;

    // vehicle nearest mouse (to be highlighted)
    AbstractVehicle& nearMouse = *OpenSteerDemo::vehicleNearestToMouse ();	

    // draw each boid in flock
    for (iterator i = flock.begin(); i != flock.end(); i++) (**i).draw ();

    // highlight vehicle nearest mouse
    OpenSteerDemo::drawCircleHighlightOnVehicle (nearMouse, 1, gGray70);

    // highlight selected vehicle
    OpenSteerDemo::drawCircleHighlightOnVehicle (selected, 1, gGray50);

	std::ostringstream status;
	const float h = drawGetWindowHeight ();
	Vec3 screenLocation (10, h-50, 0);
	AbstractVehicleGroup kVG( this->allVehicles() );
	Color kColor = gGray80;
	if (NULL != this->getVehicleFactory())
	{
		// display status in the upper left corner of the window		
		status << "[F1/F2] " << kVG.population() << " boids";
		status << "\n[F3]    PD type: ";
		switch (cyclePD)
		{
		case 0: status << "LQ bin lattice"; break;
		case 1: status << "brute force";    break;
		}
		status << "\n[F4]    Obstacles: ";
		switch (constraint)
		{
		case EBoidConstraintType_none:
			status << "none (wrap-around at sphere boundary)" ; break;
		case EBoidConstraintType_insideSphere:
			status << "inside a sphere" ; break;
		case EBoidConstraintType_outsideSphere:
			status << "inside a sphere, outside another" ; break;
		case EBoidConstraintType_outsideSpheres:
			status << "inside a sphere, outside several" ; break;
		case EBoidConstraintType_outsideSpheresNoBig:
			status << "outside several spheres, with wrap-around" ; break;
		case EBoidConstraintType_rectangle:
			status << "inside a sphere, with a rectangle" ; break;
		case EBoidConstraintType_rectangleNoBig:
			status << "a rectangle, with wrap-around" ; break;
		case EBoidConstraintType_outsideBox:
			status << "inside a sphere, outside a box" ; break;
		case EBoidConstraintType_insideBox:
			status << "inside a box" ; break;
		}
		status << std::endl;
	} else
	{
		status << "Client boids: " << kVG.population();
		screenLocation.y -= 80;
		kColor = gGray50;
	}
    
    draw2dTextAt2dLocation (status, screenLocation, kColor, drawGetWindowWidth(), drawGetWindowHeight());

    drawObstacles ();
}

//-----------------------------------------------------------------------------
void BoidsPlugin::close (void)
{
    // delete each member of the flock
	AbstractVehicleGroup kVG( this->allVehicles() );
    while (kVG.population() > 0) removeBoidFromFlock ();

    // delete the proximity database
	ET_SAFE_DELETE( pd );    
}

//-----------------------------------------------------------------------------
void BoidsPlugin::reset (void)
{
    // reset each boid in flock
    for (iterator i = flock.begin(); i != flock.end(); i++) (**i).reset();

    // reset camera position
    CameraPlugin::position3dCamera( *SimpleVehicle::selectedVehicle );

    // make camera jump immediately to new position
    Camera::camera.doNotSmoothNextMove ();
}


//-----------------------------------------------------------------------------
// for purposes of demonstration, allow cycling through various
// types of proximity databases.  this routine is called when the
// OpenSteerDemo user pushes a function key.
void BoidsPlugin::nextPD (void)
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
            const float div = 10.0f;
            const Vec3 divisions (div, div, div);
            const float diameter = Boid::worldRadius * 1.1f * 2;
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
    for (iterator i=flock.begin(); i!=flock.end(); i++) (**i).newPD(*pd);

    // delete old PD (if any)
    delete oldPD;
}

//-----------------------------------------------------------------------------
void BoidsPlugin::handleFunctionKeys (int keyNumber)
{
    switch (keyNumber)
    {
    case 1:  addBoidToFlock ();         break;
    case 2:  removeBoidFromFlock ();    break;
    case 3:  nextPD ();                 break;
    case 4:  nextBoundaryCondition ();  break;
    case 5:  printLQbinStats ();        break;
    }
}

//-----------------------------------------------------------------------------
void addBoid(GLUI_Control* pkControl )
{
	BoidsPlugin* pkPlugin = (BoidsPlugin*)pkControl->ptr_val;
	pkPlugin->addBoidToFlock();
}

//-----------------------------------------------------------------------------
void removeBoid(GLUI_Control* pkControl )
{
	BoidsPlugin* pkPlugin = (BoidsPlugin*)pkControl->ptr_val;
	pkPlugin->removeBoidFromFlock();
}


//-----------------------------------------------------------------------------
void BoidsPlugin::initGui( void* pkUserdata )
{
	GLUI* glui = ::getRootGLUI();
	GLUI_Panel* pluginPanel = static_cast<GLUI_Panel*>( pkUserdata );

	GLUI_Control* pkControl;
	pkControl = glui->add_button_to_panel( pluginPanel, "Add", -1, addBoid );
	pkControl->set_ptr_val( this );
	pkControl = glui->add_button_to_panel( pluginPanel, "Remove", -1, removeBoid  );
	pkControl->set_ptr_val( this );
}
//-----------------------------------------------------------------------------
void BoidsPlugin::printLQbinStats (void)
{
#ifndef NO_LQ_BIN_STATS
	AbstractVehicleGroup kVG( this->allVehicles() );
    int min, max; float average;
    Boid& aBoid = **(flock.begin());
    aBoid.proximityToken->getBinPopulationStats (min, max, average);
    std::cout << std::setprecision (2)
              << std::setiosflags (std::ios::fixed);
    std::cout << "Bin populations: min, max, average: "
              << min << ", " << max << ", " << average
              << " (non-empty bins)" << std::endl; 
    std::cout << "Boid neighbors:  min, max, average: "
              << Boid::minNeighbors << ", "
              << Boid::maxNeighbors << ", "
              << ((float)Boid::totalNeighbors) / ((float)kVG.population())
              << std::endl;
#endif // NO_LQ_BIN_STATS
}

//-----------------------------------------------------------------------------
void BoidsPlugin::printMiniHelpForFunctionKeys (void) const
{
    std::ostringstream message;
    message << "Function keys handled by ";
    message << '"' << name() << '"' << ':' << std::ends;
    OpenSteerDemo::printMessage (message);
    OpenSteerDemo::printMessage ("  F1     add a boid to the flock.");
    OpenSteerDemo::printMessage ("  F2     remove a boid from the flock.");
    OpenSteerDemo::printMessage ("  F3     use next proximity database.");
    OpenSteerDemo::printMessage ("  F4     next flock boundary condition.");
    OpenSteerDemo::printMessage ("");
}

//-----------------------------------------------------------------------------
void BoidsPlugin::addBoidToFlock (void)
{    
	osAbstractVehicle* pkVehicle = this->createVehicle( ET_CID_NETBOID, pd );
	AbstractVehicleGroup kVG( this->allVehicles() );
	kVG.addVehicle( pkVehicle );
}


//-----------------------------------------------------------------------------
void BoidsPlugin::removeBoidFromFlock (void)
{
	AbstractVehicleGroup kVG( this->allVehicles() );
	if (kVG.population() > 0)
	{
		// save pointer to last pedestrian, then remove it from the crowd
		AbstractVehicle* boid = flock.back();
		flock.pop_back();

		// if it is SimpleVehicle's selected vehicle, unselect it
		if (boid == SimpleVehicle::selectedVehicle)
			SimpleVehicle::selectedVehicle = NULL;

		// delete the Pedestrian
		const AbstractVehicleFactory* pkFactory = this->getVehicleFactory();
		if( NULL != pkFactory )
		{
			pkFactory->destroyVehicle( boid );
		}
		boid = NULL;
	}
}  

//-----------------------------------------------------------------------------
AbstractVehicle* BoidsPlugin::createVehicle( 
	EntityClassId classId, ProximityDatabase* pd ) const
{
	AbstractVehicle* pkVehicle = NULL;
	const AbstractVehicleFactory* pkFactory = this->getVehicleFactory();
	if( NULL != pkFactory )
	{
		pkVehicle = pkFactory->createVehicle( classId, pd );
	}	
	return pkVehicle;
}
//-----------------------------------------------------------------------------
Boid::groupType::iterator BoidsPlugin::FindBoid( const Boid* pkBoid )
{
	Boid::groupType::iterator kIter = flock.begin();
	Boid::groupType::iterator kIterEnd = flock.end();
	while(kIter != kIterEnd)
	{
		if(*kIter == pkBoid)
		{
			return kIter;
		}
		++kIter;
	}
	return kIterEnd;
}
  

//-----------------------------------------------------------------------------
void BoidsPlugin::nextBoundaryCondition (void)
{    
	SetCurrentBoundaryCondition((EBoidConstraintType) ((int) constraint + 1));
}

void BoidsPlugin::SetCurrentBoundaryCondition( 
	const EBoidConstraintType  eType,
	bool bLocalChange )
{
	if(eType != this->constraint)
	{
		this->bWasLocalChange = bLocalChange;
		this->constraint = eType;
		updateObstacles ();
	}	
}
//-----------------------------------------------------------------------------
void BoidsPlugin::initObstacles (void)
{
	this->bWasLocalChange = false;
    constraint = EBoidConstraintType_none;

    insideBigSphere.radius = Boid::worldRadius;
    insideBigSphere.setSeenFrom (Obstacle::inside);

    outsideSphere0.radius = Boid::worldRadius * 0.5f;

    const float r = Boid::worldRadius * 0.33f;
    outsideSphere1.radius = r;
    outsideSphere2.radius = r;
    outsideSphere3.radius = r;
    outsideSphere4.radius = r;
    outsideSphere5.radius = r;
    outsideSphere6.radius = r;

    const float p = Boid::worldRadius * 0.5f;
    const float m = -p;
    const float z = 0.0f;
    outsideSphere1.center.set (p, z, z);
    outsideSphere2.center.set (m, z, z);
    outsideSphere3.center.set (z, p, z);
    outsideSphere4.center.set (z, m, z);
    outsideSphere5.center.set (z, z, p);
    outsideSphere6.center.set (z, z, m);

    const Vec3 tiltF = Vec3 (1.0f, 1.0f, 0.0f).normalized ();
    const Vec3 tiltS (0.0f, 0.0f, 1.0f);
    const Vec3 tiltU = Vec3 (-1.0f, 1.0f, 0.0f).normalized ();

    bigRectangle.width = 50.0f;
    bigRectangle.height = 80.0f;
    bigRectangle.setSeenFrom (Obstacle::both);
    bigRectangle.setForward (tiltF);
    bigRectangle.setSide (tiltS);
    bigRectangle.setUp (tiltU);

    outsideBigBox.width = 50.0f;
    outsideBigBox.height = 80.0f;
    outsideBigBox.depth = 20.0f;
    outsideBigBox.setForward (tiltF);
    outsideBigBox.setSide (tiltS);
    outsideBigBox.setUp (tiltU);

    insideBigBox = outsideBigBox;
    insideBigBox.setSeenFrom (Obstacle::inside);

    updateObstacles ();
}

//-----------------------------------------------------------------------------
// update Boid::obstacles list when constraint changes
void BoidsPlugin::updateObstacles (void)
{
    // first clear out obstacle list
    this->obstacles().clear ();

    // add back obstacles based on mode
    switch (constraint)
    {
    default:
        // reset for wrap-around, fall through to first case:
        constraint = EBoidConstraintType_none;
    case EBoidConstraintType_none:
        break;
    case EBoidConstraintType_insideSphere:
        this->obstacles().push_back (&insideBigSphere);
        break;
    case EBoidConstraintType_outsideSphere:
        this->obstacles().push_back (&insideBigSphere);
        this->obstacles().push_back (&outsideSphere0);
        break;
    case EBoidConstraintType_outsideSpheres:
        this->obstacles().push_back (&insideBigSphere);
    case EBoidConstraintType_outsideSpheresNoBig:
        this->obstacles().push_back (&outsideSphere1);
        this->obstacles().push_back (&outsideSphere2);
        this->obstacles().push_back (&outsideSphere3);
        this->obstacles().push_back (&outsideSphere4);
        this->obstacles().push_back (&outsideSphere5);
        this->obstacles().push_back (&outsideSphere6);
        break;
    case EBoidConstraintType_rectangle:
        this->obstacles().push_back (&insideBigSphere);
        this->obstacles().push_back (&bigRectangle);
    case EBoidConstraintType_rectangleNoBig:
        this->obstacles().push_back (&bigRectangle);
        break;
    case EBoidConstraintType_outsideBox:
        this->obstacles().push_back (&insideBigSphere);
        this->obstacles().push_back (&outsideBigBox);
        break;
    case EBoidConstraintType_insideBox:
        this->obstacles().push_back (&insideBigBox);
        break;
    }
}

//-----------------------------------------------------------------------------
void BoidsPlugin::drawObstacles (void)
{
    for (ObstacleIterator o = this->obstacles().begin();
         o != this->obstacles().end();
         o++)
    {
        (**o).draw (false, // draw in wireframe
                    ((*o == &insideBigSphere) ?
                     Color (0.2f, 0.2f, 0.4f) :
                     Color (0.1f, 0.1f, 0.2f)),
                    Camera::camera.position ());
    }
}

//-----------------------------------------------------------------------------
void BoidsPlugin::tempDrawRectangle (const RectangleObstacle& rect, const Color& color)
{
    float w = rect.width / 2;
    float h = rect.height / 2;

    Vec3 v1 = rect.globalizePosition (Vec3 ( w,  h, 0));
    Vec3 v2 = rect.globalizePosition (Vec3 (-w,  h, 0));
    Vec3 v3 = rect.globalizePosition (Vec3 (-w, -h, 0));
    Vec3 v4 = rect.globalizePosition (Vec3 ( w, -h, 0));

    drawLine (v1, v2, color);
    drawLine (v2, v3, color);
    drawLine (v3, v4, color);
    drawLine (v4, v1, color);
}

//-----------------------------------------------------------------------------
void BoidsPlugin::tempDrawBox (const BoxObstacle& box, const Color& color)
{
    const float w = box.width / 2;
    const float h = box.height / 2;
    const float d = box.depth / 2;
    const Vec3 p = box.position ();
    const Vec3 s = box.side ();
    const Vec3 u = box.up ();
    const Vec3 f = box.forward ();

    const Vec3 v1 = box.globalizePosition (Vec3 ( w,  h,  d));
    const Vec3 v2 = box.globalizePosition (Vec3 (-w,  h,  d));
    const Vec3 v3 = box.globalizePosition (Vec3 (-w, -h,  d));
    const Vec3 v4 = box.globalizePosition (Vec3 ( w, -h,  d));

    const Vec3 v5 = box.globalizePosition (Vec3 ( w,  h, -d));
    const Vec3 v6 = box.globalizePosition (Vec3 (-w,  h, -d));
    const Vec3 v7 = box.globalizePosition (Vec3 (-w, -h, -d));
    const Vec3 v8 = box.globalizePosition (Vec3 ( w, -h, -d));

    drawLine (v1, v2, color);
    drawLine (v2, v3, color);
    drawLine (v3, v4, color);
    drawLine (v4, v1, color);

    drawLine (v5, v6, color);
    drawLine (v6, v7, color);
    drawLine (v7, v8, color);
    drawLine (v8, v5, color);

    drawLine (v1, v5, color);
    drawLine (v2, v6, color);
    drawLine (v3, v7, color);
    drawLine (v4, v8, color);
}

namespace
{
	using namespace OpenSteer;
	BoidsPlugin gBoidsPlugin(true);
}

