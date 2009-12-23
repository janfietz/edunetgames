#include "EduNetPluginFactory.h"

//-----------------------------------------------------------------------------
EduNetPluginFactory::EduNetPluginFactory(void)
{

}

//-----------------------------------------------------------------------------
void EduNetPluginFactory::getPluginNames( EdutNetStringList& kNames ) const
{
	this->fillStringArrayWithPluginName( kNames );
}
//-----------------------------------------------------------------------------
OpenSteer::AbstractPlugin* EduNetPluginFactory::createPluginByName( 
	const char* pszName )
{
	return this->createPluginByNameInternal( pszName );
}
