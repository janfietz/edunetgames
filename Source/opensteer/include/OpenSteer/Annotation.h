//-----------------------------------------------------------------------------
//
//
//! OpenSteer -- Steering Behaviors for Autonomous Characters
//
//! Copyright (c) 2002-2005, Sony Computer Entertainment America
//! Original author: Craig Reynolds <craig_reynolds@playstation.sony.com>
//
//! Permission is hereby granted, free of charge, to any person obtaining a
//! copy of this software and associated documentation files (the "Software"),
//! to deal in the Software without restriction, including without limitation
//! the rights to use, copy, modify, merge, publish, distribute, sublicense,
//! and/or sell copies of the Software, and to permit persons to whom the
//! Software is furnished to do so, subject to the following conditions:
//
//! The above copyright notice and this permission notice shall be included in
//! all copies or substantial portions of the Software.
//
//! THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//! IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//! FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
//! THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//! LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
//! FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
//! DEALINGS IN THE SOFTWARE.
//
//
//-----------------------------------------------------------------------------
//
//
//! AnnotationMixin
//
//! This mixin (class with templated superclass) adds OpenSteerDemo-based
//! graphical annotation functionality to a given base class, which is
//! typically something that supports the AbstractVehicle interface.
//
//! 10-04-04 bk:  put everything into the OpenSteer namespace
//! 04-01-03 cwr: made into a mixin
//! 07-01-02 cwr: created (as Annotation.h) 
//
//
//-----------------------------------------------------------------------------


#ifndef OPENSTEER_ANNOTATION_H
#define OPENSTEER_ANNOTATION_H
#include "OpenSteer/AbstractRenderer.h"
#include "OpenSteer/Vec3.h"
#include "OpenSteer/Color.h"
#include <sstream>

//-----------------------------------------------------------------------------


namespace OpenSteer {

	enum EAnnotationMode
	{
		EAnnotationMode_global,
		EAnnotationMode_local,
		EAnnotationMode_count
	};

	class Annotation
	{
	public:
		virtual void draw( AbstractRenderer* pRenderer, bool bDrawPhaseActive ) const {};
	};

	typedef std::vector<Annotation> AnnotationList;

	//////////////////////////////////////////////////////////////////////////
	class DiscAnnotation : public Annotation
	{
	public:
		DiscAnnotation(const float radius,
			const Vec3& axis,
			const Vec3& center,
			const Color& color,
			const int segments,
			const bool filled,
			const bool in3d = false):
		m_radius(radius),
		m_axis(axis),
		m_center(center),
		m_color(color),
		m_segments(segments),
		m_filled(filled),
		m_b3d(in3d){};

		virtual void draw( AbstractRenderer* pRenderer, bool bDrawPhaseActive ) const OS_OVERRIDE;
	protected:
		float m_radius;
		Vec3 m_axis;
		Vec3 m_center;
		Color m_color;
		int m_segments;
		bool m_filled;
		bool m_b3d;
	};

	//////////////////////////////////////////////////////////////////////////
	class LineAnnotation : public Annotation
	{
	public:
		LineAnnotation(const Vec3& startPoint,
			const Vec3& endPoint,
			const Color& color):
		m_startPoint(startPoint),
		m_endPoint(endPoint),
		m_color(color){};

		virtual void draw( AbstractRenderer* pRenderer, bool bDrawPhaseActive ) const OS_OVERRIDE;
	private:
		Vec3 m_startPoint;
		Vec3 m_endPoint;
		Color m_color;
	};


    //! graphical annotation: master on/off switch
    bool annotationIsOn (void);
    void setAnnotationOn (void);
    void setAnnotationOff (void);
    bool toggleAnnotationState (void);

	bool drawPhaseActive (void);

    template <class Super>
    class AnnotationMixin : public Super
    {
    public:

        //! constructor
        AnnotationMixin ();

        //! destructor
        virtual ~AnnotationMixin ();

        //-------------------------------------------------------------------------
        //! trails / streamers
        //
        //! these routines support visualization of a vehicle's recent path
        //
        //! XXX conceivable trail/streamer should be a separate class,
        //! XXX Annotation would "has-a" one (or more))

        //! record a position for the current time, called once per update
        void recordTrailVertex (const float currentTime, const Vec3& position);

        //! draw the trail as a dotted line, fading away with age
		void drawTrail ( AbstractRenderer* pRenderer) 
		{
			drawTrail (pRenderer, grayColor (0.7f), gWhite);
		}
		void drawTrail ( AbstractRenderer* pRenderer, const Color& trailColor, const Color& tickColor);

        //! set trail parameters: the amount of time it represents and the
        //! number of samples along its length.  re-allocates internal buffers.
        void setTrailParameters (const float duration, const int vertexCount);

        //! forget trail history: used to prevent long streaks due to teleportation
        void clearTrailHistory (void);

        //-------------------------------------------------------------------------
        //! drawing of lines, circles and (filled) disks to annotate steering
        //! behaviors.  When called during OpenSteerDemo's simulation update phase,
        //! these functions call a "deferred draw" routine which buffer the
        //! arguments for use during the redraw phase.
        //
        //! note: "circle" means unfilled
        //!       "disk" means filled
        //!       "XZ" means on a plane parallel to the X and Z axes (perp to Y)
        //!       "3d" means the circle is perpendicular to the given "axis"
        //!       "segments" is the number of line segments used to draw the circle

        //! draw an opaque colored line segment between two locations in space
		void annotationLine (AbstractRenderer* pRenderer,
							const Vec3& startPoint,
                            const Vec3& endPoint,
                            const Color& color) const;

        //! draw a circle on the XZ plane
        void annotationXZCircle (AbstractRenderer* pRenderer,
			const float radius,
                                 const Vec3& center,
                                 const Color& color,
                                 const int segments) const
        {
            annotationXZCircleOrDisk (pRenderer,radius, center, color, segments, false);
        }


        //! draw a disk on the XZ plane
        void annotationXZDisk (AbstractRenderer* pRenderer,
			const float radius,
                               const Vec3& center,
                               const Color& color,
                               const int segments) const
        {
            annotationXZCircleOrDisk (pRenderer,radius, center, color, segments, true);
        }


        //! draw a circle perpendicular to the given axis
        void annotation3dCircle (AbstractRenderer* pRenderer,
			const float radius,
                                 const Vec3& center,
                                 const Vec3& axis,
                                 const Color& color,
                                 const int segments) const
        {
            annotation3dCircleOrDisk (pRenderer,radius, center, axis, color, segments, false);
        }


        //! draw a disk perpendicular to the given axis
        void annotation3dDisk (AbstractRenderer* pRenderer,
			const float radius,
                               const Vec3& center,
                               const Vec3& axis,
                               const Color& color,
                               const int segments) const
        {
            annotation3dCircleOrDisk (pRenderer,radius, center, axis, color, segments, true);
        }

        //

        //-------------------------------------------------------------------------
        //! support for annotation circles

        void annotationXZCircleOrDisk (AbstractRenderer* pRenderer,
			const float radius,
                                       const Vec3& center,
                                       const Color& color,
                                       const int segments,
                                       const bool filled) const
        {
            annotationCircleOrDisk (pRenderer,radius,
                                    Vec3::zero,
                                    center,
                                    color,
                                    segments,
                                    filled,
                                    false); //! "not in3d" -> on XZ plane
        }


        void annotation3dCircleOrDisk (AbstractRenderer* pRenderer,
			const float radius,
                                       const Vec3& center,
                                       const Vec3& axis,
                                       const Color& color,
                                       const int segments,
                                       const bool filled) const
        {
            annotationCircleOrDisk (pRenderer,radius,
                                    axis,
                                    center,
                                    color,
                                    segments,
                                    filled,
                                    true); //! "in3d"
        }

        void annotationCircleOrDisk (AbstractRenderer* pRenderer,
			const float radius,
                                     const Vec3& axis,
                                     const Vec3& center,
                                     const Color& color,
                                     const int segments,
                                     const bool filled,
                                     const bool in3d) const;

		void setAnnotationMode( EAnnotationMode eMode )
		{
			this->m_eAnnotationMode = eMode;
		}

		EAnnotationMode getAnnotationMode( void ) const
		{
			return this->m_eAnnotationMode;
		}

		bool isAnnotated( void ) const
		{
			return ( ( this->m_eAnnotationMode == OpenSteer::EAnnotationMode_local ) || OpenSteer::annotationIsOn() );
		}

		virtual void collect3DTextAnnotation( std::ostringstream& kStream )
		{

		}

		void drawDeferredAnnotations( AbstractRenderer* pRenderer );
		void clearDeferredAnnotations();

        //-------------------------------------------------------------------------
    private:

        //! trails
        int trailVertexCount;       //! number of vertices in array (ring buffer)
        int trailIndex;             //! array index of most recently recorded point
        float trailDuration;        //! duration (in seconds) of entire trail
        float trailSampleInterval;  //! desired interval between taking samples
        float trailLastSampleTime;  //! global time when lat sample was taken
        int trailDottedPhase;       //! dotted line: draw segment or not
        Vec3 curPosition;           //! last reported position of vehicle
        Vec3* trailVertices;        //! array (ring) of recent points along trail
        char* trailFlags;           //! array (ring) of flag bits for trail points

		EAnnotationMode m_eAnnotationMode;

		mutable AnnotationList m_deferedAnnotations;

		
    };

} //! namespace OpenSteer



//-----------------------------------------------------------------------------
//! Constructor and destructor
template<class Super>
OpenSteer::AnnotationMixin<Super>::AnnotationMixin (void):
m_eAnnotationMode( OpenSteer::EAnnotationMode_global ),
trailVertices( NULL ),
trailFlags( NULL )
{

    //! xxx I wonder if it makes more sense to NOT do this here, see if the
    //! xxx vehicle class calls it to set custom parameters, and if not, set
    //! xxx these default parameters on first call to a "trail" function.  The
    //! xxx issue is whether it is a problem to allocate default-sized arrays
    //! xxx then to free them and allocate new ones
    setTrailParameters (5, 100);  //! 5 seconds with 100 points along the trail
}


template<class Super>
OpenSteer::AnnotationMixin<Super>::~AnnotationMixin (void)
{
    delete[] trailVertices;
    delete[] trailFlags;
}

//-----------------------------------------------------------------------------
//! set trail parameters: the amount of time it represents and the number of
//! samples along its length.  re-allocates internal buffers.
template<class Super>
void 
OpenSteer::AnnotationMixin<Super>::setTrailParameters (const float duration, 
                                                       const int vertexCount)
{
    //! record new parameters
    trailDuration = duration;
    trailVertexCount = vertexCount;

    //! reset other internal trail state
    trailIndex = 0;
    trailLastSampleTime = 0;
    trailSampleInterval = trailDuration / trailVertexCount;
    trailDottedPhase = 1;

    //! prepare trailVertices array: free old one if needed, allocate new one
    delete[] trailVertices;
    trailVertices = OS_NEW Vec3[trailVertexCount];

    //! prepare trailFlags array: free old one if needed, allocate new one
    delete[] trailFlags;
    trailFlags = OS_NEW char[trailVertexCount];

    //! initializing all flags to zero means "do not draw this segment"
    for (int i = 0; i < trailVertexCount; i++) trailFlags[i] = 0;
}


//-----------------------------------------------------------------------------
//! forget trail history: used to prevent long streaks due to teleportation
//
//! XXX perhaps this could be made automatic: triggered when the change in
//! XXX position is well out of the range of the vehicles top velocity
template<class Super>
void 
OpenSteer::AnnotationMixin<Super>::clearTrailHistory (void)
{
    //! brute force implementation, reset everything
    setTrailParameters( trailDuration, trailVertexCount );
}

//-----------------------------------------------------------------------------
//! record a position for the current time, called once per update
template<class Super>
void 
OpenSteer::AnnotationMixin<Super>::recordTrailVertex (const float currentTime,
                                                      const Vec3& position)
{
    const float timeSinceLastTrailSample = currentTime - trailLastSampleTime;
    if (timeSinceLastTrailSample > trailSampleInterval)
    {
        trailIndex = (trailIndex + 1) % trailVertexCount;
        trailVertices [trailIndex] = position;
        trailDottedPhase = (trailDottedPhase + 1) % 2;
        const int tick = (floorXXX (currentTime) >
                          floorXXX (trailLastSampleTime));
        trailFlags [trailIndex] = trailDottedPhase | (tick ? '\2' : '\0');
        trailLastSampleTime = currentTime;
    }
    curPosition = position;
}


//-----------------------------------------------------------------------------
//! draw the trail as a dotted line, fading away with age
template<class Super>
void 
OpenSteer::AnnotationMixin<Super>::drawTrail (AbstractRenderer* pRenderer, 
											  const Color& trailColor,
                                              const Color& tickColor)
{
    if ( this->isAnnotated() )
    {
        int index = trailIndex;
        for (int j = 0; j < trailVertexCount; j++)
        {
            //! index of the next vertex (mod around ring buffer)
            const int next = (index + 1) % trailVertexCount;

            //! "tick mark": every second, draw a segment in a different color
            const int tick = ((trailFlags [index] & 2) ||
                              (trailFlags [next] & 2));
            const Color color = tick ? tickColor : trailColor;

            //! draw every other segment
            if (trailFlags [index] & 1)
            {
                if (j == 0)
                {
                    //! draw segment from current position to first trail point
                    pRenderer->drawLineAlpha (curPosition,
                                   trailVertices [index],
                                   color,
                                   1);
                }
                else
                {
                    //! draw trail segments with opacity decreasing with age
                    const float minO = 0.05f; //! minimum opacity
                    const float fraction = (float) j / trailVertexCount;
                    const float opacity = (fraction * (1 - minO)) + minO;
                    pRenderer->drawLineAlpha (trailVertices [index],
                                   trailVertices [next],
                                   color,
                                   opacity);
                }
            }
            index = next;
        }
    }
}

//////////////////////////////////////////////////////////////////////////
template<class Super>
void OpenSteer::AnnotationMixin<Super>::drawDeferredAnnotations( AbstractRenderer* pRenderer )
{
	AnnotationList::const_iterator iter = m_deferedAnnotations.begin();
	const AnnotationList::const_iterator iterEnd = m_deferedAnnotations.end();
	while (iter != iterEnd)
	{
		(*iter).draw( pRenderer, drawPhaseActive() );
		++iter;
	}
}

//////////////////////////////////////////////////////////////////////////
template<class Super>
void OpenSteer::AnnotationMixin<Super>::clearDeferredAnnotations( )
{
	m_deferedAnnotations.clear();	
}

//-----------------------------------------------------------------------------
//! request (deferred) drawing of a line for graphical annotation
//
//! This is called during OpenSteerDemo's simulation phase to annotate behavioral
//! or steering state.  When annotation is enabled, a description of the line
//! segment is queued to be drawn during OpenSteerDemo's redraw phase.


#ifndef NOT_OPENSTEERDEMO  //! only when building OpenSteerDemo
template<class Super>
void 
OpenSteer::AnnotationMixin<Super>::annotationLine (AbstractRenderer* pRenderer,
												   const Vec3& startPoint,
                                                   const Vec3& endPoint,
                                                   const Color& color) const
{
	if ( this->isAnnotated() )
    {
		LineAnnotation kLine(startPoint, endPoint, color);

		if (NULL != pRenderer)
		{
			kLine.draw( pRenderer , drawPhaseActive() );		
		}
		else
		{
			LineAnnotation kLine(startPoint, endPoint, color);
			m_deferedAnnotations.push_back( kLine );
		}
        
    }
}
#else
template<class Super> void OpenSteer::AnnotationMixin<Super>::annotationLine
 (const Vec3&, const Vec3&, const Vec3&) const {}
#endif //! NOT_OPENSTEERDEMO


//-----------------------------------------------------------------------------
//! request (deferred) drawing of a circle (or disk) for graphical annotation
//
//! This is called during OpenSteerDemo's simulation phase to annotate behavioral
//! or steering state.  When annotation is enabled, a description of the
//! "circle or disk" is queued to be drawn during OpenSteerDemo's redraw phase.


#ifndef NOT_OPENSTEERDEMO  //! only when building OpenSteerDemo
template<class Super>
void 
OpenSteer::AnnotationMixin<Super>::annotationCircleOrDisk (AbstractRenderer* pRenderer,
															const float radius,
                                                           const Vec3& axis,
                                                           const Vec3& center,
                                                           const Color& color,
                                                           const int segments,
                                                           const bool filled,
                                                           const bool in3d) const
{
	if ( this->isAnnotated() )
    {
		DiscAnnotation circle(radius, axis, center, color, segments, filled, in3d);
		if (NULL != pRenderer)
		{
			circle.draw( pRenderer, drawPhaseActive() );
		}
		else
		{
			
			m_deferedAnnotations.push_back( circle );
		}
    }
}
#else
template<class Super>
void OpenSteer::AnnotationMixin<Super>::annotationCircleOrDisk
(const float, const Vec3&, const Vec3&, const Vec3&, const int,
 const bool, const bool) const {}
#endif //! NOT_OPENSTEERDEMO




//-----------------------------------------------------------------------------
#endif //! OPENSTEER_ANNOTATION_H
