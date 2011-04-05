#include "EduNetPluginFactory.h"

namespace EduNet	{


//-----------------------------------------------------------------------------
PluginFactory::PluginFactory(void)
{

}

//-----------------------------------------------------------------------------
void PluginFactory::getPluginNames( enStringArray_t& kNames ) const
{
	this->fillStringArrayWithPluginName( kNames );
}
//-----------------------------------------------------------------------------
OpenSteer::AbstractPlugin* PluginFactory::createPluginByName( 
	const char* pszName )
{
	return this->createPluginByNameInternal( pszName );
}

}
