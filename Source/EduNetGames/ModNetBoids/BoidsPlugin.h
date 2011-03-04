#ifndef NETBOIDPLUGIN_H
#define NETBOIDPLUGIN_H
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

#include "OpenSteerUT/OpenSteerUTTypes.h"
#include "OpenSteer/Plugin.h"

#include "NetBoid.h"


// Include names declared in the OpenSteer namespace into the
// namespaces to search to find names.
//using namespace OpenSteer;
class NetBoidFactory;

namespace OpenSteer
{
// enumerate demos of various constraints on the flock
enum EBoidConstraintType
{
    EBoidConstraintType_none,
    EBoidConstraintType_insideSphere,
    EBoidConstraintType_outsideSphere,
    EBoidConstraintType_outsideSpheres,
    EBoidConstraintType_outsideSpheresNoBig,
    EBoidConstraintType_rectangle,
    EBoidConstraintType_rectangleNoBig,
    EBoidConstraintType_outsideBox,
    EBoidConstraintType_insideBox
};

//-----------------------------------------------------------------------------
class NetBoidsPlugin : public OpenSteer::Plugin
{
    ET_DECLARE_BASE ( OpenSteer::Plugin );
public:
    NetBoidsPlugin ( bool bAddToRegistry = false );
    virtual ~NetBoidsPlugin();

    const char* name ( void ) const
    {
        return "Boids";
    }

    float selectionOrderSortKey ( void ) const
    {
        return 0.03f;
    }

    void open ( void );
    void close ( void );
    void reset ( void );
    void update ( const float currentTime, const float elapsedTime );
    virtual void redraw ( AbstractRenderer* pRenderer, const float currentTime, const float elapsedTime) OS_OVERRIDE;

    void nextPD ( void );
    void handleFunctionKeys ( int keyNumber );
    void printLQbinStats ( void );
    void printMiniHelpForFunctionKeys ( void ) const;

    // return an AVGroup containing each boid of the flock
    const OpenSteer::AVGroup& allVehicles ( void ) const
    {
        return ( const OpenSteer::AVGroup& ) flock;
    }
    OpenSteer::AVGroup& allVehicles ( void )
    {
        return ( OpenSteer::AVGroup& ) flock;
    }

    // JF ++
    virtual OpenSteer::ProximityDatabase* accessProximityDataBase ( void ) const
    {
        return this->pd;
    }

    const EBoidConstraintType GetCurrentBoundaryCondition ( void ) const
    {
        return this->constraint;
    }

    const bool WasBoundaryConditionChangedLocally ( void ) const
    {
        return this->bWasLocalChange;
    }

    void SetCurrentBoundaryCondition ( const EBoidConstraintType  eType,
                                       bool bLocalChange = true );

    void initGui ( void* pkUserdata );
    void addBoidToFlock ( void );
    void removeBoidFromFlock ( void );
    virtual void addVehicle( osAbstractVehicle* pkVehicle );
    virtual void removeVehicle ( osAbstractVehicle* pkVehicle);


    virtual osAbstractVehicle* createVehicle ( osEntityClassId ) const;

    OpenSteer::Boid::groupType& accessFlock( void )
    {
      return this->flock;
    }

private:

    // select next "boundary condition / constraint / obstacle"
    void nextBoundaryCondition ( void );

    void initObstacles ( void );
    // update Boid::obstacles list when constraint changes
    void updateObstacles ( void );
	void drawObstacles ( OpenSteer::AbstractRenderer* pRenderer );

	bool removeBoidFromFactory(AbstractVehicle* boid, const AbstractEntityFactory* pkFactory);

    class SO : public OpenSteer::SphereObstacle
    {
		void draw ( OpenSteer::AbstractRenderer* pRenderer, 
			const bool filled, const OpenSteer::Color& color, const OpenSteer::Vec3& vp ) const
        {
            pRenderer->drawSphereObstacle( *this, 10.0f, filled, color, vp );
        }
    };

    class RO : public OpenSteer::RectangleObstacle
    {
        void draw ( OpenSteer::AbstractRenderer* pRenderer, 
			const bool, const OpenSteer::Color& color, const OpenSteer::Vec3& ) const
        {
            tempDrawRectangle ( pRenderer, *this, color );
        }
    };

    class BO : public OpenSteer::BoxObstacle
    {
        void draw ( OpenSteer::AbstractRenderer* pRenderer, 
			const bool, const OpenSteer::Color& color, const OpenSteer::Vec3& ) const
        {
            tempDrawBox ( pRenderer,  *this, color );
        }
    };


	static void tempDrawRectangle ( OpenSteer::AbstractRenderer* pRenderer, const OpenSteer::RectangleObstacle& rect, const OpenSteer::Color& color );
    static void tempDrawBox ( OpenSteer::AbstractRenderer* pRenderer, const OpenSteer::BoxObstacle& box, const OpenSteer::Color& color );

    // flock: a group (STL vector) of pointers to all boids
    OpenSteer::Boid::groupType flock;

    // JF ++
    OpenSteer::Boid::groupType::iterator FindBoid ( const OpenSteer::Boid* pkBoid );
    // JF --
    // pointer to database used to accelerate proximity queries
    OpenSteer::ProximityDatabase* pd;


    // which of the various proximity databases is currently in use
    int cyclePD;

    // --------------------------------------------------------
    // the rest of this plug-in supports the various obstacles:
    // --------------------------------------------------------


    EBoidConstraintType constraint;
    bool bWasLocalChange;


    RO bigRectangle;
    BO outsideBigBox, insideBigBox;
    SO insideBigSphere, outsideSphere0, outsideSphere1, outsideSphere2,
    outsideSphere3, outsideSphere4, outsideSphere5, outsideSphere6;

	class NetBoidFactory* m_boidFactory;
	class CameraPlugin* m_pCameraPlugin;
};
}


#endif //NETBOIDPLUGIN_H
