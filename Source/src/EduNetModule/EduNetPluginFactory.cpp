#include "EduNetPluginFactory.h"

namespace EduNet	{


//-----------------------------------------------------------------------------
PluginFactory::PluginFactory(void)
{

}

//-----------------------------------------------------------------------------
PluginFactory::~PluginFactory()
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
