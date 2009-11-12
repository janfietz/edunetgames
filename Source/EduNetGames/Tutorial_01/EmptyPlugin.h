#ifndef __EMPTYPLUGIN_H__
#define __EMPTYPLUGIN_H__
//-----------------------------------------------------------------------------
// implements a plugin doing nothing
#include "EduNet/common/EduNetCommon.h"
#include "EduNet/templates/TUpdatePeriod.h"
#include "EduNet/common/InstanceCount.h"

#include "EduNetGames.h"
#include "OpenSteerUT/AbstractVehicleGroup.h"



// ???
// class AbstractGamePlugin : public AbstractPlugin
// {
// public:
// }


// typedef GamePluginMixin<Plugin> SimpleGamePlugin;


// anonymous namespace
namespace EduNetGames{

	using namespace OpenSteer;

	class EmptyVehicle : public SimpleVehicle
	{
	public:

		EmptyVehicle():m_uiId( EmptyVehicle::ms_kInstanceCount.Constructor() )
		{
			this->reset();
		}

		virtual ~EmptyVehicle()
		{
			EmptyVehicle::ms_kInstanceCount.Destructor();
		}

		// reset vehicle state
		virtual void reset (void)
		{
			SimpleVehicle::reset();
			m_kUpdatePeriod.SetPeriodTime( 0.5f );
		}

		// per frame simulation update
		virtual void update (const float currentTime, const float elapsedTime)
		{
			// do nothing

			// test the UpdatePeriod
			size_t uiTicks = m_kUpdatePeriod.UpdateDeltaTime( elapsedTime );
			if( uiTicks >= 1 )
			{
				bool bTest = true;
				bTest  = false;
			}
		}

		// draw this character/vehicle into the scene
		virtual void draw (const float currentTime, const float elapsedTime)
		{
			drawBasic2dCircularVehicle (*this, gGray50);
		}
		EduNet::UpdatePeriodFloat m_kUpdatePeriod;
		static OpenSteer::InstanceTracker ms_kInstanceCount;
		size_t m_uiId;
		ET_IMPLEMENT_CLASS_NO_COPY( EmptyVehicle );
	};


	class EmptyPlugin : public Plugin
	{
		ET_DECLARE_BASE(Plugin);
	public:

		// construction / destruction
		EmptyPlugin (bool bAddToRegistry = true):BaseClass(bAddToRegistry){};
		virtual ~EmptyPlugin() {}

		virtual const char* name (void) const {return "EmptyPlugin";}

		virtual float selectionOrderSortKey (void) const {return 1.0f;}

		virtual void open (void)
		{
			m_kVehicle.reset();
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
			AbstractVehicleGroup kVehicles( m_kVehicles );
			kVehicles.reset( );
		}

		const AVGroup& allVehicles (void) const {return (const AVGroup&) m_kVehicles;}

		void initGui( void* pkUserdata );

	private:
		AVGroup m_kVehicles; // for allVehicles
		EmptyVehicle m_kVehicle;
		ET_IMPLEMENT_CLASS_NO_COPY( EmptyPlugin );
	};
}
#endif //__EMPTYPLUGIN_H__