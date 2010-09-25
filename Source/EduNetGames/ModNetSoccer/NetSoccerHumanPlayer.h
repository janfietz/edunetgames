#ifndef __NETSOCCERHUMANPLAYER_H__
#define __NETSOCCERHUMANPLAYER_H__

#include "EduNetConnect/SerializablePlayer.h"


//-----------------------------------------------------------------------------
class NetSoccerHumanPlayer : public ClientSerializablePlayer
{	
	ET_DECLARE_BASE( ClientSerializablePlayer )
public:

	NetSoccerHumanPlayer();
	virtual ~NetSoccerHumanPlayer();

	OS_IMPLEMENT_CLASSNAME( NetSoccerHumanPlayer )

	//---------------------------------------------------------------------------
	// AbstractNetSerializable interface
	virtual int serialize( RakNet::SerializeParameters *serializeParameters ) const;
	virtual void deserialize( RakNet::DeserializeParameters *deserializeParameters );
	virtual void serializeConstruction( RakNet::BitStream *constructionBitstream );
	virtual bool deserializeConstruction( RakNet::BitStream *constructionBitstream );

	virtual OpenSteer::AbstractEntity* cloneEntity( void ) const;

};

//-----------------------------------------------------------------------------
class NetSoccerHumanPlayerFactory : public OpenSteer::TEntityFactory<NetSoccerHumanPlayer>
{
	ET_DECLARE_BASE( OpenSteer::TEntityFactory<NetSoccerHumanPlayer> );
public:
	NetSoccerHumanPlayerFactory(){};
	virtual ~NetSoccerHumanPlayerFactory(){};
};

#endif // __NETSOCCERHUMANPLAYER_H__