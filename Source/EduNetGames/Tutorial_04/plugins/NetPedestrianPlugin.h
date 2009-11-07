#ifndef __NETPEDESTRIANPLUGIN_H__
#define __NETPEDESTRIANPLUGIN_H__

//-----------------------------------------------------------------------------
#include "EduNet/common/EduNetCommon.h"
#include "OpenSteerExtras/OpenSteerExtrasTypes.h"


//-----------------------------------------------------------------------------
class NetPedestrianPlugin : public OpenSteer::Plugin
{
public:

	virtual ~NetPedestrianPlugin();

	//-----------------------------------------------------------------------------
	// OpenSteer::Plugin interface
	virtual const char* name (void) const;

	virtual float selectionOrderSortKey (void) const;


	virtual void open (void);
	virtual void update (const float currentTime, const float elapsedTime);
	virtual void redraw (const float currentTime, const float elapsedTime);

	virtual void close (void);

	virtual void reset (void);

	virtual void handleFunctionKeys (int keyNumber);

	virtual void printMiniHelpForFunctionKeys (void) const;

	virtual const osAVGroup& allVehicles (void) const;

	// implement to initialize additional gui functionality
	virtual void initGui( void* pkUserdata );


	void serialNumberAnnotationUtility (const osAbstractVehicle& selected,
		const osAbstractVehicle& nearMouse);

	void drawPathAndObstacles (void);

	void addPedestrianToCrowd (void);

	void removePedestrianFromCrowd (void);


	// for purposes of demonstration, allow cycling through various
	// types of proximity databases.  this routine is called when the
	// OpenSteerDemo user pushes a function key.
	void nextPD (void);



private:

	// type for a group of Pedestrians
	typedef std::vector<class NetPedestrian*> groupType;
	// crowd: a group (STL vector) of all Pedestrians
	groupType crowd;
	typedef groupType::const_iterator iterator;

	osVector3 gridCenter;

	// pointer to database used to accelerate proximity queries
	osProximityDatabase* pd;

	// keep track of current flock size
	int population;

	// which of the various proximity databases is currently in use
	int cyclePD;
};



#endif //  __NETPEDESTRIANPLUGIN_H__