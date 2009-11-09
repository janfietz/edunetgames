#pragma once
#include "Tutorial_02/ClientPlugin.h"
#include "Tutorial_02/PeerPlugin.h"


#include "NetBoidReplicaConnection.h"


// ----------------------------------------------------------------------------
class NetPeerBoidPlugin : public PeerPlugin<OpenSteer::BoidsPlugin>
{
	ET_DECLARE_BASE(PeerPlugin<OpenSteer::BoidsPlugin>);
public:
	NetPeerBoidPlugin(bool bAddToRegistry = true);
	virtual ~NetPeerBoidPlugin(){};

	virtual const char* name (void) const {return "NetPeerBoidPlugin";};

	 bool requestInitialSelection (void) const {return true;}
	 virtual float selectionOrderSortKey (void) const { return 2.0f ;}
	 
	virtual bool needRedraw ( void ) const { return false; }
	
	void handleFunctionKeys (int keyNumber);


	virtual void StartNetworkSession( void );
	virtual void CreateContent( void );
	virtual void DeleteContent( void );

private:
	struct ReplicationParams
	{
		ReplicationParams():
			interval(30){}		
		RakNetTime interval;
	};
	void ChangeReplicationInterval( RakNetTime additionalTime );

	class NetBoidReplicaFactory* m_pkBoidFactory;
	class NetBoidConditionReplica* m_pkConditionReplic;	
	NetBoidReplicaManager m_kReplicaManager;

	ReplicationParams m_kReplicationSettings;
};

// ----------------------------------------------------------------------------
class NetClientBoidPlugin : public ClientPlugin<OpenSteer::BoidsPlugin>
{
	ET_DECLARE_BASE(ClientPlugin<OpenSteer::BoidsPlugin>);
public:
	NetClientBoidPlugin();
	virtual ~NetClientBoidPlugin(){};

	virtual const char* name (void) const {return "NetClientBoidPlugin";};

	virtual float selectionOrderSortKey (void) const { return 3.0f ;}

	virtual void StartNetworkSession( void );
	virtual void CreateContent( void );
	virtual void DeleteContent( void );

private:

	class NetBoidReplicaFactory* m_pkBoidFactory;
	class NetBoidConditionReplica* m_pkConditionReplic;
	NetBoidReplicaManager m_kReplicaManager;
	

};