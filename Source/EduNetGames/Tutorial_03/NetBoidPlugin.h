#pragma once
#include "EduNetConnect/ClientPlugin.h"
#include "EduNetConnect/PeerPlugin.h"

#include "NetBoidReplicaConnection.h"
#include "BoidsPlugin.h"

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
	virtual void initGui( void* pkUserdata );


	virtual void StartNetworkSession( void );
	virtual void CreateContent( void );
	virtual void DeleteContent( void );

	void UpdateReplicationValue( void );

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
	NetClientBoidPlugin(bool bAddToRegistry = true);
	virtual ~NetClientBoidPlugin(){};

	virtual const char* name (void) const {return "NetClientBoidPlugin";};

	virtual float selectionOrderSortKey (void) const { return 3.0f ;}

	virtual void StartNetworkSession( void );
	virtual void CreateContent( void );
	virtual void DeleteContent( void );

private:
	
	class NetBoidConditionReplica* m_pkConditionReplic;
	NetBoidReplicaManager m_kReplicaManager;
	

};