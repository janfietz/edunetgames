// note: this file contains unused source code
//       either scrap removed from other files
//       or implementation ideas

//-------------------------------------------------------------------------
// state machine test
#if 0
#include "OpenSteer/AbstractState.h"
#include <string>

std::string	kTest;


void testStateMachine( void );

using namespace OpenSteer;
typedef TState<OpenSteer::LocalSpace> AbstractLocalSpaceState;
typedef TStateMachine<OpenSteer::LocalSpace> LocalSpaceStateMachine;

class LocalSpaceState : public AbstractLocalSpaceState
{
public:	
	/*	LocalSpaceState( )
	{
	}
	LocalSpaceState( const LocalSpaceState& kSource )
	{
	}
	*/	virtual void enter( OpenSteer::LocalSpace* ){};
	  virtual void exit( OpenSteer::LocalSpace* ){};

	  virtual void execute( OpenSteer::LocalSpace* pkEntity, osScalar fAccumTime, osScalar fDeltaTime )
	  {
		  bool bTest = true;
		  bTest = false;
	  }

};


typedef TStateVector<OpenSteer::LocalSpace> LocalSpaceStateVector;


void testStateMachine( void )
{

	LocalSpace kLocalSpace;
	LocalSpaceStateMachine kStateMachine;
	LocalSpaceState kState;
	kStateMachine.pushState( kState );
	kStateMachine.execute( &kLocalSpace, 1.0f, 2.0f );

}
#endif
//-------------------------------------------------------------------------

#if 0	
//-----------------------------------------------------------------------------
class Pedestrian : public SimpleVehicle
{
public:

	// type for a group of Pedestrians
	typedef std::vector<Pedestrian*> groupType;

	SteeringForceVehicleUpdate m_kSteeringForceUpdate;
	EulerVehicleUpdate m_kEulerUpdate;

	// constructor
	Pedestrian (ProximityDatabase& pd):
	m_kSteeringForceUpdate(*this),
		m_kEulerUpdate(*this)
	{
		// allocate a token for this boid in the proximity database
		proximityToken = NULL;
		newPD (pd);

		// reset Pedestrian state
		reset ();
	}

	// destructor
	virtual ~Pedestrian ()
	{
		// delete this boid's token in the proximity database
		delete proximityToken;
	}

	// reset all instance state
	void reset (void)
	{
		// reset the vehicle 
		SimpleVehicle::reset ();

		// max speed and max steering force (maneuverability)
		float fAgility = 1.0 + frandom01();
		setMaxSpeed ( 2.0 * fAgility );
		setMaxForce ( 8.0 * fAgility );

		// initially stopped
		setSpeed (0);

		// size of bounding sphere, for obstacle avoidance, etc.
		setRadius (0.5); // width = 0.7, add 0.3 margin, take half

		// set the path for this Pedestrian to follow
		path = getTestPath ();

		// set initial position
		// (random point on path + random horizontal offset)
		const float d = path->length() * frandom01();
		const float r = path->radius();
		const Vec3 randomOffset = randomVectorOnUnitRadiusXZDisk () * r;
		setPosition (path->mapPathDistanceToPoint (d) + randomOffset);

		// randomize 2D heading
		randomizeHeadingOnXZPlane ();

		// pick a random direction for path following (upstream or downstream)
		pathDirection = (frandom01() > 0.5) ? -1 : +1;

		// trail parameters: 3 seconds with 60 points along the trail
		setTrailParameters (3, 60);

		// notify proximity database that our position has changed
		proximityToken->updateForNewPosition (position());
	}

	// per frame simulation update
	void update (const float currentTime, const float elapsedTime)
	{
		// apply steering force to our momentum
		//            applySteeringForce (determineCombinedSteering (elapsedTime),
		//                                elapsedTime);
		// alternative way
		this->m_kSteeringForceUpdate.update( currentTime, elapsedTime );
		const Vec3& kSteeringForce = this->m_kSteeringForceUpdate.getForce();
		this->m_kEulerUpdate.setForce( kSteeringForce );
		this->m_kEulerUpdate.update( currentTime, elapsedTime );


		// reverse direction when we reach an endpoint
		if (gUseDirectedPathFollowing)
		{
			const Color darkRed (0.7f, 0, 0);
			float const pathRadius = path->radius();

			if (Vec3::distance (position(), gEndpoint0) < pathRadius )
			{
				pathDirection = +1;
				annotationXZCircle (pathRadius, gEndpoint0, darkRed, 20);
			}
			if (Vec3::distance (position(), gEndpoint1) < pathRadius )
			{
				pathDirection = -1;
				annotationXZCircle (pathRadius, gEndpoint1, darkRed, 20);
			}
		}

		// annotation
		annotationVelocityAcceleration (5, 0);
		recordTrailVertex (currentTime, position());

		// notify proximity database that our position has changed
		proximityToken->updateForNewPosition (position());
	}
#endif

//-------------------------------------------------------------------------
// Functions for drawing text (in GLUT's 9x15 bitmap font) in a given
// color, starting at a location on the screen which can be specified
// in screen space (draw2dTextAt2dLocation) or as the screen space
// projection of a location in 3d model space (draw2dTextAt3dLocation)
//
// based on code originally from:
//   Introduction to OpenGL - L23a - Displaying Text
//   http://www.york.ac.uk/services/cserv/sw/graphics/OPENGL/L23a.html

// xxx  Note: I *think* "const char* const s" means that both the pointer s
// xxx  AND the char string it points to are declared read only.  I should
// xxx  check that this is really the case.  I added it based on something
// xxx  from Telespace (Pedestrian constructor) xxx

// xxx  and for THAT matter, why not just use reference ("&") args instead?





//-----------------------------------------------------------------------------
// draw string s right-justified in the upper righthand corner


//     // XXX display the total number of AbstractVehicles created
//     {
//         std::ostringstream s;
//         s << "vehicles: " << xxx::SerialNumberCounter << std::ends;

//         // draw string s right-justified in the upper righthand corner
//         const int h = glutGet (GLUT_WINDOW_HEIGHT);
//         const int w = glutGet (GLUT_WINDOW_WIDTH);
//         const int fontWidth = 9; // for GLUT_BITMAP_9_BY_15
//         const int fontHeight = 15; // for GLUT_BITMAP_9_BY_15
//         const int x = w - (fontWidth * s.pcount());
//         const int y = h - (fontHeight + 5);
//         const Vec3 screenLocation (x, y, 0);
//         draw2dTextAt2dLocation (s, screenLocation, gWhite);
//     }



// // void draw2dTextAt3dLocation (const char* s,
// void draw2dTextAt3dLocation (const char* const s,
//                              const Vec3 location,
//                              const Vec3 color)
// {
//     // set text color and raster position
//     glColor3f (color.r(), color.g(), color.b());
//     glRasterPos3f (location.x, location.y, location.z);

//     // loop over each character in string (until null terminator)
//     int lines = 0;
//     for (const char* p = s; *p; p++)
//     {
//         if (*p == '\n')
//         {
//             // handle "new line" character, reset raster position
//             lines++;
//             const int fontHeight = 15; // for GLUT_BITMAP_9_BY_15
//             const int vOffset = lines * (fontHeight + 1);
//             glRasterPos3f (location.x, location.y-vOffset, location.z);

//         }
//         else
//         {
//             // otherwise draw character bitmap
//             glutBitmapCharacter (GLUT_BITMAP_9_BY_15, *p);
//         }
//     }
// }


// // void draw2dTextAt2dLocation (char* s,
// void draw2dTextAt2dLocation (const char* const s,
//                              const Vec3 location,
//                              const Vec3 color)
// {
//     // store OpenGL matrix mode
//     int savedMatrixMode;
//     glGetIntegerv (GL_MATRIX_MODE, &savedMatrixMode);

//     // clear projection transform
//     glMatrixMode (GL_PROJECTION);
//     glPushMatrix ();
//     glLoadIdentity ();

//     // set up orthogonal projection onto window's screen space
//     const float w = glutGet (GLUT_WINDOW_WIDTH);
//     const float h = glutGet (GLUT_WINDOW_HEIGHT);
//     glOrtho (0.0f, w, 0.0f, h, -1.0f, 1.0f);

//     // clear model transform
//     glMatrixMode (GL_MODELVIEW);
//     glPushMatrix ();
//     glLoadIdentity ();

//     // draw text at specified location (which is now interpreted as
//     // relative to screen space) and color
//     draw2dTextAt3dLocation (s, location, color);

//     // restore previous model/projection transformation state
//     glPopMatrix ();
//     glMatrixMode (GL_PROJECTION);
//     glPopMatrix ();

//     // restore OpenGL matrix mode
//     glMatrixMode (savedMatrixMode);
// }




// // for now these cannot be nested (would need to have a stack of saved
// // xxx  matrix modes instead of just a global).



// int gxxxsavedMatrixMode;


// inline void begin2dDrawing (void)
// {
//     // store OpenGL matrix mode
// //     int savedMatrixMode;
//     glGetIntegerv (GL_MATRIX_MODE, &gxxxsavedMatrixMode);

//     // clear projection transform
//     glMatrixMode (GL_PROJECTION);
//     glPushMatrix ();
//     glLoadIdentity ();

//     // set up orthogonal projection onto window's screen space
//     const float w = glutGet (GLUT_WINDOW_WIDTH);
//     const float h = glutGet (GLUT_WINDOW_HEIGHT);
//     glOrtho (0.0f, w, 0.0f, h, -1.0f, 1.0f);

//     // clear model transform
//     glMatrixMode (GL_MODELVIEW);
//     glPushMatrix ();
//     glLoadIdentity ();
// }


// inline void end2dDrawing (void)
// {
//     // restore previous model/projection transformation state
//     glPopMatrix ();
//     glMatrixMode (GL_PROJECTION);
//     glPopMatrix ();

//     // restore OpenGL matrix mode
//     glMatrixMode (gxxxsavedMatrixMode);
// }



// void draw2dTextAt3dLocation (const char* const s,
//                              const Vec3 location,
//                              const Vec3 color)
// {
//     // set text color and raster position
//     glColor3f (color.r(), color.g(), color.b());
//     glRasterPos3f (location.x, location.y, location.z);

//     // loop over each character in string (until null terminator)
//     int lines = 0;
//     for (const char* p = s; *p; p++)
//     {
//         if (*p == '\n')

//             // handle "new line" character, reset raster position
//             lines++;
//             const int fontHeight = 15; // for GLUT_BITMAP_9_BY_15
//             const int vOffset = lines * (fontHeight + 1);
//             glRasterPos3f (location.x, location.y-vOffset, location.z);

//         }
//         else
//         {
//             // otherwise draw character bitmap
//             glutBitmapCharacter (GLUT_BITMAP_9_BY_15, *p);
//         }
//     }
// }


// void draw2dTextAt2dLocation (const char* const s,
//                              const Vec3 location,
//                              const Vec3 color)
// {
// //     // store OpenGL matrix mode
// //     int savedMatrixMode;
// //     glGetIntegerv (GL_MATRIX_MODE, &savedMatrixMode);

// //     // clear projection transform
// //     glMatrixMode (GL_PROJECTION);
// //     glPushMatrix ();
// //     glLoadIdentity ();

// //     // set up orthogonal projection onto window's screen space
// //     const float w = glutGet (GLUT_WINDOW_WIDTH);
// //     const float h = glutGet (GLUT_WINDOW_HEIGHT);
// //     glOrtho (0.0f, w, 0.0f, h, -1.0f, 1.0f);

// //     // clear model transform
// //     glMatrixMode (GL_MODELVIEW);
// //     glPushMatrix ();
// //     glLoadIdentity ();

//     begin2dDrawing ();

//     // draw text at specified location (which is now interpreted as
//     // relative to screen space) and color
//     draw2dTextAt3dLocation (s, location, color);

// //     // restore previous model/projection transformation state
// //     glPopMatrix ();
// //     glMatrixMode (GL_PROJECTION);
// //     glPopMatrix ();

// //     // restore OpenGL matrix mode
// //     glMatrixMode (savedMatrixMode);

//     end2dDrawing ();

// }


// // for now these cannot be nested (would need to have a stack of saved
// // xxx  matrix modes instead of just a global).



// int gxxxsavedMatrixMode;


// inline void begin2dDrawing (void)
// {
//     // store OpenGL matrix mode
// //     int savedMatrixMode;
//     glGetIntegerv (GL_MATRIX_MODE, &gxxxsavedMatrixMode);

//     // clear projection transform
//     glMatrixMode (GL_PROJECTION);
//     glPushMatrix ();
//     glLoadIdentity ();

//     // set up orthogonal projection onto window's screen space
//     const float w = glutGet (GLUT_WINDOW_WIDTH);
//     const float h = glutGet (GLUT_WINDOW_HEIGHT);
//     glOrtho (0.0f, w, 0.0f, h, -1.0f, 1.0f);

//     // clear model transform
//     glMatrixMode (GL_MODELVIEW);
//     glPushMatrix ();
//     glLoadIdentity ();
// }


// inline void end2dDrawing (void)
// {
//     // restore previous model/projection transformation state
//     glPopMatrix ();
//     glMatrixMode (GL_PROJECTION);
//     glPopMatrix ();

//     // restore OpenGL matrix mode
//     glMatrixMode (gxxxsavedMatrixMode);
// }



// void draw2dTextAt3dLocation (const char* const s,
//                              const Vec3 location,
//                              const Vec3 color)
// {
//     // set text color and raster position
//     glColor3f (color.r(), color.g(), color.b());
//     glRasterPos3f (location.x, location.y, location.z);

//     // switch into 2d screen space in case we need to handle a new-line
//     begin2dDrawing ();
//     GLint rasterPosition[4];
//     glGetIntegerv(GL_CURRENT_RASTER_POSITION, rasterPosition);
//     glRasterPos2i (rasterPosition[0], rasterPosition[1]);

//     // loop over each character in string (until null terminator)
//     int lines = 0;
//     for (const char* p = s; *p; p++)
//     {
//         if (*p == '\n')
//         {
//             // handle new-line character, reset raster position
//             lines++;
//             const int fontHeight = 15; // for GLUT_BITMAP_9_BY_15
//             const int vOffset = lines * (fontHeight + 1);
//             glRasterPos2i (rasterPosition[0], rasterPosition[1] - vOffset);
//         }
//         else
//         {
//             // otherwise draw character bitmap
//             glutBitmapCharacter (GLUT_BITMAP_9_BY_15, *p);
//         }
//     }

//     // xxx
//     end2dDrawing ();
// }


// void draw2dTextAt2dLocation (const char* const s,
//                              const Vec3 location,
//                              const Vec3 color)
// {
//     begin2dDrawing ();

//     // draw text at specified location (which is now interpreted as
//     // relative to screen space) and color
//     draw2dTextAt3dLocation (s, location, color);

//     end2dDrawing ();
// }


// // for now these cannot be nested (would need to have a stack of saved
// // xxx  matrix modes instead of just a global).
// int gxxxsavedMatrixMode;


// void draw2dTextAt3dLocation (const char* const s,
//                              const Vec3 location,
//                              const Vec3 color)
// {
//     // set text color and raster position
//     glColor3f (color.r(), color.g(), color.b());
//     glRasterPos3f (location.x, location.y, location.z);

//     // switch into 2d screen space in case we need to handle a new-line
//     GLint rasterPosition[4];
//     glGetIntegerv (GL_CURRENT_RASTER_POSITION, rasterPosition);
//     const GLint originalMatrixMode = begin2dDrawing ();

//     //xxx uncommenting this causes the "2d" version to print the wrong thing
//     //xxx with it out the first line of a multi-line "3d" string jiggles
//     //glRasterPos2i (rasterPosition[0], rasterPosition[1]);

//     // loop over each character in string (until null terminator)
//     int lines = 0;
//     for (const char* p = s; *p; p++)
//     {
//         if (*p == '\n')
//         {
//             // handle new-line character, reset raster position
//             lines++;
//             const int fontHeight = 15; // for GLUT_BITMAP_9_BY_15
//             const int vOffset = lines * (fontHeight + 1);
//             glRasterPos2i (rasterPosition[0], rasterPosition[1] - vOffset);
//         }
//         else
//         {
//             // otherwise draw character bitmap
//             glutBitmapCharacter (GLUT_BITMAP_9_BY_15, *p);
//         }
//     }

//     // switch back out of 2d screen space
//     end2dDrawing (originalMatrixMode);
// }


// void draw2dTextAt2dLocation (const char* const s,
//                              const Vec3 location,
//                              const Vec3 color)
// {
//     const GLint originalMatrixMode = begin2dDrawing ();

//     // draw text at specified location (which is now interpreted as
//     // relative to screen space) and color
//     draw2dTextAt3dLocation (s, location, color);

//     end2dDrawing (originalMatrixMode);
// }
