#include "EduNetPluginFactory.h"

//-----------------------------------------------------------------------------
EduNetPluginFactory::EduNetPluginFactory(void)
{

}

//-----------------------------------------------------------------------------
size_t EduNetPluginFactory::getPluginNames( void )
{
	return 0;
}
//-----------------------------------------------------------------------------
OpenSteer::AbstractPlugin* EduNetPluginFactory::createPluginByName( 
	const char* pszName )
{
	return NULL;
}
