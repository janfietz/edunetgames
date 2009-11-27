#ifndef __ABSTRACTENTITYQUERY_H__
#define __ABSTRACTENTITYQUERY_H__

//-----------------------------------------------------------------------------
// Copyright (c) 2009, Jan Fietz, Cyrus Preuss
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without modification, 
// are permitted provided that the following conditions are met:
// 
// * Redistributions of source code must retain the above copyright notice, 
//   this list of conditions and the following disclaimer.
// * Redistributions in binary form must reproduce the above copyright notice, 
//   this list of conditions and the following disclaimer in the documentation 
//   and/or other materials provided with the distribution.
// * Neither the name of EduNetGames nor the names of its contributors
//   may be used to endorse or promote products derived from this software
//   without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
// IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES 
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON 
// ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, 
// EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//-----------------------------------------------------------------------------

#include "EduNetCommon/EduNetCommon.h"
#include "OpenSteer/AbstractEntity.h"
#include "OpenSteer/AbstractVehicle.h"
#include "OpenSteer/Obstacle.h"

namespace OpenSteer {

	using namespace OpenSteer;

	//-------------------------------------------------------------------------
	class AbstractEntityQuery
	{
	public:
		virtual ~AbstractEntityQuery() {}
		virtual bool isEntityMatch( const AbstractEntity* pkEntity ) const ET_ABSTRACT;
	};

	//-------------------------------------------------------------------------
	class EntityPointerQuery : public AbstractEntityQuery
	{
	public:
		EntityPointerQuery( const AbstractEntity* pkEntity ):m_pkEntity( pkEntity ) {}
		virtual ~EntityPointerQuery() {}
		virtual bool isEntityMatch( const AbstractEntity* pkEntity ) const
		{
			return ( pkEntity == this->m_pkEntity );
		}
	private:
		const AbstractEntity* m_pkEntity;
	};

	//-------------------------------------------------------------------------
	class NetworkEntityQuery : public AbstractEntityQuery
	{
	public:
		NetworkEntityQuery( NetworkId networkId ):m_networkId( networkId ) {}
		virtual ~NetworkEntityQuery() {}
		virtual bool isEntityMatch( const AbstractEntity* pkEntity ) const
		{
			return ( pkEntity->getNetworkId() == this->m_networkId );
		}
	private:
		NetworkId m_networkId;
	};

	//-------------------------------------------------------------------------
	template <class ContainerType, class EntityType >
	class TEntityQuery
	{
	public:
#pragma warning(push)
#pragma warning(disable: 4355) // warning C4355: 'this' : used in base member initializer list
		TEntityQuery( const ContainerType& kContainer, AbstractEntityQuery* pkQuery ):
		  m_kContainer(kContainer),
			  m_pkQuery(pkQuery)
		{
		}
#pragma warning(pop)

		virtual ~TEntityQuery()
		{
		}

		EntityType* findType( void ) const
		{
			ContainerType::const_iterator iter = m_kContainer.begin();
			ContainerType::const_iterator last = m_kContainer.end();
			while( iter != last )
			{
				if( true == m_pkQuery->isEntityMatch( *iter ) )
				{
					return (*iter);
				}
				++iter;
			}
			return NULL;
		}

		typename ContainerType::const_iterator find( void ) const
		{
			ContainerType::const_iterator iter = m_kContainer.begin();
			ContainerType::const_iterator last = m_kContainer.end();
			while( iter != last )
			{
				if( true == m_pkQuery->isEntityMatch( *iter ) )
				{
					return iter;
				}
				++iter;
			}
			return last;
		}

		typename ContainerType::iterator find( void )
		{
			ContainerType& kContainer = (ContainerType&)m_kContainer;
			ContainerType::iterator iter = kContainer.begin();
			ContainerType::iterator last = kContainer.end();
			while( iter != last )
			{
				if( true == m_pkQuery->isEntityMatch( *iter ) )
				{
					return iter;
				}
				++iter;
			}
			return last;
		}

		size_t find( ContainerType& kQuery ) const
		{
			ContainerType::const_iterator iter = m_kContainer.begin();
			ContainerType::const_iterator last = m_kContainer.end();
			while( iter != last )
			{
				if( true == m_pkQuery->isEntityMatch( *iter ) )
				{
					kQuery.push_back( *iter );
				}
				++iter;
			}
			return (kQuery.size() > 0);
		}

	private:
		const ContainerType& m_kContainer;
		AbstractEntityQuery* m_pkQuery;

	};

	typedef TEntityQuery<AVGroup, AbstractVehicle> AVQuery;
	typedef TEntityQuery<AbstractPlayerGroup, AbstractPlayer> AbstractPlayerQuery;
	typedef TEntityQuery<ObstacleGroup, AbstractObstacle> AbstractObstacleQuery;


}

#endif // __ABSTRACTENTITYQUERY_H__