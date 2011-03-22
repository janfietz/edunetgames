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

#include "EduNetCommon/EduNetDraw.h"

#include <sstream>

#include "OpenSteer/Color.h"
#include "OpenSteer/UnusedParameter.h"
#include "OpenSteerUT/AbstractVehicleGroup.h"
#include "OpenSteerUT/CameraPlugin.h"

#ifdef WIN32
// Windows defines these as macros :(
#undef min
#undef max
#endif

#ifndef NO_LQ_BIN_STATS
#include <iomanip> // for setprecision
#include <limits> // for numeric_limits::max()
#endif // NO_LQ_BIN_STATS

#include "NetBoidGameLogic.h"
#include "NetBoidEntityFactory.h"

#include "wx/spinctrl.h"

// Include names declared in the OpenSteer namespace into the
// namespaces to search to find names.
using namespace OpenSteer;

namespace
{
#ifdef ET_DEBUG
size_t uiInitialFlockSize = 10;
#else
size_t uiInitialFlockSize = 50;
#endif
}

//-----------------------------------------------------------------------------
NetBoidsPlugin::NetBoidsPlugin ( bool bAddToRegistry ) :
        BaseClass ( bAddToRegistry ),
		pd(NULL)
{	
	this->m_boidFactory = ET_NEW NetBoidFactory();	
	this->setEntityFactory(m_boidFactory);
}

//-----------------------------------------------------------------------------
NetBoidsPlugin::~NetBoidsPlugin ( ) 
{	
	this->setEntityFactory( NULL );
	ET_SAFE_DELETE (m_boidFactory );	
}

//-----------------------------------------------------------------------------
void NetBoidsPlugin::open ( void )
{
	
    // make the database used to accelerate proximity queries
    cyclePD = -1;
    this->pd = NULL;
    nextPD ();

    if ( false == this->isRemoteObject() )
    {
        // make default-sized flock
        for ( size_t i = 0; i < uiInitialFlockSize; i++ )
            addBoidToFlock ();
    }

    // initialize camera
	m_pCameraPlugin = ET_NEW CameraPlugin();

    m_pCameraPlugin->init3dCamera ( *SimpleVehicle::getSelectedVehicle() );
	Camera& kCam(m_pCameraPlugin->accessCamera());
    kCam.mode = Camera::cmFixed;
    kCam.fixedDistDistance = CameraPlugin::cameraTargetDistance;
    kCam.fixedDistVOffset = 0;
    kCam.lookdownDistance = 20;
    kCam.aimLeadTime = 0.5;
    kCam.povOffset.set ( 0, 0.5, -2 );

    // set up obstacles
    initObstacles ();
}

//-----------------------------------------------------------------------------
void NetBoidsPlugin::update ( const float currentTime, const float elapsedTime )
{
    if ( false == this->isEnabled() )
    {
        return;
    }
    NetBoidGameLogic kGameLogic;
    kGameLogic.setPlugin ( this );
    kGameLogic.update ( currentTime, elapsedTime );

    AbstractVehicleGroup kVG ( this->allVehicles() );
    kVG.setCustomUpdated ( &kGameLogic );
    kVG.update ( currentTime, elapsedTime );

#ifndef NO_LQ_BIN_STATS
    Boid::maxNeighbors = Boid::totalNeighbors = 0;
    Boid::minNeighbors = std::numeric_limits<int>::max();
#endif // NO_LQ_BIN_STATS
	
	m_pCameraPlugin->update( currentTime, elapsedTime);
}

//-----------------------------------------------------------------------------
void NetBoidsPlugin::redraw ( AbstractRenderer* pRenderer,
	const float currentTime, const float elapsedTime )
{
	m_pCameraPlugin->redraw(pRenderer, currentTime, elapsedTime);

    if ( false == this->isVisible() )
    {
        return;
    }
    // draw each boid in flock
    for ( Boid::groupType::iterator i = flock.begin(); i != flock.end(); i++ )
        ( **i ).draw ( pRenderer, currentTime, elapsedTime );


    std::ostringstream status;
    const float h = pRenderer->drawGetWindowHeight ();
    Vec3 screenLocation ( 10, h-50, 0 );
    AbstractVehicleGroup kVG ( this->allVehicles() );
    Color kColor = gGray80;
    if ( NULL != this->getEntityFactory() )
    {
        // display status in the upper left corner of the window
        status << "[F1/F2] " << kVG.population() << " boids";
        status << "\n[F3]    PD type: ";
        switch ( cyclePD )
        {
        case 0:
            status << "LQ bin lattice";
            break;
        case 1:
            status << "brute force";
            break;
        }
        status << "\n[F4]    Obstacles: ";
        switch ( constraint )
        {
        case EBoidConstraintType_none:
            status << "none (wrap-around at sphere boundary)" ;
            break;
        case EBoidConstraintType_insideSphere:
            status << "inside a sphere" ;
            break;
        case EBoidConstraintType_outsideSphere:
            status << "inside a sphere, outside another" ;
            break;
        case EBoidConstraintType_outsideSpheres:
            status << "inside a sphere, outside several" ;
            break;
        case EBoidConstraintType_outsideSpheresNoBig:
            status << "outside several spheres, with wrap-around" ;
            break;
        case EBoidConstraintType_rectangle:
            status << "inside a sphere, with a rectangle" ;
            break;
        case EBoidConstraintType_rectangleNoBig:
            status << "a rectangle, with wrap-around" ;
            break;
        case EBoidConstraintType_outsideBox:
            status << "inside a sphere, outside a box" ;
            break;
        case EBoidConstraintType_insideBox:
            status << "inside a box" ;
            break;
        }
        status << std::endl;
    }
    else
    {
        status << "Client boids: " << kVG.population();
        screenLocation.y -= 80;
        kColor = gGray50;
    }

    pRenderer->draw2dTextAt2dLocation ( status, screenLocation, kColor, 
		pRenderer->drawGetWindowWidth(), 
		pRenderer->drawGetWindowHeight() );

    drawObstacles (pRenderer);

	
}

//-----------------------------------------------------------------------------
void NetBoidsPlugin::close ( void )
{
    // delete each member of the flock
    AbstractVehicleGroup kVG ( this->allVehicles() );
    while ( kVG.population() > 0 ) removeBoidFromFlock ();

    // delete the proximity database
    ET_SAFE_DELETE ( pd );

	ET_SAFE_DELETE(m_pCameraPlugin)
}

//-----------------------------------------------------------------------------
void NetBoidsPlugin::reset ( void )
{
    // reset each boid in flock
    for ( Boid::groupType::iterator i = flock.begin(); i != flock.end(); i++ )
        ( **i ).reset();

    // reset camera position
	m_pCameraPlugin->position3dCamera ( *SimpleVehicle::getSelectedVehicle() );

    // make camera jump immediately to new position
    m_pCameraPlugin->accessCamera().doNotSmoothNextMove ();
}


//-----------------------------------------------------------------------------
// for purposes of demonstration, allow cycling through various
// types of proximity databases.  this routine is called when the
// OpenSteerDemo user pushes a function key.
void NetBoidsPlugin::nextPD ( void )
{
    // save pointer to old PD
    ProximityDatabase* oldPD = pd;

    // allocate new PD
    const int totalPD = 2;
    switch ( cyclePD = ( cyclePD + 1 ) % totalPD )
    {
    case 0:
    {
        const Vec3 center;
        const float div = 10.0f;
        const Vec3 divisions ( div, div, div );
        const float diameter = Boid::worldRadius * 1.1f * 2;
        const Vec3 dimensions ( diameter, diameter, diameter );
        typedef LQProximityDatabase<AbstractVehicle*> LQPDAV;
        pd = ET_NEW LQPDAV ( center, dimensions, divisions );
        break;
    }
    case 1:
    {
        pd = ET_NEW BruteForceProximityDatabase<AbstractVehicle*> ();
        break;
    }
    }

    // switch each boid to new PD
    for ( Boid::groupType::iterator i=flock.begin(); i!=flock.end(); i++ )
        ( **i ).allocateProximityToken ( pd );

    // delete old PD (if any)
    delete oldPD;
}

//-----------------------------------------------------------------------------
void NetBoidsPlugin::handleFunctionKeys ( int keyNumber )
{
    switch ( keyNumber )
    {
    case 49: //1
        addBoidToFlock ();
        break;
    case 50:
        removeBoidFromFlock ();
        break;
    case 51:
        nextPD ();
        break;
    case 52:
        nextBoundaryCondition ();
        break;
    case 53:
        printLQbinStats ();
        break;
    }
}

//-----------------------------------------------------------------------------
void NetBoidsPlugin::printLQbinStats ( void )
{
#ifndef NO_LQ_BIN_STATS
    AbstractVehicleGroup kVG ( this->allVehicles() );
    int min, max;
    float average;
    Boid& aBoid = ** ( flock.begin() );
    aBoid.m_pkProximityToken->getBinPopulationStats ( min, max, average );
    std::cout << std::setprecision ( 2 )
              << std::setiosflags ( std::ios::fixed );
    std::cout << "Bin populations: min, max, average: "
              << min << ", " << max << ", " << average
              << " (non-empty bins)" << std::endl;
    std::cout << "Boid neighbors:  min, max, average: "
              << Boid::minNeighbors << ", "
              << Boid::maxNeighbors << ", "
              << ( ( float ) Boid::totalNeighbors ) / ( ( float ) kVG.population() )
              << std::endl;
#endif // NO_LQ_BIN_STATS
}

//-----------------------------------------------------------------------------
void NetBoidsPlugin::printMiniHelpForFunctionKeys ( void ) const
{
    std::ostringstream message;
    message << "Function keys handled by ";
    message << '"' << name() << '"' << ':' << std::ends;
    EduNet::Log::printMessage ( message );
    EduNet::Log::printMessage ( "  F1     add a boid to the flock." );
    EduNet::Log::printMessage ( "  F2     remove a boid from the flock." );
    EduNet::Log::printMessage ( "  F3     use next proximity database." );
    EduNet::Log::printMessage ( "  F4     next flock boundary condition." );
    EduNet::Log::printMessage ( "" );
}

//-----------------------------------------------------------------------------
void NetBoidsPlugin::addBoidToFlock ( void )
{
    osAbstractVehicle* pkVehicle = this->createVehicle ( ET_CID_NETBOID );
    this->addVehicle ( pkVehicle );
}


//-----------------------------------------------------------------------------
void NetBoidsPlugin::removeBoidFromFlock ( void )
{
    AbstractVehicleGroup kVG ( this->allVehicles() );
    if ( kVG.population() > 0 )
    {
        const AbstractEntityFactory* pkFactory = this->getEntityFactory();

		OpenSteer::Boid::groupType::iterator it = flock.begin();
		OpenSteer::Boid::groupType::const_iterator itEnd = flock.end();
		while(it != itEnd)
		{
			AbstractVehicle* boid = *it;
			if (true == removeBoidFromFactory(boid, pkFactory) )
			{
				this->removeVehicle( boid );
				return;
			}
			else
			{
				++it;
			}			
		}		
    }
}

//-----------------------------------------------------------------------------
bool NetBoidsPlugin::removeBoidFromFactory(AbstractVehicle* boid,
	const AbstractEntityFactory* pkFactory)
{
	if ( NULL != pkFactory )
	{
		return pkFactory->destroyVehicle ( boid );
	}
	return true;
}
//-----------------------------------------------------------------------------
AbstractVehicle* NetBoidsPlugin::createVehicle (
    EntityClassId classId ) const
{
    AbstractVehicle* pkVehicle = NULL;
    const AbstractEntityFactory* pkFactory = this->getEntityFactory();
    if ( NULL != pkFactory )
    {
        pkVehicle = pkFactory->createVehicle ( classId );
    }
    return pkVehicle;
}
//-----------------------------------------------------------------------------
void NetBoidsPlugin::addVehicle ( osAbstractVehicle* pkVehicle )
{
    AbstractVehicleGroup kVG ( this->allVehicles() );
    kVG.addVehicleToPlugin ( pkVehicle, this );
}
//-----------------------------------------------------------------------------
void NetBoidsPlugin::removeVehicle ( osAbstractVehicle* pkVehicle )
{
    AbstractVehicleGroup kVG ( this->allVehicles() );
    kVG.removeVehicleFromPlugin ( pkVehicle );
}
//-----------------------------------------------------------------------------
Boid::groupType::iterator NetBoidsPlugin::FindBoid ( const Boid* pkBoid )
{
    Boid::groupType::iterator kIter = flock.begin();
    Boid::groupType::iterator kIterEnd = flock.end();
    while ( kIter != kIterEnd )
    {
        if ( *kIter == pkBoid )
        {
            return kIter;
        }
        ++kIter;
    }
    return kIterEnd;
}


//-----------------------------------------------------------------------------
void NetBoidsPlugin::nextBoundaryCondition ( void )
{
    SetCurrentBoundaryCondition ( ( EBoidConstraintType ) ( ( int ) constraint + 1 ) );
}

void NetBoidsPlugin::SetCurrentBoundaryCondition (
    const EBoidConstraintType  eType,
    bool bLocalChange )
{
    if ( eType != this->constraint )
    {
        this->bWasLocalChange = bLocalChange;
        this->constraint = eType;
        updateObstacles ();
    }
}
//-----------------------------------------------------------------------------
void NetBoidsPlugin::initObstacles ( void )
{
    this->bWasLocalChange = false;
    constraint = EBoidConstraintType_none;

    insideBigSphere.radius = Boid::worldRadius;
    insideBigSphere.setSeenFrom ( Obstacle::inside );

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
    outsideSphere1.center.set ( p, z, z );
    outsideSphere2.center.set ( m, z, z );
    outsideSphere3.center.set ( z, p, z );
    outsideSphere4.center.set ( z, m, z );
    outsideSphere5.center.set ( z, z, p );
    outsideSphere6.center.set ( z, z, m );

    const Vec3 tiltF = Vec3 ( 1.0f, 1.0f, 0.0f ).normalized ();
    const Vec3 tiltS ( 0.0f, 0.0f, 1.0f );
    const Vec3 tiltU = Vec3 ( -1.0f, 1.0f, 0.0f ).normalized ();

    bigRectangle.width = 50.0f;
    bigRectangle.height = 80.0f;
    bigRectangle.setSeenFrom ( Obstacle::both );
    bigRectangle.setForward ( tiltF );
    bigRectangle.setSide ( tiltS );
    bigRectangle.setUp ( tiltU );

    outsideBigBox.width = 50.0f;
    outsideBigBox.height = 80.0f;
    outsideBigBox.depth = 20.0f;
    outsideBigBox.setForward ( tiltF );
    outsideBigBox.setSide ( tiltS );
    outsideBigBox.setUp ( tiltU );

    insideBigBox = outsideBigBox;
    insideBigBox.setSeenFrom ( Obstacle::inside );

    updateObstacles ();
}

//-----------------------------------------------------------------------------
// update Boid::obstacles list when constraint changes
void NetBoidsPlugin::updateObstacles ( void )
{
    // first clear out obstacle list
    this->allObstacles().clear ();

    // add back obstacles based on mode
    switch ( constraint )
    {
    default:
        // reset for wrap-around, fall through to first case:
        constraint = EBoidConstraintType_none;
    case EBoidConstraintType_none:
        break;
    case EBoidConstraintType_insideSphere:
        this->allObstacles().push_back ( &insideBigSphere );
        break;
    case EBoidConstraintType_outsideSphere:
        this->allObstacles().push_back ( &insideBigSphere );
        this->allObstacles().push_back ( &outsideSphere0 );
        break;
    case EBoidConstraintType_outsideSpheres:
        this->allObstacles().push_back ( &insideBigSphere );
    case EBoidConstraintType_outsideSpheresNoBig:
        this->allObstacles().push_back ( &outsideSphere1 );
        this->allObstacles().push_back ( &outsideSphere2 );
        this->allObstacles().push_back ( &outsideSphere3 );
        this->allObstacles().push_back ( &outsideSphere4 );
        this->allObstacles().push_back ( &outsideSphere5 );
        this->allObstacles().push_back ( &outsideSphere6 );
        break;
    case EBoidConstraintType_rectangle:
        this->allObstacles().push_back ( &insideBigSphere );
        this->allObstacles().push_back ( &bigRectangle );
    case EBoidConstraintType_rectangleNoBig:
        this->allObstacles().push_back ( &bigRectangle );
        break;
    case EBoidConstraintType_outsideBox:
        this->allObstacles().push_back ( &insideBigSphere );
        this->allObstacles().push_back ( &outsideBigBox );
        break;
    case EBoidConstraintType_insideBox:
        this->allObstacles().push_back ( &insideBigBox );
        break;
    }
}

//-----------------------------------------------------------------------------
void NetBoidsPlugin::drawObstacles ( OpenSteer::AbstractRenderer* pRenderer )
{
	Camera* pCam(pRenderer->AccessCamera());
	Vec3 kPos = pCam ? pCam->position() : Vec3::zero;
    for ( ObstacleIterator o = this->allObstacles().begin();
          o != this->allObstacles().end();
          o++ )
    {
        ( **o ).draw ( pRenderer, false, // draw in wireframe
                       ( ( *o == &insideBigSphere ) ?
                         Color ( 0.2f, 0.2f, 0.4f ) :
                         Color ( 0.1f, 0.1f, 0.2f ) ),
                       kPos );
    }
}

//-----------------------------------------------------------------------------
void NetBoidsPlugin::tempDrawRectangle ( OpenSteer::AbstractRenderer* pRenderer,
										const RectangleObstacle& rect, const Color& color )
{
    float w = rect.width / 2;
    float h = rect.height / 2;

    Vec3 v1 = rect.globalizePosition ( Vec3 ( w,  h, 0 ) );
    Vec3 v2 = rect.globalizePosition ( Vec3 ( -w,  h, 0 ) );
    Vec3 v3 = rect.globalizePosition ( Vec3 ( -w, -h, 0 ) );
    Vec3 v4 = rect.globalizePosition ( Vec3 ( w, -h, 0 ) );

    pRenderer->drawLine ( v1, v2, color );
    pRenderer->drawLine ( v2, v3, color );
    pRenderer->drawLine ( v3, v4, color );
    pRenderer->drawLine ( v4, v1, color );
}

//-----------------------------------------------------------------------------
void NetBoidsPlugin::tempDrawBox ( OpenSteer::AbstractRenderer* pRenderer,
								  const BoxObstacle& box, const Color& color )
{
    const float w = box.width / 2;
    const float h = box.height / 2;
    const float d = box.depth / 2;
    const Vec3 p = box.position ();
    const Vec3 s = box.side ();
    const Vec3 u = box.up ();
    const Vec3 f = box.forward ();

    const Vec3 v1 = box.globalizePosition ( Vec3 ( w,  h,  d ) );
    const Vec3 v2 = box.globalizePosition ( Vec3 ( -w,  h,  d ) );
    const Vec3 v3 = box.globalizePosition ( Vec3 ( -w, -h,  d ) );
    const Vec3 v4 = box.globalizePosition ( Vec3 ( w, -h,  d ) );

    const Vec3 v5 = box.globalizePosition ( Vec3 ( w,  h, -d ) );
    const Vec3 v6 = box.globalizePosition ( Vec3 ( -w,  h, -d ) );
    const Vec3 v7 = box.globalizePosition ( Vec3 ( -w, -h, -d ) );
    const Vec3 v8 = box.globalizePosition ( Vec3 ( w, -h, -d ) );

    pRenderer->drawLine ( v1, v2, color );
    pRenderer->drawLine ( v2, v3, color );
    pRenderer->drawLine ( v3, v4, color );
    pRenderer->drawLine ( v4, v1, color );

    pRenderer->drawLine ( v5, v6, color );
    pRenderer->drawLine ( v6, v7, color );
    pRenderer->drawLine ( v7, v8, color );
    pRenderer->drawLine ( v8, v5, color );

    pRenderer->drawLine ( v1, v5, color );
    pRenderer->drawLine ( v2, v6, color );
    pRenderer->drawLine ( v3, v7, color );
    pRenderer->drawLine ( v4, v8, color );
}

//-----------------------------------------------------------------------------
wxWindow* NetBoidsPlugin::prepareGui( wxWindow* parent )
{

	wxWindow* window = BaseClass::prepareGui( parent );

	wxBoxSizer* buttonSizer = new wxBoxSizer( wxHORIZONTAL );
	window->GetSizer()->Add( buttonSizer );

	wxStaticText* label = new wxStaticText(window, wxID_STATIC, "Boids");
	buttonSizer->Add(label);

	wxSpinCtrl* spinCtrl = new wxSpinCtrl(window, id_AddBoid, wxT("Boids"),
		wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 1000, uiInitialFlockSize);
	spinCtrl->Bind( wxEVT_COMMAND_SPINCTRL_UPDATED, &NetBoidsPlugin::onChangeBoidPopulation, this );
	buttonSizer->Add( spinCtrl );

	return window;
}

//-----------------------------------------------------------------------------
void NetBoidsPlugin::onChangeBoidPopulation( wxSpinEvent& event )
{
	AbstractVehicleGroup kVG ( this->allVehicles() );
	while(event.GetPosition() != kVG.population() )
	{
		int pos = event.GetPosition() - kVG.population();

		if ( pos > 0)
		{
			addBoidToFlock();
		}
		else if (pos < 0)
		{
			removeBoidFromFlock();
		}
	}
	
	
}
