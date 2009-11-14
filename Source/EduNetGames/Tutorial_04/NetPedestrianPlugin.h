#ifndef __NETPEDESTRIANPLUGIN_H__
#define __NETPEDESTRIANPLUGIN_H__

//-----------------------------------------------------------------------------
#include "EduNetCommon/EduNetCommon.h"
#include "NetPedestrianFactory.h"


//-----------------------------------------------------------------------------
class NetPedestrianPlugin : public OpenSteer::Plugin
{
	ET_DECLARE_BASE(OpenSteer::Plugin);
public:
	NetPedestrianPlugin( bool bAddToRegistry = true ):
	  BaseClass( bAddToRegistry ),pd(NULL)
	{
		this->setVehicleFactory( &this->m_kOfflinePedestrianFactory );
	}

	virtual ~NetPedestrianPlugin();

	OS_IMPLEMENT_CLASSNAME( NetPedestrianPlugin )
	//-----------------------------------------------------------------------------
	// OpenSteer::Plugin interface
	virtual const char* name() const { return this->getClassName(); };

	virtual float selectionOrderSortKey (void) const;

	virtual void open (void);
	virtual void update (const float currentTime, const float elapsedTime);
	virtual void redraw (const float currentTime, const float elapsedTime);

	virtual void close (void);

	virtual void reset (void);

	virtual void handleFunctionKeys (int keyNumber);

	virtual void printMiniHelpForFunctionKeys (void) const;

	virtual osProximityDatabase* accessProximityDataBase( void ) const
	{
		return this->pd;
	}

	virtual const osAVGroup& allVehicles (void) const;

	// implement to initialize additional gui functionality
	virtual void initGui( void* pkUserdata );

	void serialNumberAnnotationUtility (const osAbstractVehicle& selected,
		const osAbstractVehicle& nearMouse);

	void drawPathAndObstacles (void);

	void addPedestrianToCrowd(void);
	void addPedestrianToCrowd( osAbstractVehicle* pkVehicle );

	void removePedestrianFromCrowd (void);

	// for purposes of demonstration, allow cycling through various
	// types of proximity databases.  this routine is called when the
	// OpenSteerDemo user pushes a function key.
	void nextPD (void);

	// implement to create a vehicle of the specified class
	virtual osAbstractVehicle* createVehicle( osEntityClassId, osProximityDatabase* ) const;

private:
	NetPedestrianFactory m_kOfflinePedestrianFactory;

	osAVGroup crowd;

	osVector3 gridCenter;

	// pointer to database used to accelerate proximity queries
	osProximityDatabase* pd;

	// which of the various proximity databases is currently in use
	int cyclePD;
};



#endif //  __NETPEDESTRIANPLUGIN_H__