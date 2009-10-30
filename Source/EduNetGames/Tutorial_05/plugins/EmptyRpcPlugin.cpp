#include "EmptyRpcPlugin.h"
#include "EduNet/network/OSReplicaTypes.h"
#include "OpenSteerExtras/AbstractVehicleGroup.h"

EmptyRpcPlugin gEmptyRpcPlugin;

//-----------------------------------------------------------------------------
EmptyRpcPlugin::EmptyRpcPlugin()
{
	
}
//-----------------------------------------------------------------------------
void EmptyRpcPlugin::StartNetworkSession( void )
{
	BaseClass::StartNetworkSession();
	
}

void EmptyRpcPlugin::handleFunctionKeys (int keyNumber)
{
	switch (keyNumber)
    {
    //case 101:  ChangeReplicationInterval(5);         break; //GLUT_KEY_UP
    //case 103:  ChangeReplicationInterval(-5);    break; //GLUT_KEY_DOWN  
	default: BaseClass::handleFunctionKeys(keyNumber);
    }
	
}
