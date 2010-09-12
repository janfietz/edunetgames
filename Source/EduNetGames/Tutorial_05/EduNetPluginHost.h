#ifndef __EDUNETPLUGINHOST_H__
#define __EDUNETPLUGINHOST_H__

namespace EduNet
{
	class PluginHost
	{
	public:

		virtual const char* getCurrentPluginName( void ) const ET_ABSTRACT;
		virtual void selectPluginByName( const char* pszPluginName ) ET_ABSTRACT;
	};
};
#endif // __EDUNETPLUGINHOST_H__