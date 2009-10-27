//-----------------------------------------------------------------------------
// implements a plugin doing nothing
#include "EduNet/common/EduNetCommon.h"
#include "OpenSteer/OpenSteerDemo.h"
#include "../../OpenSteerExtras/AbstractVehicleGroup.h"

// anonymous namespace
namespace {

	using namespace OpenSteer;

	class EmptyVehicle : public SimpleVehicle
	{
		// per frame simulation update
		virtual void update (const float currentTime, const float elapsedTime)
		{
			// do nothing
		}

		// draw this character/vehicle into the scene
		virtual void draw (const float currentTime, const float elapsedTime)
		{
			drawBasic2dCircularVehicle (*this, gGray50);
		}
	};

	class EmptyPlugIn : public PlugIn
	{
	public:

		// construction / destruction
		EmptyPlugIn() {}
		virtual ~EmptyPlugIn() {}

		virtual const char* name (void) {return "A plugin doing nothing";}

		virtual float selectionOrderSortKey (void) {return 1.0f;}

		virtual void open (void)
		{
			OpenSteerDemo::selectedVehicle = &m_kVehicle;
			m_kVehicles.push_back( &m_kVehicle );

			// initialize camera
			OpenSteerDemo::init2dCamera( *OpenSteerDemo::selectedVehicle );
			OpenSteerDemo::camera.setPosition (
				10,
				OpenSteerDemo::camera2dElevation,
				10);
			OpenSteerDemo::camera.fixedPosition.set( 40, 40, 40 );
		}

		virtual void update (const float currentTime, const float elapsedTime)
		{
			AbstractVehicleGroup kVehicles( m_kVehicles );
			kVehicles.update( currentTime, elapsedTime );
		}

		virtual void redraw (const float currentTime, const float elapsedTime)
		{
			AbstractVehicleGroup kVehicles( m_kVehicles );
			kVehicles.redraw( currentTime, elapsedTime );
			if( NULL != OpenSteerDemo::selectedVehicle )
			{
				// update camera, tracking test vehicle
				OpenSteerDemo::updateCamera (currentTime, elapsedTime, *OpenSteerDemo::selectedVehicle );
				// draw "ground plane"
				OpenSteerDemo::gridUtility( OpenSteerDemo::selectedVehicle->position() );
			}
		}

		virtual void close (void)
		{
		}

		virtual void reset (void)
		{
		}

		const AVGroup& allVehicles (void) {return (const AVGroup&) m_kVehicles;}

		AVGroup m_kVehicles; // for allVehicles
		EmptyVehicle m_kVehicle;

	};

	EmptyPlugIn gEmptyPlugIn;

}