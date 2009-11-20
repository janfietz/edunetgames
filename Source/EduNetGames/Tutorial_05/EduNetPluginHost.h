#ifndef __EDUNETPLUGINHOST_H__
#define __EDUNETPLUGINHOST_H__

namespace EduNet
{
	class PluginHost
	{
	public:

		virtual const char* getCurrentPluginName( void ) const = 0;
		virtual void SelectPluginByName( const char* pszPluginName ) = 0;
	};
};
#endif // __EDUNETPLUGINHOST_H__