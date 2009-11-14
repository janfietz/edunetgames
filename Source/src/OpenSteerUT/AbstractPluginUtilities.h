#ifndef __ABSTRACTPLUGINUTILITIES_H__
#define __ABSTRACTPLUGINUTILITIES_H__

#include "EduNetCommon/EduNetCommon.h"

namespace OpenSteer
{

	template <class Super>
	class PluginAccessorMixin : public Super
	{
		ET_DECLARE_BASE( Super )
	public:

		// constructor
		PluginAccessorMixin():m_pkPlugin( NULL ) {};

		// destructor
		virtual ~PluginAccessorMixin()	{};


		virtual void setPlugin( class OpenSteer::AbstractPlugin* pPlugin )
		{
			this->m_pkPlugin = pPlugin;
		}

		virtual OpenSteer::AbstractPlugin* getPlugin( void ) const
		{
			return this->m_pkPlugin;
		}

	private:
		class OpenSteer::AbstractPlugin* m_pkPlugin;

	};

	typedef PluginAccessorMixin<OpenSteer::Entity> TPluginAccessor;


}

#endif //  __ABSTRACTPLUGINUTILITIES_H__