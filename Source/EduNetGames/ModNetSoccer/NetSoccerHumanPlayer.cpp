#include "NetSoccerHumanPlayer.h"

//---------------------------------------------------------------------------
NetSoccerHumanPlayer::NetSoccerHumanPlayer()
{

}

//---------------------------------------------------------------------------
NetSoccerHumanPlayer::~NetSoccerHumanPlayer()
{

}

//-----------------------------------------------------------------------------
OpenSteer::AbstractEntity* NetSoccerHumanPlayer::cloneEntity( void ) const
{
	return ET_NEW NetSoccerHumanPlayer();
}


//---------------------------------------------------------------------------
// AbstractNetSerializable interface
//-----------------------------------------------------------------------------
int NetSoccerHumanPlayer::serialize( RakNet::SerializeParameters *serializeParameters ) const
{
	return BaseClass::serialize( serializeParameters );
}

//-----------------------------------------------------------------------------
void NetSoccerHumanPlayer::deserialize( RakNet::DeserializeParameters *deserializeParameters )
{
	BaseClass::deserialize( deserializeParameters );
}

//-----------------------------------------------------------------------------
void NetSoccerHumanPlayer::serializeConstruction( RakNet::BitStream *constructionBitstream )
{
	return BaseClass::serializeConstruction( constructionBitstream );
}

//-----------------------------------------------------------------------------
bool NetSoccerHumanPlayer::deserializeConstruction( RakNet::BitStream *constructionBitstream )
{
	return BaseClass::deserializeConstruction( constructionBitstream );
}
