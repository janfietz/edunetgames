#ifndef __NETWORKVEHICLE_H__
#define __NETWORKVEHICLE_H__

#include "AbstractNetworkVehicle.h"

namespace OpenSteer {


	template <class Super>
	class NetworkVehicleMixin : public Super
	{
	public:

		// constructor
		NetworkVehicleMixin ();

		// destructor
		virtual ~NetworkVehicleMixin ();

		// AbstractNetworkVehicle interface
		virtual void testFunction();
		virtual void setIsRemoteObject( bool bIsRemote );
		virtual bool isRemoteObject( void ) const;

	private:
		bool m_bIsRemoteObject;


	};



	//----------------------------------------------------------------------------
	// Constructor and destructor
	template<class Super>
	NetworkVehicleMixin<Super>::NetworkVehicleMixin(void):m_bIsRemoteObject( false )
	{
	}


	template<class Super>
	NetworkVehicleMixin<Super>::~NetworkVehicleMixin(void)
	{
	}

	//----------------------------------------------------------------------------
	// interface
	template<class Super>
	void
	NetworkVehicleMixin<Super>::testFunction(void)
	{
		bool btest = true;
		btest = false;
	}

	template<class Super>
	void
	NetworkVehicleMixin<Super>::setIsRemoteObject( bool bIsRemote )
	{
		m_bIsRemoteObject = bIsRemote;
	}

	template<class Super>
	bool
	NetworkVehicleMixin<Super>::isRemoteObject( void ) const
	{
		return m_bIsRemoteObject;
	}
	// JF --

} // namespace OpenSteer

#endif