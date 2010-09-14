#include "EduNetPluginFactory.h"

//-----------------------------------------------------------------------------
EduNetPluginFactory::EduNetPluginFactory(void)
{

}

//-----------------------------------------------------------------------------
void EduNetPluginFactory::getPluginNames( enStringArray_t& kNames ) const
{
	this->fillStringArrayWithPluginName( kNames );
}
//-----------------------------------------------------------------------------
OpenSteer::AbstractPlugin* EduNetPluginFactory::createPluginByName( 
	const char* pszName )
{
	return this->createPluginByNameInternal( pszName );
}
